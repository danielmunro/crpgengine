typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int sceneCount;
    Scene *currentScene;
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
    AudioManager *audioManager;
    int animIndex;
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

void setScene(Game *g, Scene *scene) {
    printf("request scene set to %s\n", scene->name);
    if (g->currentScene != NULL) {
        free(g->currentScene);
    }
//    g->currentScene = createScene();
//    strcpy(g->currentScene->name, scene->name);
//    g->currentScene->tilemap = scene->tilemap;
//    g->currentScene->entrance = scene->entrance;
//    g->currentScene->showCollisions = scene->showCollisions;
//    g->currentScene->layerCount = scene->layerCount;
//    g->currentScene->music = scene->music;
//    for (int i = 0; i < MAX_EXITS; i++) {
//        g->currentScene->exits[i] = scene->exits[i];
//    }
//    for (int i = 0; i < MAX_OBJECTS; i++) {
//        g->currentScene->objects[i] = scene->objects[i];
//    }
//    for (int i = 0; i < LAYER_COUNT; i++) {
//        g->currentScene->layers[i] = scene->layers[i];
//    }
//    for (int i = 0; i < MAX_MOBILES; i++) {
//        g->currentScene->mobiles[i] = scene->mobiles[i];
//    }
    g->currentScene = scene;

    memset(g->animations, 0, sizeof(g->animations));
    g->animIndex = 0;
    addAllAnimations(g, g->player->mob->animations);
    Rectangle r = g->currentScene->entrance;
    g->player->pos.x = r.x + (r.width / 2);
    g->player->pos.y = r.y + (r.height / 2);
    drawScene(g->currentScene);
    printf("current scene: %s\n", g->currentScene->name);
    printf("play music %s\n", g->currentScene->music);
    playMusic(g->audioManager, g->currentScene->music);
    printf("scene set to %s\n", g->currentScene->name);
}

int mapCondition(char *when) {
    if (strcmp(when, WHEN_HAS) == 0) {
        return CONDITION_HAS;
    } else if (strcmp(when, WHEN_NOT_HAS) == 0) {
        return CONDITION_NOT_HAS;
    } else if (strcmp(when, WHEN_ENGAGED) == 0) {
        return CONDITION_ENGAGED;
    } else if (strcmp(when, WHEN_AT) == 0) {
        return CONDITION_AT;
    }
}

int mapOutcome(char *then) {
    if (strcmp(then, THEN_SPEAK) == 0) {
        return OUTCOME_SPEAK;
    } else if (strcmp(then, THEN_MOVE_TO) == 0) {
        return OUTCOME_MOVE_TO;
    } else if (strcmp(then, THEN_DIRECTION) == 0) {
        return OUTCOME_DIRECTION;
    } else if (strcmp(then, THEN_SPRITE) == 0) {
        return OUTCOME_SPRITE;
    } else if (strcmp(then, THEN_WAIT) == 0) {
        return OUTCOME_WAIT;
    } else if (strcmp(then, THEN_GIVE_ITEM) == 0) {
        return OUTCOME_GIVE_ITEM;
    } else if (strcmp(then, THEN_TAKE) == 0) {
        return OUTCOME_TAKE;
    }
    return -1;
}

ControlBlock *mapIntermediateToReal(Game *g, ControlBlockInt *cbi) {
    printf("mapping int control blocks to real %d, %d\n", cbi->whenCount, cbi->thenCount);
    ControlBlock *c = createControlBlock(cbi->control);
    c->whenCount = cbi->whenCount;
    c->thenCount = cbi->thenCount;
    for (int i = 0; i < cbi->whenCount; i++) {
        char *source = strtok(cbi->when[i][0], ".");
        char *condition = strtok(NULL, ".");
        printf("source, condition -- %s, %s\n", source, condition);
        c->when[i] = malloc(sizeof(When));
        c->when[i]->condition = mapCondition(condition);
        if (strcmp(source, "player") == 0) {
            c->when[i]->source = g->player->mob;
        }
        for (int s = 0; s < g->sceneCount; s++) {
            for (int m = 0; m < MAX_MOBILES; m++) {
                if (g->scenes[s]->mobiles[m] == NULL) {
                    break;
                }
                printf("check mob %s\n", g->scenes[s]->mobiles[m]->id);
                if (strcmp(g->scenes[s]->mobiles[m]->id, source) == 0) {
                    c->when[i]->source = g->scenes[s]->mobiles[m];
                    printf("found source %s\n", source);
                }
                printf("condition check %s, %s\n", condition, cbi->when[i][1]);
                if (c->when[i]->condition == CONDITION_ENGAGED && strcmp(g->scenes[s]->mobiles[m]->id, cbi->when[i][1]) == 0) {
                    printf("set mobile trigger to %s\n", g->scenes[s]->mobiles[m]->name);
                    c->when[i]->mobileTrigger = g->scenes[s]->mobiles[m];
                }
            }
        }
        if (c->when[i]->source == NULL) {
            fprintf(stderr, "source not found: %s!", source);
        }
    }
    for (int i = 0; i < cbi->thenCount; i++) {
        char *target = strtok(cbi->then[i][0], ".");
        char *outcome = strtok(NULL, ".");
        printf("target, outcome -- %s, %s\n", target, outcome);
        c->then[i] = malloc(sizeof(Then));
        char *toMap;
        if (outcome != NULL) {
            toMap = outcome;
        } else {
            toMap = target;
        }
        c->then[i]->outcome = mapOutcome(toMap);
        printf("outcome is set to %d\n", c->then[i]->outcome);
        if (strcmp(target, "player") == 0) {
            c->then[i]->target = g->player->mob;
        }
        for (int s = 0; s < g->sceneCount; s++) {
            for (int m = 0; m < MAX_MOBILES; m++) {
                if (g->scenes[s]->mobiles[m] == NULL) {
                    break;
                }
                printf("check mob %s\n", g->scenes[s]->mobiles[m]->id);
                if (strcmp(g->scenes[s]->mobiles[m]->id, target) == 0) {
                    c->then[i]->target = g->scenes[s]->mobiles[m];
                    printf("found target %s\n", target);
                }
            }
        }
        if (c->then[i]->outcome == OUTCOME_SPEAK) {
            c->then[i]->message = &cbi->then[i][1][0];
        }
        if (c->then[i]->target == NULL && c->then[i]->outcome != OUTCOME_WAIT) {
            fprintf(stderr, "target not found: %s!\n", target);
        }
    }
    return c;
}

void loadScenes(Game *g, int showCollisions, char *indexDir, char *scenes[MAX_SCENES]) {
    for (int i = 0; i < g->sceneCount; i++) {
        g->scenes[i] = loadScene(indexDir, scenes[i], showCollisions);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        for (int c = 0; c < MAX_CONTROLS; c++) {
            if (g->scenes[i]->controlBlocksInt[c] == NULL) {
                break;
            }
            g->scenes[i]->controlBlocks[c] = mapIntermediateToReal(g, g->scenes[i]->controlBlocksInt[c]);
        }
    }
}

void processAnimations(Game *g) {
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i] != NULL && g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
}

void evaluateExits(Game *g) {
    Scene *s = g->currentScene;
    int exit = atExit(s, g->player);
    if (exit > -1) {
        char *to = s->exits[exit]->to;
        for (int i = 0; i < g->sceneCount; i++) {
            if (strcmp(to, g->scenes[i]->name) == 0) {
                g->player->mob->position = (Vector2) {
                    (float) s->exits[exit]->x,
                    (float) s->exits[exit]->y
                };
                setScene(g, g->scenes[i]);
                break;
            }
        }
        fprintf(stderr, "warp to %s not found\n", to);
    }
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        checkInput(g->player);
        BeginDrawing();
        renderScene(g->currentScene, g->player);
        drawControl(g->player, g->currentScene->activeControlBlock);
        EndDrawing();
        processAnimations(g);
        evaluateMovement(g->currentScene, g->player);
        evaluateExits(g);
        controlWhenCheck(g->currentScene, g->player);
        UpdateMusicStream(g->audioManager->music[g->audioManager->musicIndex]->music);
    }
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    g->audioManager = loadAudioManager(r->indexDir);
    printf("audio manager loaded %d songs\n", g->audioManager->musicCount);
    char *scenes[MAX_SCENES];
    char *sceneDir = pathCat(r->indexDir, "/scenes");
    g->sceneCount = getFilesInDirectory(sceneDir, scenes);
    g->player = loadPlayer(r->indexDir);
    loadScenes(g, r->showCollisions, r->indexDir, scenes);
    setScene(g, g->scenes[r->sceneIndex]);
    return g;
}
