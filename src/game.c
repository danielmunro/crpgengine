typedef struct {
    RuntimeArgs *runtimeArgs;
    SceneManager *scenes;
    Player *player;
    AnimationManager *animations;
    AudioManager *audio;
    SpritesheetManager *sprites;
    Beastiary *beastiary;
    ItemManager *items;
    Log *log;
    Menu *menus[MAX_MENUS];
    int menuCount;
    Timing *timing;
    NotificationManager *notifications;
    ControlManager *controls;
    MobileManager *mobiles;
    UIManager *ui;
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
    setScene(game->scenes, scene, game->player, entrance->name);
}

void evaluateExits(Game *g) {
    addDebug(g->log, "exploration -- evaluate exits");
    Exploration *e = g->scenes->current->exploration;
    int exit = atExit(e, g->player);
    if (exit > -1) {
        addDebug(g->log, "player at exit");
        char *sceneName = e->exits[exit]->scene;
        char *entranceName = e->exits[exit]->to;
        for (int i = 0; i < g->scenes->count; i++) {
            if (strcmp(sceneName, g->scenes->scenes[i]->name) == 0) {
                attemptToUseExit(
                        g,
                        g->scenes->scenes[i],
                        findEntrance(g->scenes->scenes[i]->exploration, entranceName)
                );
                return;
            }
        }
        addError(g->log, "warp to '%s' not found", sceneName);
    }
}

void explorationMenuKeyPressed(Game *g) {
    addMenu(g->scenes->current->exploration, findMenu(g->menus, g->menuCount, PARTY_MENU));
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
        explorationDebugKeyPressed(g->scenes->current->exploration, mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        explorationSpaceKeyPressed(g->scenes->current->exploration,
                                   g->player,
                                   g->scenes->current->activeControlBlocks);
    }
    if (IsKeyPressed(KEY_M)) {
        explorationMenuKeyPressed(g);
    }
    if (IsKeyPressed(KEY_T)) {
        addInfo(g->log, "player play time :: %ds", g->player->secondsPlayed);
    }
    if (IsKeyPressed(KEY_S)) {
        save(g->player, g->scenes->current->name, g->runtimeArgs->indexDir);
    }
}

void menuItemSelected(Game *g) {
    Exploration *exploration = g->scenes->current->exploration;
    Menu *menu = getCurrentMenu(exploration);
    MenuContext *context = createMenuContext(
            g->player,
            g->scenes->current->name,
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
    Exploration *exploration = g->scenes->current->exploration;
    if (IsKeyPressed(KEY_ESCAPE)) {
        removeMenu(exploration);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *menu = getCurrentMenu(exploration);
        menu->cursor++;
        MenuContext *c = createMenuContext(
                g->player,
                g->scenes->current->name,
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
                g->scenes->current->name,
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
    Scene *s = g->scenes->current;
    checkExplorationInput(g);
    drawExplorationView(
            s->exploration,
            g->player,
            g->notifications,
            s->activeControlBlocks);
    doMobileMovementUpdates(s->exploration);
    processAnimations(g->animations);
    evaluateMovement(s->exploration, g->player);
    evaluateExits(g);
    checkControls(g->controls);
    checkFights(s, g->player);
}

void doFightLoop(Game *g) {
    Scene *s = g->scenes->current;
    checkFightInput(s->fight, g->player);
    drawFightView(s->encounters, s->fight, g->player);
    processFightAnimations();
    checkControls(g->controls);
    checkRemoveFight(s);
}

void doInGameMenuLoop(Game *g) {
    Exploration *exploration = g->scenes->current->exploration;
    drawAllMenus(
            g->player,
            exploration->menus,
            exploration->menuCount,
            g->scenes->current->name,
            g->runtimeArgs->indexDir);
    checkMenuInput(g);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        startTiming(g->timing);
        if (isFighting(g->scenes->current)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->scenes->current->exploration) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g->scenes->current)) {
            doExplorationLoop(g);
        }
        updateMusicStream(g->audio);
        stopTiming(g->timing);
    }
}

SaveData *initializePlayer(Game *g) {
    RuntimeArgs *r = g->runtimeArgs;
    char saveFilePath[MAX_FS_PATH_LENGTH];
    if (r->saveFile != NULL) {
        sprintf((char *)saveFilePath, "%s/_saves/%s", r->indexDir, r->saveFile);
    } else {
        strcpy(saveFilePath, getAutosaveFile(r->indexDir));
    }
    SaveData *save = NULL;
    if (FileExists(saveFilePath) && !r->forceNewGame) {
        save = loadSaveData(saveFilePath);
        g->player = mapSaveDataToPlayer(g->animations, g->log, save);
    } else {
        g->player = createNewPlayer(g->log, g->animations, r->indexDir);
        addItem(g->player, g->items->items[0]);
    }
    g->player->saveFiles = getSaveFiles(r->indexDir);
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (g->player->party[i] == NULL) {
            break;
        }
        addMobileToManager(g->mobiles, g->player->party[i]);
    }
    return save;
}

Game *createGame(ConfigData *cfg, RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->runtimeArgs = r;
    g->ui = createUIManager(r->indexDir, cfg->font);
    g->log = createLog(g->runtimeArgs->logLevel);
    g->sprites = loadSpritesheetManager(g->log, r->indexDir);
    g->animations = createAnimationManager(g->log);
    loadAllAnimations(g->animations, g->sprites, r->indexDir);
    g->audio = loadAudioManager(g->log, r->indexDir);
    g->beastiary = loadBeastiary(g->log, g->runtimeArgs->indexDir);
    g->items = createItemManager();
    loadAllItems(g->items, r->indexDir);
    g->mobiles = createMobileManager(g->log, g->animations);
    SaveData *save = initializePlayer(g);
    g->menuCount = getMenuList(g->menus);
    g->notifications = createNotificationManager();
    g->timing = createTiming(g->log, g->notifications, g->player, g->runtimeArgs->logMemory);
    g->controls = createControlManager(
            g->log,
            g->player,
            g->runtimeArgs,
            g->items,
            g->notifications,
            g->mobiles);
    g->scenes = createSceneManager(g->log, g->controls,
                                   g->animations, g->audio);
    loadScenesFromFiles(g->scenes, g->mobiles, g->beastiary, g->runtimeArgs);
    setSceneBasedOnSave(g->scenes, g->player, save, g->runtimeArgs->sceneIndex);
    addDebug(g->log, "done creating game object");
    free(save);
    return g;
}
