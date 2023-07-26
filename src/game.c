typedef struct {
    struct timeval start;
    double elapsedTime;
    double timeInterval;
} Timing;

typedef struct {
    RuntimeArgs *runtimeArgs;
    Scene *scenes[MAX_SCENES];
    int sceneCount;
    Scene *currentScene;
    Player *player;
    AnimationManager *animationManager;
    AudioManager *audioManager;
    SpritesheetManager *spritesheetManager;
    Beastiary *beastiary;
    Log *log;
    Menu *menus[MAX_MENUS];
    int menuCount;
    Timing *timing;
} Game;

void setScene(Game *g, Scene *scene, char *entranceName) {
    addInfo(g->log, "setting scene to '%s'", scene->name);
    g->currentScene = scene;
    clearAnimations(g->animationManager);
    Mobile *mob = getPartyLeader(g->player);
    addAllAnimations(g->animationManager, mob->animations);
    if (entranceName != NULL) {
        Entrance *entrance = findEntrance(scene->exploration, entranceName);
        addDebug(g->log, "entrance found :: %s", entrance->name);
        mob->position.x = entrance->area.x + (entrance->area.width / 2) - (int) (MOB_COLLISION_WIDTH / 2);
        mob->position.y = entrance->area.y + (entrance->area.height / 2) - (int) (MOB_COLLISION_HEIGHT / 2);
        mob->direction = entrance->direction;
    }
    addInfo(g->log, "player position :: %f %f", mob->position.x, mob->position.y);
    renderExplorationLayers(g->currentScene->exploration);
    playMusic(g->audioManager, g->currentScene->music);
    addInfo(g->log, "finished setting scene to '%s'", g->currentScene->name);
}

Mobile *findMobById(Game *g, char *id) {
    for (int s = 0; s < g->sceneCount; s++) {
        for (int m = 0; m < g->scenes[s]->exploration->mobileCount; m++) {
            if (strcmp(g->scenes[s]->exploration->mobiles[m]->id, id) == 0) {
                return g->scenes[s]->exploration->mobiles[m];
            }
        }
    }
    addError(g->log, "mob not found: %s", id);
    return NULL;
}

When *mapWhen(Game *g, WhenData wd) {
    Mobile *trigger = NULL;
    Mobile *mob = getPartyLeader(g->player);
    if (wd.mob != NULL) {
        trigger = findMobById(g, wd.mob);
        addDebug(g->log, "mobile trigger is '%s'", trigger->name);
    }
    When *w = createWhen(
            mob,
            trigger,
            mapCondition(wd.condition),
            wd.story);
    addDebug(g->log, "condition: %s, mapped to: %d, story: %s",
             wd.condition,
             w->condition,
             wd.story);
    return w;
}

Then *mapThen(Game *g, ThenData td) {
    Mobile *target;
    Mobile *mob = getPartyLeader(g->player);
    if (td.player) {
        target = mob;
    } else {
        target = findMobById(g, td.mob);
    }
    Vector2 pos;
    if (td.position != NULL) {
        pos = getPositionFromString(td.position);
    } else {
        pos = (Vector2){0,0};
    }
    Then *t = createThen(
            target,
            &td.message[0],
            &td.story[0],
            td.direction,
            mapOutcome(td.action),
            pos,
            td.parallel
    );
    addDebug(g->log, "then story is '%s', outcome: %d, message: %s",
             t->story, t->outcome, t->message);
    return t;
}

ControlBlock *mapStorylineToControlBlock(Game *g, StorylineData *storyline) {
    ControlBlock *c = createControlBlock();
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    addDebug(g->log, "processing storyline with %d when and %d then clauses",
             storyline->when_count, storyline->then_count);
    for (int i = 0; i < storyline->when_count; i++) {
        c->when[i] = mapWhen(g, storyline->when[i]);
    }
    for (int i = 0; i < storyline->then_count; i++) {
        c->then[i] = mapThen(g, storyline->then[i]);
    }
    addDebug(g->log, "done processing when/then clauses");
    return c;
}

void loadScenes(Game *g, char *scenes[MAX_SCENES], char *sceneDirectories[MAX_SCENES]) {
    addDebug(g->log, "attempting to load scenes");
    for (int i = 0; i < g->sceneCount; i++) {
        g->scenes[i] = loadScene(
                g->log,
                g->animationManager,
                g->beastiary,
                g->runtimeArgs->indexDir,
                scenes[i],
                sceneDirectories[i],
                g->runtimeArgs);
        addDebug(g->log, "scene loaded :: %s (%d)", g->scenes[i]->name, i);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        addDebug(g->log, "scene storyline count :: %s -- %d",
                 g->scenes[i]->name, g->scenes[i]->storylineCount);
        for (int c = 0; c < g->scenes[i]->storylineCount; c++) {
            g->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(g, g->scenes[i]->storylines[c]);
            g->scenes[i]->controlBlockCount++;
        }
    }
}

void loadBeastiary(Game *g) {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/beastiary.yaml", g->runtimeArgs->indexDir);
    BeastiaryData *data = loadBeastiaryYaml(filePath);
    for (int i = 0; i < data->beasts_count; i++) {
        g->beastiary->beasts[i] = createBeastFromData(g->runtimeArgs->indexDir, &data->beasts[i]);
        addDebug(g->log, "beast '%s' created", g->beastiary->beasts[i]->id);
        g->beastiary->beastCount++;
    }
}

void attemptToUseExit(Game *game, Scene *scene, Entrance *entrance) {
    if (entrance == NULL) {
        addWarning(game->log, "no entrance found for '%s' scene", scene->name);
        return;
    }
    addDebug(game->log, "entrance %s found at %f, %f, %f, %f",
            entrance->name,
            entrance->area.x,
            entrance->area.y,
            entrance->area.width,
            entrance->area.height
    );
    getPartyLeader(game->player)->position = (Vector2) {
            entrance->area.x,
            entrance->area.y
    };
    setScene(game, scene, entrance->name);
}

void evaluateExits(Game *g) {
    addDebug(g->log, "exploration -- evaluate exits");
    Exploration *e = g->currentScene->exploration;
    int exit = atExit(e, g->player);
    if (exit > -1) {
        addDebug(g->log, "player at exit");
        char *sceneName = e->exits[exit]->scene;
        char *entranceName = e->exits[exit]->to;
        for (int i = 0; i < g->sceneCount; i++) {
            if (strcmp(sceneName, g->scenes[i]->name) == 0) {
                attemptToUseExit(
                        g,
                        g->scenes[i],
                        findEntrance(g->scenes[i]->exploration, entranceName)
                );
                return;
            }
        }
        addError(g->log, "warp to '%s' not found", sceneName);
    }
}

void explorationMenuKeyPressed(Game *g) {
    addMenu(g->currentScene->exploration, findMenu(g->menus, g->menuCount, PARTY_MENU));
}

void checkExplorationInput(Game *g) {
    addDebug(g->log, "exploration -- check player input");
    Mobile *mob = getPartyLeader(g->player);
    resetMoving(mob);
    getMobAnimation(mob)->isPlaying = 0;
    explorationCheckMoveKeys(g->player);
    if (IsKeyPressed(KEY_C)) {
        explorationDebugKeyPressed(g->currentScene->exploration, mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        explorationSpaceKeyPressed(g->currentScene->exploration, g->player, g->currentScene->activeControlBlocks);
    }
    if (IsKeyPressed(KEY_M)) {
        explorationMenuKeyPressed(g);
    }
    if (IsKeyPressed(KEY_T)) {
        addInfo(g->log, "player play time :: %ds", g->player->secondsPlayed);
    }
    if (IsKeyPressed(KEY_S)) {
        char filePath[MAX_FS_PATH_LENGTH];
        sprintf(filePath, "%s/%s/%s", g->runtimeArgs->indexDir, "_saves", "autosave.yaml");
        SaveData *save = createSaveData(g->currentScene->name, g->player);
        saveSaveData(save, filePath);
        free(save);
        addInfo(g->log, "game progress saved");
    }
}

void menuItemSelected(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    Menu *menu = getCurrentMenu(exploration);
    MenuSelectResponse *response = menu->selected(menu->cursor);
    if (response->type == OPEN_MENU) {
        addMenu(exploration, findMenu(g->menus, g->menuCount, response->menuType));
    } else if (response->type == CLOSE_MENU) {
        removeMenu(exploration);
    }
}

void checkMenuInput(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    if (IsKeyPressed(KEY_ESCAPE)) {
        removeMenu(exploration);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *menu = getCurrentMenu(exploration);
        menu->cursor++;
        normalizeMenuCursor(menu, g->player);
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *menu = getCurrentMenu(exploration);
        menu->cursor--;
        normalizeMenuCursor(menu, g->player);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        menuItemSelected(g);
    }
}

void doExplorationLoop(Game *g) {
    checkExplorationInput(g);
    drawExplorationView(g->currentScene->exploration, g->player, g->currentScene->activeControlBlocks);
    doMobileMovementUpdates(g->currentScene->exploration);
    processAnimations(g->animationManager);
    evaluateMovement(g->currentScene->exploration, g->player);
    evaluateExits(g);
    checkControls(g->currentScene, g->player);
    checkFights(g->currentScene, g->player);
}

void doFightLoop(Game *g) {
    checkFightInput(g->currentScene->fight, g->player);
    drawFightView(g->currentScene->encounters, g->currentScene->fight, g->player);
    processFightAnimations();
    checkControls(g->currentScene, g->player);
    checkRemoveFight(g->currentScene);
}

void doInGameMenuLoop(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    drawAllMenus(g->player, exploration->menus, exploration->menuCount);
    checkMenuInput(g);
}

void startTiming(Timing *t) {
    gettimeofday(&t->start, NULL);
}

void stopTiming(Timing *t, Player *p) {
    struct timeval end;
    gettimeofday(&end, NULL);
    double timeInterval = (double) (end.tv_sec - t->start.tv_sec) * 1000.0;      // sec to ms
    timeInterval += (end.tv_usec - t->start.tv_usec) / 1000.0;   // us to ms
    t->elapsedTime += timeInterval;
    if (t->elapsedTime > 1000.0) {
        t->elapsedTime -= 1000.0;
        p->secondsPlayed += 1;
    }
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        startTiming(g->timing);
        if (isFighting(g->currentScene)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->currentScene->exploration) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g->currentScene)) {
            doExplorationLoop(g);
        }
        updateMusicStream(g->audioManager);
        stopTiming(g->timing, g->player);
    }
}

void loadScenesFromFiles(Game *g) {
    SceneLoader *sl = createSceneLoader(g->runtimeArgs->indexDir);
    addDebug(g->log, "get scene directories :: %s", sl->sceneDirectory);
    sl->count = getFilesInDirectory(sl->sceneDirectory, sl->scenes);
    addDebug(g->log, "top level count :: %d", sl->count);
    buildSceneFilesList(sl);
    g->sceneCount = addSubsceneFiles(sl);
    for (int i = 0; i < g->sceneCount; i++) {
        addInfo(g->log, "found scene: %s, %s", sl->scenes[i], sl->sceneFiles[i]);
    }
    loadScenes(g, sl->scenes, sl->sceneFiles);
}

void loadAllAnimations(AnimationManager *am, SpritesheetManager *sm, const char *indexDir) {
    char animationsDir[MAX_FS_PATH_LENGTH / 2];
    sprintf(animationsDir, "%s/animations", indexDir);
    char *files[MAX_FILES];
    int count = getFilesInDirectory(animationsDir, files);
    for (int i = 0; i < count; i++) {
        if (strcmp(getFilenameExt(files[i]), "yaml") == 0) {
            char animationFile[MAX_FS_PATH_LENGTH];
            sprintf(animationFile, "%s/%s", animationsDir, files[i]);
            loadAnimations(am, sm, animationFile);
        }
    }
}

void initializeLog(Game *g) {
    g->log = createLog(g->runtimeArgs->logLevel);
    addInfo(g->log, "log level set to %s", getLogLevelString(g->log->level));
}

void initializeBeasts(Game *g) {
    g->beastiary = createBeastiary();
    loadBeastiary(g);
}

Scene *findScene(Game *g, const char *name) {
    for (int i = 0; i < g->sceneCount; i++) {
        if (strcmp(g->scenes[i]->name, name) == 0) {
            return g->scenes[i];
        }
    }
    return NULL;
}

const char *getAutosaveFile(const char *indexDir) {
    const char *autosaveFilePath = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *)autosaveFilePath, "%s/_saves/autosave.yaml", indexDir);
    return autosaveFilePath;
}

Player *mapSaveDataToPlayer(Game *g, SaveData *save) {
    Mobile *mobs[MAX_PARTY_SIZE];
    addInfo(g->log, "save file party count :: %d", save->party_count);
    for (int i = 0; i < save->party_count; i++) {
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(g->animationManager, save->party[i].animations, animations);
        mobs[i] = createMobile(
                save->party[i].id,
                save->party[i].name,
                getPositionFromString(save->party[i].position),
                getDirectionFromString(save->party[i].direction),
                animations);
    }
    for (int i = save->party_count; i < MAX_PARTY_SIZE; i++) {
        mobs[i] = NULL;
    }
    Player *player = createPlayer(g->log, mobs);
    player->secondsPlayed = save->secondsPlayed;
    player->coins = save->coins;
    return player;
}

SaveData *initializePlayer(Game *g) {
    const char *autosaveFilePath = getAutosaveFile(g->runtimeArgs->indexDir);
    SaveData *save = NULL;
    if (FileExists(autosaveFilePath)) {
        save = loadSaveData(autosaveFilePath);
        g->player = mapSaveDataToPlayer(g, save);
    } else {
        g->player = createNewPlayer(g->log, g->animationManager, g->runtimeArgs->indexDir);
    }
    free((char *)autosaveFilePath);
    return save;
}

void setSceneBasedOnSave(Game *g, SaveData *save) {
    if (save != NULL && g->runtimeArgs->sceneIndex == -1) {
        setScene(g, findScene(g, save->scene), NULL);
        return;
    }
    setScene(g, g->scenes[START_SCENE], START_ENTRANCE);
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->runtimeArgs = r;
    g->currentScene = NULL;
    initializeLog(g);
    g->timing = malloc(sizeof(Timing));
    g->spritesheetManager = loadSpritesheetManager(g->log, r->indexDir);
    g->animationManager = createAnimationManager(g->log);
    loadAllAnimations(g->animationManager, g->spritesheetManager, r->indexDir);
    g->audioManager = loadAudioManager(g->log, r->indexDir);
    initializeBeasts(g);
    SaveData *save = initializePlayer(g);
    loadScenesFromFiles(g);
    setSceneBasedOnSave(g, save);
    g->menuCount = getMenuList(g->menus);
    addDebug(g->log, "done creating game object");
    if (r->exit) {
        exit(0);
    }
    free(save);
    return g;
}
