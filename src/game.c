typedef struct {
    Scene *scenes[MAX_SCENES];
    int sceneCount;
    Scene *currentScene;
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
    AudioManager *audioManager;
    int animIndex;
    Beastiary *beastiary;
    Log *log;
    Menu *menus[MAX_MENUS];
    int menuCount;
} Game;

void addAnimation(Game *g, Animation *a) {
    g->animations[g->animIndex] = a;
    g->animIndex++;
}

void addAllAnimations(Game *g, Animation *animations[MAX_ANIMATIONS]) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == NULL) {
            break;
        }
        addAnimation(g, animations[i]);
    }
}

void clearAnimations(Game *g) {
    memset(g->animations, 0, sizeof(g->animations));
    g->animIndex = 0;
}

void setScene(Game *g, Scene *scene, char *entranceName) {
    addDebug(g->log, "setting scene to '%s'", scene->name);
    g->currentScene = scene;
    clearAnimations(g);
    Mobile *mob = getPartyLeader(g->player);
    addAllAnimations(g, mob->animations);
    Entrance *entrance = findEntrance(scene->exploration, entranceName);
    mob->position.x = entrance->area.x + (entrance->area.width / 2);
    mob->position.y = entrance->area.y + (entrance->area.height / 2);
    mob->direction = entrance->direction;
    renderExplorationLayers(g->currentScene->exploration);
    playMusic(g->audioManager, g->currentScene->music);
    addDebug(g->log, "finished setting scene to '%s'", g->currentScene->name);
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

ControlBlock *mapStorylineToControlBlock(Game *g, StorylineData *storyline) {
    ControlBlock *c = createControlBlock();
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    addDebug(g->log, "processing storyline with %d when and %d then clauses",
             storyline->when_count, storyline->then_count);
    Mobile *mob = getPartyLeader(g->player);
    for (int i = 0; i < storyline->when_count; i++) {
        WhenData wd = storyline->when[i];
        Mobile *trigger = NULL;
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
        c->when[i] = w;
    }
    addDebug(g->log, "done processing when conditions");
    for (int i = 0; i < storyline->then_count; i++) {
        ThenData td = storyline->then[i];
        Mobile *target;
        if (td.player) {
            target = mob;
        } else {
            target = findMobById(g, td.mob);
        }
        Then *t = createThen(
                target,
                &td.message[0],
                &td.story[0],
                mapOutcome(td.action));
        addDebug(g->log, "then story is '%s', outcome: %d, message: %s",
                 t->story, t->outcome, t->message);
        c->then[i] = t;
    }
    return c;
}

void loadScenes(Game *g, RuntimeArgs *r, char *scenes[MAX_SCENES]) {
    for (int i = 0; i < g->sceneCount; i++) {
        g->scenes[i] = loadScene(g->log, g->beastiary, r->indexDir, scenes[i], r->showCollisions);
        addDebug(g->log, "scene %s (%d) loaded", g->scenes[i]->name, i);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        for (int c = 0; c < g->scenes[i]->storylineCount; c++) {
            g->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(g, g->scenes[i]->storylines[c]);
            g->scenes[i]->controlBlockCount++;
        }
    }
}

void loadBeastiary(Game *g, const char *indexDir) {
    char filePath[255];
    sprintf(filePath, "%s/beastiary.yaml", indexDir);
    BeastiaryData *data = loadBeastiaryYaml(filePath);
    for (int i = 0; i < data->beasts_count; i++) {
        g->beastiary->beasts[i] = createBeastFromData(indexDir, &data->beasts[i]);
        addDebug(g->log, "beast '%s' created", g->beastiary->beasts[i]->id);
        g->beastiary->beastCount++;
    }
}

void processExplorationAnimations(Game *g) {
    addDebug(g->log, "exploration -- process animations");
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i] != NULL && g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
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
                Entrance *entrance = findEntrance(g->scenes[i]->exploration, entranceName);
                if (entrance != NULL) {
                    addDebug(g->log, "entrance %s found at %f, %f, %f, %f", entranceName, entrance->area.x,
                             entrance->area.y, entrance->area.width, entrance->area.height);
                    getPartyLeader(g->player)->position = (Vector2) {
                            entrance->area.x,
                            entrance->area.y
                    };
                    setScene(g, g->scenes[i], entranceName);
                }
                return;
            }
        }
        addError(g->log, "warp to '%s' not found", sceneName);
    }
}

void loadMenus(Game *g) {
    Menu *menus[MAX_MENUS] = {
            createMenu(
                    PARTY_MENU,
                    getPartyMenuCursorLength,
                    drawPartyMenuScreen,
                    partyMenuItemSelected),
            createMenu(
                    ITEMS_MENU,
                    getItemsCursorLength,
                    drawItemsMenuScreen,
                    partyMenuItemSelected),
            createMenu(
                    QUIT_MENU,
                    getQuitCursorLength,
                    drawQuitMenuScreen,
                    quitMenuItemSelected),
    };
    g->menuCount = 3;
    for (int i = 0; i < g->menuCount; i++) {
        g->menus[i] = menus[i];
    }
}

void removeMenu(Exploration *exploration) {
    exploration->menuCount--;
    exploration->menus[exploration->menuCount]->cursor = 0;
    exploration->menus[exploration->menuCount] = NULL;
}

void explorationMenuKeyPressed(Game *g) {
    addMenu(g->currentScene->exploration, findMenu(g->menus, g->menuCount, PARTY_MENU));
}

void explorationSpaceKeyPressed(Exploration *exploration, Player *player, ControlBlock *controlBlock) {
    addInfo(exploration->log, "space key pressed");
    if (controlBlock != NULL) {
        addDebug(exploration->log, "active control block progress: %d", controlBlock->progress);
    } else {
        addDebug(exploration->log, "no active control blocks set");
    }
    if (player->engaged && controlBlock == NULL) {
        player->engaged = false;
        return;
    }
    if (player->engaged && controlBlock->then[controlBlock->progress]->outcome == SPEAK) {
        if (controlBlock != NULL) {
            controlBlock->progress++;
            addDebug(exploration->log, "active control block progress at %d", controlBlock->progress);
        }
        if (controlBlock->progress >= controlBlock->thenCount
            || controlBlock->then[controlBlock->progress]->outcome != SPEAK) {
            addDebug(exploration->log, "unset engaged");
            player->engaged = false;
        }
        if (controlBlock->progress >= controlBlock->thenCount) {
            addDebug(exploration->log, "unsetting active control block");
            controlBlock->progress = 0;
            controlBlock = NULL;
        }
    } else if (player->blockedBy != NULL) {
        player->engageable = player->blockedBy;
        addInfo(exploration->log, "engaging with %s", player->engageable->name);
        player->engaged = true;
    }
}

void explorationCheckMoveKeys(Player *player) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        checkMoveKey(player, MOVE_KEYS[i], DIRECTIONS[i]);
    }
}

void checkExplorationInput(Game *g) {
    addDebug(g->log, "exploration -- check player input");
    resetMoving(g->player);
    Mobile *mob = getPartyLeader(g->player);
    getMobAnimation(mob)->isPlaying = 0;
    explorationCheckMoveKeys(g->player);
    if (IsKeyDown(KEY_C)) {
        explorationDebugKeyPressed(g->currentScene->exploration, mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        explorationSpaceKeyPressed(g->currentScene->exploration, g->player, g->currentScene->activeControlBlock);
    }
    if (IsKeyPressed(KEY_M)) {
        explorationMenuKeyPressed(g);
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
    drawExplorationView(g->currentScene->exploration, g->player, g->currentScene->activeControlBlock);
    processExplorationAnimations(g);
    evaluateMovement(g->currentScene->exploration, g->player);
    evaluateExits(g);
    checkControls(g->currentScene, g->player);
    checkFights(g->currentScene, g->player);
    updateMusicStream(g->audioManager);
}

void doFightLoop(Game *g) {
    checkFightInput(g->currentScene->fight, g->player);
    drawFightView(g->currentScene->encounters, g->currentScene->fight, g->player);
    processFightAnimations();
    checkControls(g->currentScene, g->player);
    checkRemoveFight(g->currentScene);
    updateMusicStream(g->audioManager);
}

void doInGameMenuLoop(Game *g) {
    Exploration *exploration = g->currentScene->exploration;
    drawAllMenus(g->player, exploration->menus, exploration->menuCount);
    checkMenuInput(g);
    updateMusicStream(g->audioManager);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        if (isFighting(g->currentScene)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->currentScene->exploration) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g->currentScene)) {
            doExplorationLoop(g);
        }
    }
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    g->log = createLog(r->logLevel);
    g->audioManager = loadAudioManager(g->log, r->indexDir);
    g->player = loadPlayer(g->log, r->indexDir);
    addInfo(g->log, "log level set to %s", getLogLevelString(g->log->level));
    g->beastiary = createBeastiary();
    char *scenes[MAX_SCENES];
    char sceneDir[255];
    sprintf(sceneDir, "%s/scenes", r->indexDir);
    g->sceneCount = getFilesInDirectory(sceneDir, scenes);
    loadBeastiary(g, r->indexDir);
    loadScenes(g, r, scenes);
    setScene(g, g->scenes[r->sceneIndex], "start");
    loadMenus(g);
    addDebug(g->log, "done creating game object");
    return g;
}
