typedef struct {
    RuntimeArgs *runtimeArgs;
    SceneManager *sceneManager;
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
    ControlManager *controlManager;
    MobileManager *mobileManager;
} Game;

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
    setScene(game->sceneManager, scene, game->player, entrance->name);
}

void evaluateExits(Game *g) {
    addDebug(g->log, "exploration -- evaluate exits");
    Exploration *e = g->sceneManager->currentScene->exploration;
    int exit = atExit(e, g->player);
    if (exit > -1) {
        addDebug(g->log, "player at exit");
        char *sceneName = e->exits[exit]->scene;
        char *entranceName = e->exits[exit]->to;
        for (int i = 0; i < g->sceneManager->count; i++) {
            if (strcmp(sceneName, g->sceneManager->scenes[i]->name) == 0) {
                attemptToUseExit(
                        g,
                        g->sceneManager->scenes[i],
                        findEntrance(g->sceneManager->scenes[i]->exploration, entranceName)
                );
                return;
            }
        }
        addError(g->log, "warp to '%s' not found", sceneName);
    }
}

void explorationMenuKeyPressed(Game *g) {
    addMenu(g->sceneManager->currentScene->exploration, findMenu(g->menus, g->menuCount, PARTY_MENU));
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
        explorationDebugKeyPressed(g->sceneManager->currentScene->exploration, mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        explorationSpaceKeyPressed(g->sceneManager->currentScene->exploration,
                                   g->player,
                                   g->sceneManager->currentScene->activeControlBlocks);
    }
    if (IsKeyPressed(KEY_M)) {
        explorationMenuKeyPressed(g);
    }
    if (IsKeyPressed(KEY_T)) {
        addInfo(g->log, "player play time :: %ds", g->player->secondsPlayed);
    }
    if (IsKeyPressed(KEY_S)) {
        save(g->player, g->sceneManager->currentScene->name, g->runtimeArgs->indexDir);
    }
}

void menuItemSelected(Game *g) {
    Exploration *exploration = g->sceneManager->currentScene->exploration;
    Menu *menu = getCurrentMenu(exploration);
    MenuContext *context = createMenuContext(
            g->player,
            g->sceneManager->currentScene->name,
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
    Exploration *exploration = g->sceneManager->currentScene->exploration;
    if (IsKeyPressed(KEY_ESCAPE)) {
        removeMenu(exploration);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *menu = getCurrentMenu(exploration);
        menu->cursor++;
        MenuContext *c = createMenuContext(
                g->player,
                g->sceneManager->currentScene->name,
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
                g->sceneManager->currentScene->name,
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
            g->sceneManager->currentScene->exploration,
            g->player,
            g->notificationManager,
            g->sceneManager->currentScene->activeControlBlocks);
    doMobileMovementUpdates(g->sceneManager->currentScene->exploration);
    processAnimations(g->animationManager);
    evaluateMovement(g->sceneManager->currentScene->exploration, g->player);
    evaluateExits(g);
    checkControls(g->controlManager);
    checkFights(g->sceneManager->currentScene, g->player);
}

void doFightLoop(Game *g) {
    checkFightInput(g->sceneManager->currentScene->fight, g->player);
    drawFightView(g->sceneManager->currentScene->encounters, g->sceneManager->currentScene->fight, g->player);
    processFightAnimations();
    checkControls(g->controlManager);
    checkRemoveFight(g->sceneManager->currentScene);
}

void doInGameMenuLoop(Game *g) {
    Exploration *exploration = g->sceneManager->currentScene->exploration;
    drawAllMenus(
            g->player,
            exploration->menus,
            exploration->menuCount,
            g->sceneManager->currentScene->name,
            g->runtimeArgs->indexDir);
    checkMenuInput(g);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        startTiming(g->timing);
        if (isFighting(g->sceneManager->currentScene)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->sceneManager->currentScene->exploration) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g->sceneManager->currentScene)) {
            doExplorationLoop(g);
        }
        updateMusicStream(g->audioManager);
        stopTiming(g->timing);
    }
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
        g->player = mapSaveDataToPlayer(g->animationManager, g->log, save);
    } else {
        g->player = createNewPlayer(g->log, g->animationManager, g->runtimeArgs->indexDir);
        addItem(g->player, g->itemManager->items[0]);
    }
    g->player->saveFiles = getSaveFiles(g->runtimeArgs->indexDir);
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (g->player->party[i] == NULL) {
            break;
        }
        addMobileToManager(g->mobileManager, g->player->party[i]);
    }
    return save;
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->runtimeArgs = r;
    g->log = createLog(g->runtimeArgs->logLevel);
    g->spritesheetManager = loadSpritesheetManager(g->log, r->indexDir);
    g->animationManager = createAnimationManager(g->log);
    loadAllAnimations(g->animationManager, g->spritesheetManager, r->indexDir);
    g->audioManager = loadAudioManager(g->log, r->indexDir);
    g->beastiary = loadBeastiary(g->log, g->runtimeArgs->indexDir);
    g->itemManager = createItemManager();
    loadAllItems(g->itemManager, r->indexDir);
    g->mobileManager = createMobileManager(g->log, g->animationManager);
    SaveData *save = initializePlayer(g);
    g->menuCount = getMenuList(g->menus);
    g->notificationManager = createNotificationManager();
    g->timing = createTiming(g->log, g->notificationManager, g->player, g->runtimeArgs->logMemory);
    g->controlManager = createControlManager(
            g->log,
            g->player,
            g->runtimeArgs,
            g->itemManager,
            g->notificationManager,
            g->mobileManager);
    g->sceneManager = createSceneManager(g->log, g->controlManager,
                                         g->animationManager, g->audioManager);
    loadScenesFromFiles(g->sceneManager, g->mobileManager, g->beastiary, g->runtimeArgs);
    setSceneBasedOnSave(g->sceneManager, g->player, save, g->runtimeArgs->sceneIndex);
    g->controlManager->scene = g->sceneManager->currentScene;
    addDebug(g->log, "done creating game object");
    free(save);
    return g;
}
