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

void setScene(Game *g, Scene *scene) {
    addDebug(g->log, "setting scene to '%s'", scene->name);
    g->currentScene = scene;
    clearAnimations(g);
    addAllAnimations(g, g->player->mob->animations);
    Rectangle r = g->currentScene->exploration->entrance;
    g->player->mob->position.x = r.x + (r.width / 2);
    g->player->mob->position.y = r.y + (r.height / 2);
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
    for (int i = 0; i < storyline->when_count; i++) {
        WhenData wd = storyline->when[i];
        When *w = createWhen();
        w->condition = mapCondition(wd.condition);
        addDebug(g->log, "condition: %s, mapped to: %d, story: %s",
                 wd.condition,
                 w->condition,
                 wd.story);
        if (wd.story != NULL) {
            w->story = wd.story;
        }
        w->source = g->player->mob;
        if (wd.mob != NULL) {
            w->mobileTrigger = findMobById(g, wd.mob);
            addDebug(g->log, "mobileTrigger is '%s', mob: '%s'", wd.mob, w->mobileTrigger->name);
        }
        c->when[i] = w;
    }
    addDebug(g->log, "done processing when conditions");
    for (int i = 0; i < storyline->then_count; i++) {
        ThenData td = storyline->then[i];
        Then *t = createThen();
        if (td.player) {
            t->target = g->player->mob;
        } else {
            t->target = findMobById(g, td.mob);
        }
        t->story = &td.story[0];
        t->outcome = mapOutcome(td.action);
        t->message = &td.message[0];
        addDebug(g->log, "then story is '%s', outcome: %d, message: %s",
                 t->story, t->outcome, t->message);
        c->then[i] = t;
    }
    return c;
}

void loadScenes(Game *g, RuntimeArgs  *r, char *scenes[MAX_SCENES]) {
    for (int i = 0; i < g->sceneCount; i++) {
        g->scenes[i] = loadScene(g->log, g->beastiary, r->indexDir, scenes[i], r->showCollisions);
        addDebug(g->log, "scene %s (%d) loaded", g->scenes[i]->name, i);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        for (int c = 0; c < g->scenes[i]->storylineCount; c++) {
            g->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(g, g->scenes[i]->storylines[c]);
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
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i] != NULL && g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
}

void evaluateExits(Game *g) {
    Exploration *e = g->currentScene->exploration;
    int exit = atExit(e, g->player);
    if (exit > -1) {
        char *to = e->exits[exit]->to;
        for (int i = 0; i < g->sceneCount; i++) {
            if (strcmp(to, g->scenes[i]->name) == 0) {
                g->player->mob->position = (Vector2) {
                    (float) e->exits[exit]->x,
                    (float) e->exits[exit]->y
                };
                setScene(g, g->scenes[i]);
                return;
            }
        }
        addError(g->log, "warp to '%s' not found", to);
    }
}

void doExplorationLoop(Game *g) {
    checkExplorationInput(g->currentScene->exploration, g->player, g->currentScene->activeControlBlock);
    BeginDrawing();
    drawExplorationView(g->currentScene->exploration, g->player, g->currentScene->activeControlBlock);
    EndDrawing();
    processExplorationAnimations(g);
    evaluateMovement(g->currentScene->exploration, g->player);
    evaluateExits(g);
    checkControls(g->currentScene, g->player);
    checkFights(g->currentScene, g->player);
    UpdateMusicStream(g->audioManager->music[g->audioManager->musicIndex]->music);
}

void doFightLoop(Game *g) {
//    checkInput(g->currentScene, g->player);
    BeginDrawing();
    drawFightView(g->currentScene->encounters, g->currentScene->fight, g->player);
    EndDrawing();
//    processAnimations(g);
//    evaluateMovement(g->currentScene, g->player);
//    evaluateExits(g);
//    checkControls(g->currentScene, g->player);
//    checkFights(g->currentScene, g->player);
    UpdateMusicStream(g->audioManager->music[g->audioManager->musicIndex]->music);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        if (isFighting(g->currentScene)) {
            doFightLoop(g);
        } else if (isExploring(g->currentScene)) {
            doExplorationLoop(g);
        }
    }
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    g->log = createLog(r->debug);
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
    setScene(g, g->scenes[r->sceneIndex]);
    addDebug(g->log, "done creating game object");
    return g;
}
