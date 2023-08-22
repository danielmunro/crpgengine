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
    Timing *timing;
    NotificationManager *notifications;
    ControlManager *controls;
    MobileManager *mobiles;
    UIManager *ui;
    FightManager *fight2;
    Menu **menus;
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
        g->ui->menuContext = createMenuContext(
//            g->fight,
            g->player,
            g->ui->fonts,
            g->scenes->current->name,
            g->runtimeArgs->indexDir,
            0);
    addMenu(g->menus, findMenu(g->ui, PARTY_MENU));
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
    Menu *menu = getCurrentMenu(g->menus);
    MenuSelectResponse *response = menu->selected(g->ui->menuContext);
    if (response->type == OPEN_MENU) {
        addMenu(g->menus, findMenu(g->ui, response->menuType));
    } else if (response->type == CLOSE_MENU) {
        removeMenu(g->menus);
    }
    free(response);
}

void checkMenuInput(Game *g) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        int menuCount = removeMenu(g->menus);
        if (menuCount == 0) {
            free(g->ui->menuContext);
            g->ui->menuContext = NULL;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *menu = getCurrentMenu(g->menus);
        menu->cursor++;
        normalizeMenuCursor(menu, g->ui->menuContext);
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *menu = getCurrentMenu(g->menus);
        menu->cursor--;
        normalizeMenuCursor(menu, g->ui->menuContext);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        menuItemSelected(g);
    }
}

bool isExploring(Game *g) {
    return !isFighting(g->fight2) && !getCurrentMenu(g->menus);
}

bool canTriggerFight(Game *g, Player *p) {
    if (!isDungeon(g->scenes->current) || isFighting(g->fight2) || !isMoving(getPartyLeader(p))) {
        return false;
    }
    return true;
}

void checkFights(Game *g, Scene *s) {
    addDebug(s->log, "exploration -- check for fight");
    if (!canTriggerFight(g, g->player)) {
        return;
    }
    if (rand() % 100 + 1 == 1) {
        createFightFromEncounters(
                g->fight2,
                g->log,
                s->encounters,
                g->player,
                findSpritesheetByName(g->sprites, SPRITESHEET_NAME_UI));
        Animation *animation = findAnimation(getPartyLeader(g->player)->animations, LEFT);
        animation->currentFrame = animation->firstFrame;
        g->ui->menuContext = createMenuContext(
//                g->fight,
                g->player,
                g->ui->fonts,
                NULL,
                NULL,
                -1
//                g->fight->cursors[g->fight->menu]
        );
    }
}

void doExplorationLoop(Game *g) {
    Scene *s = g->scenes->current;
    checkExplorationInput(g);
    drawExplorationView(
            s->exploration,
            g->player,
            g->notifications,
            s->activeControlBlocks,
            getFontStyle(g->ui->fonts, FONT_STYLE_DEFAULT));
    doMobileMovementUpdates(s->exploration);
    processAnimations(g->animations);
    evaluateMovement(s->exploration, g->player);
    evaluateExits(g);
    checkControls(g->controls);
    checkFights(g, s);
}

void doFightLoop(Game *g) {
    Scene *s = g->scenes->current;
    fightUpdate(g->fight2->fight);
    checkFightInput(g->fight2->fight);
    drawFightView(s->encounters, g->fight2->fight, g->ui);
    processFightAnimations();
    checkControls(g->controls);
    checkRemoveFight(g->fight2);
}

void doInGameMenuLoop(Game *g) {
    drawAllMenus(
            g->ui->menuContext,
            g->menus);
    checkMenuInput(g);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        startTiming(g->timing);
        if (isFighting(g->fight2)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->menus) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g)) {
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
    g->log = createLog(g->runtimeArgs->logLevel);
    g->sprites = loadSpritesheetManager(g->log, r->indexDir);
    g->ui = createUIManager(g->log, g->sprites, r->indexDir, cfg->font);
    g->animations = createAnimationManager(g->log);
    loadAllAnimations(g->animations, g->sprites, r->indexDir);
    g->audio = loadAudioManager(g->log, r->indexDir);
    g->beastiary = loadBeastiary(g->log, g->runtimeArgs->indexDir);
    g->items = createItemManager();
    loadAllItems(g->items, r->indexDir);
    g->mobiles = createMobileManager(g->log, g->animations);
    SaveData *save = initializePlayer(g);
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
    g->menus = calloc(MAX_MENUS, sizeof(Menu));
    g->fight2 = createFightManager(g->log);
    return g;
}
