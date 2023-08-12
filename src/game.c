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
    ItemManager *itemManager;
    Log *log;
    Menu *menus[MAX_MENUS];
    int menuCount;
    Timing *timing;
    NotificationManager *notificationManager;
} Game;

int thenCheck(Game *g, ControlBlock *cb) {
    Then *then = cb->then[cb->progress];
    int progress = 0;
    if (isMovingAndAtDestination(cb)) {
        addInfo(g->log, "mob at destination, control block proceeding :: %s", then->target->name);
        progress++;
    } else if (isAddStoryOutcome(then)) {
        addStory(g->player, then->story);
        progress++;
    } else if (needsToStartMoving(then)) {
        addInfo(g->log, "mob needs to start moving");
        addMobileMovement(
                g->currentScene->exploration,
                createMobileMovement(
                        then->target,
                        then->position
                )
        );
        g->player->engaged = false;
        getPartyLeader(g->player)->isBeingMoved = true;
    } else if (isFaceDirectionOutcome(then)) {
        addInfo(g->log, "set direction for mob :: %s, %s", then->target->name, then->direction);
        then->target->direction =
                getDirectionFromString(then->direction);
        progress++;
    } else if (needsToChangePosition(then)) {
        Mobile *target = then->target;
        target->position = then->position;
        addInfo(g->log, "change position for mob :: %s, %f, %f",
                target->name, target->position.x, target->position.y);
        progress++;
    } else if (needsToWait(then)) {
        Mobile *target = then->target;
        if (target->waitTimer == -1) {
            addInfo(g->log, "setting initial wait timer");
            target->waitTimer = then->amount;
        }
        struct timeval update;
        gettimeofday(&update, NULL);
        if (update.tv_sec > target->lastTimerUpdate.tv_sec) {
            target->lastTimerUpdate = update;
            target->waitTimer--;
            if (target->waitTimer == 0) {
                addInfo(g->log, "timer done");
                target->waitTimer = -1;
                progress++;
            }
        }
    } else if (needsToLock(then)) {
        Mobile *mob = getPartyLeader(g->player);
        mob->locked = true;
        resetMoving(mob);
        progress++;
    } else if (needsToUnlock(then)) {
        Mobile *mob = getPartyLeader(g->player);
        mob->locked = false;
        resetMoving(mob);
        progress++;
    } else if (needsToSave(then)) {
        save(g->player, g->currentScene->name, g->runtimeArgs->indexDir);
        progress++;
    } else if (needsToReceiveItem(then, getPartyLeader(g->player))) {
        addInfo(g->log, "player receiving item: %s", then->item);
        addItem(g->player, findItem(g->itemManager, then->item));
        const char *message = malloc(64);
        sprintf((char *)message, "you received:\n%s", then->item);
        addNotification(g->notificationManager, createNotification(RECEIVE_QUEST_ITEM, message));
        progress++;
    }
    cb->progress += progress;
    return progress;
}

int controlThenCheckAllActive(Game *g) {
    int progress = 0;
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (g->currentScene->activeControlBlocks[i] != NULL) {
            progress += thenCheck(g, g->currentScene->activeControlBlocks[i]);
        }
    }
    return progress;
}

void checkControls(Game *g) {
    addDebug(g->log, "exploration -- check %d control blocks",
             g->currentScene->controlBlockCount);
    controlWhenCheck(g->currentScene, g->player, EVENT_GAME_LOOP);
    controlThenCheckAllActive(g);
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (g->currentScene->activeControlBlocks[i] != NULL &&
            needsToRemoveActiveControlBlock(g->currentScene->activeControlBlocks[i])) {
            g->currentScene->activeControlBlocks[i]->progress = 0;
            g->currentScene->activeControlBlocks[i] = NULL;
        }
    }
}

void proceedControlsUntilDone(Game *g) {
    controlWhenCheck(g->currentScene, g->player, EVENT_SCENE_LOADED);
    bool isMakingProgress = true;
    while (isMakingProgress) {
        isMakingProgress = controlThenCheckAllActive(g) > 0;
        checkControls(g);
    }
}

double reportMaxMemory(Log *log) {
    int who = RUSAGE_SELF;
    struct rusage usage;
    getrusage(who, &usage);
    double memoryInMb = (double) usage.ru_maxrss / 1000000;
    addInfo(log, "max memory: %f\n", memoryInMb);
    return memoryInMb;
}

void setScene(Game *g, Scene *scene, char *entranceName) {
    addInfo(g->log, "setting scene to '%s'", scene->name);
    if (g->currentScene != NULL) {
        unloadLayers(g->currentScene->exploration);
    }
    g->currentScene = scene;
    clearAnimations(g->animationManager);
    Mobile *mob = getPartyLeader(g->player);
    addAllAnimations(g->animationManager, mob->animations);
    if (entranceName != NULL) {
        Entrance *entrance = findEntrance(scene->exploration, entranceName);
        addDebug(g->log, "set position from scene entrance :: %s", entrance->name);
        useEntrance(mob, entrance);
    }
    addDebug(g->log, "player position :: %f %f", mob->position.x, mob->position.y);
    renderExplorationLayers(g->currentScene->exploration);
    playMusic(g->audioManager, g->currentScene->music);
    proceedControlsUntilDone(g);
}

Mobile *findMobById(Game *g, const char *id) {
    for (int s = 0; s < g->sceneCount; s++) {
        for (int m = 0; m < g->scenes[s]->exploration->mobileCount; m++) {
            if (strcmp(g->scenes[s]->exploration->mobiles[m]->id, id) == 0) {
                return g->scenes[s]->exploration->mobiles[m];
            }
        }
    }
    addError(g->log, "mob not found: %s", id);
    exit(EXIT_MOBILE_NOT_FOUND);
}

When *mapWhen(Game *g, Scene *s, WhenData wd) {
    Mobile *trigger = NULL;
    Mobile *mob = getPartyLeader(g->player);
    ArriveAt *arriveAt = NULL;
    if (wd.mob != NULL) {
        trigger = findMobById(g, wd.mob);
        addDebug(g->log, "mobile trigger is '%s'", trigger->name);
    }
    if (wd.arriveAt != NULL) {
        Exploration *e = s->exploration;
        for (int i = 0; i < e->arriveAtCount; i++) {
            if (strcmp(e->arriveAt[i]->name, wd.arriveAt) == 0) {
                arriveAt = e->arriveAt[i];
                break;
            }
        }
    }
    Condition c = mapCondition(wd.condition);
    addDebug(g->log, "condition: %s, mapped to: %d, story: %s",
             wd.condition,
             c,
             wd.story);
    return createWhen(
            mob,
            trigger,
            c,
            wd.story,
            arriveAt);
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
    int amount = 0;
    if (hasAmountProperty(td)) {
        amount = td.amount;
    }
    Outcome o = mapOutcome(td.action);
    addDebug(g->log, "then story is '%s', outcome: %d, message: %s",
             td.story, o, td.message);
    return createThen(
            target,
            &td.message[0],
            &td.story[0],
            &td.direction[0],
            &td.item[0],
            o,
            pos,
            td.parallel,
            amount
    );
}

ControlBlock *mapStorylineToControlBlock(Game *g, Scene *scene, StorylineData *storyline) {
    ControlBlock *c = createControlBlock();
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    addDebug(g->log, "processing storyline with %d when and %d then clauses",
             storyline->when_count, storyline->then_count);
    for (int i = 0; i < storyline->when_count; i++) {
        c->when[i] = mapWhen(g, scene, storyline->when[i]);
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
            g->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(g, g->scenes[i], g->scenes[i]->storylines[c]);
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
    free(data);
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
    Mobile *mob = getPartyLeader(g->player);
    addDebug(g->log, "exploration -- check player input");
    resetMoving(mob);
    if (!canPlayerMove(mob)) {
        return;
    }
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
        save(g->player, g->currentScene->name, g->runtimeArgs->indexDir);
    }
}

void menuItemSelected(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    Menu *menu = getCurrentMenu(exploration);
    MenuContext *context = createMenuContext(
            g->player,
            g->currentScene->name,
            g->runtimeArgs->indexDir,
            menu->cursor);
    MenuSelectResponse *response = menu->selected(context);
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
        MenuContext *c = createMenuContext(
                g->player,
                g->currentScene->name,
                g->runtimeArgs->indexDir,
                menu->cursor);
        normalizeMenuCursor(menu, c);
        free(c);
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *menu = getCurrentMenu(exploration);
        menu->cursor--;
        MenuContext *c = createMenuContext(
                g->player,
                g->currentScene->name,
                g->runtimeArgs->indexDir,
                menu->cursor);
        normalizeMenuCursor(menu, c);
        free(c);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        menuItemSelected(g);
    }
}

void doExplorationLoop(Game *g) {
    checkExplorationInput(g);
    drawExplorationView(
            g->currentScene->exploration,
            g->player,
            g->notificationManager,
            g->currentScene->activeControlBlocks);
    doMobileMovementUpdates(g->currentScene->exploration);
    processAnimations(g->animationManager);
    evaluateMovement(g->currentScene->exploration, g->player);
    evaluateExits(g);
    checkControls(g);
    checkFights(g->currentScene, g->player);
}

void doFightLoop(Game *g) {
    checkFightInput(g->currentScene->fight, g->player);
    drawFightView(g->currentScene->encounters, g->currentScene->fight, g->player);
    processFightAnimations();
    checkControls(g);
    checkRemoveFight(g->currentScene);
}

void doInGameMenuLoop(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    drawAllMenus(
            g->player,
            exploration->menus,
            exploration->menuCount,
            g->currentScene->name,
            g->runtimeArgs->indexDir);
    checkMenuInput(g);
}

void startTiming(Timing *t) {
    gettimeofday(&t->start, NULL);
}

void stopTiming(Game *g) {
    struct timeval end;
    gettimeofday(&end, NULL);
    double timeInterval = (double) (end.tv_sec - g->timing->start.tv_sec) * 1000.0;
    timeInterval += (end.tv_usec - g->timing->start.tv_usec) / 1000.0;
    g->timing->elapsedTime += timeInterval;
    if (g->timing->elapsedTime > 1000.0) {
        g->timing->elapsedTime -= 1000.0;
        g->player->secondsPlayed += 1;
        if (g->runtimeArgs->logMemory) {
            reportMaxMemory(g->log);
        }
    }
    decayNotifications(g->notificationManager, timeInterval);
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
        stopTiming(g);
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
    free(sl);
}

void loadAllAnimations(AnimationManager *am, SpritesheetManager *sm, const char *indexDir) {
    char animationsDir[MAX_FS_PATH_LENGTH / 2];
    sprintf(animationsDir, "%s/animations", indexDir);
    char **files = calloc(MAX_FILES, sizeof(char *));
    int count = getFilesInDirectory(animationsDir, files);
    for (int i = 0; i < count; i++) {
        if (strcmp(getFilenameExt(files[i]), "yaml") == 0) {
            char animationFile[MAX_FS_PATH_LENGTH];
            sprintf(animationFile, "%s/%s", animationsDir, files[i]);
            loadAnimations(am, sm, animationFile);
        }
    }
    free(files);
}

void loadAllItems(ItemManager *itemManager, const char *indexDir) {
    const char *itemsFile = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *)itemsFile, "%s/items.yaml", indexDir);
    ItemsData *itemsData = loadItemYaml(itemsFile);
    itemManager->items = calloc(itemsData->items_count, sizeof(ItemData));
    for (int i = 0; i < itemsData->items_count; i++) {
        itemManager->items[i] = &itemsData->items[i];
    }
    itemManager->count = itemsData->items_count;
    free((char *)itemsFile);
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
    addInfo(g->log, "save file party count :: %d", save->player->party_count);
    for (int i = 0; i < save->player->party_count; i++) {
        Animation *animations[MAX_ANIMATIONS];
        loadAnimationsByName(g->animationManager, save->player->party[i].animations, animations);
        mobs[i] = createMobile(
                save->player->party[i].id,
                save->player->party[i].name,
                getPositionFromString(save->player->party[i].position),
                getDirectionFromString(save->player->party[i].direction),
                animations);
        mobs[i]->attributes = createAttributesFromData(save->player->party[i].attributes);
    }
    for (int i = save->player->party_count; i < MAX_PARTY_SIZE; i++) {
        mobs[i] = NULL;
    }
    Player *p = createPlayer(
            g->log,
            mobs,
            save->player->coins,
            save->player->experience,
            save->player->level,
            save->player->secondsPlayed,
            save->player->storylines,
            save->player->storylines_count,
            save->player->items,
            save->player->items_count);
    return p;
}

SaveData *initializePlayer(Game *g) {
    char saveFilePath[MAX_FS_PATH_LENGTH];
    if (g->runtimeArgs->saveFile != NULL) {
        sprintf((char *)saveFilePath, "%s/_saves/%s", g->runtimeArgs->indexDir, g->runtimeArgs->saveFile);
    } else {
        strcpy(saveFilePath, getAutosaveFile(g->runtimeArgs->indexDir));
    }
    SaveData *save = NULL;
    if (FileExists(saveFilePath) && !g->runtimeArgs->forceNewGame) {
        save = loadSaveData(saveFilePath);
        g->player = mapSaveDataToPlayer(g, save);
    } else {
        g->player = createNewPlayer(g->log, g->animationManager, g->runtimeArgs->indexDir);
        addItem(g->player, g->itemManager->items[0]);
    }
    g->player->saveFiles = getSaveFiles(g->runtimeArgs->indexDir);
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
    g->itemManager = createItemManager();
    loadAllItems(g->itemManager, r->indexDir);
    SaveData *save = initializePlayer(g);
    loadScenesFromFiles(g);
    setSceneBasedOnSave(g, save);
    g->menuCount = getMenuList(g->menus);
    g->notificationManager = createNotificationManager();
    addDebug(g->log, "done creating game object");
    free(save);
    return g;
}
