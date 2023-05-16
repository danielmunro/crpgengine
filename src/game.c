typedef struct Game {
    Scene *scenes[MAX_SCENES];
    Scene *currentScene;
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
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
    g->currentScene = createScene();
    strcpy(g->currentScene->name, scene->name);
    g->currentScene->tilemap = scene->tilemap;
    g->currentScene->entrance = scene->entrance;
    g->currentScene->showCollisions = scene->showCollisions;
    g->currentScene->layerCount = scene->layerCount;
    for (int i = 0; i < MAX_EXITS; i++) {
        g->currentScene->exits[i] = scene->exits[i];
    }
    for (int i = 0; i < MAX_OBJECTS; i++) {
        g->currentScene->objects[i] = scene->objects[i];
    }
    for (int i = 0; i < LAYER_COUNT; i++) {
        g->currentScene->layers[i] = scene->layers[i];
    }
    for (int i = 0; i < MAX_MOBILES; i++) {
        g->currentScene->mobiles[i] = scene->mobiles[i];
    }
    memset(g->animations, 0, sizeof(g->animations));
    g->animIndex = 0;
    addAllAnimations(g, g->player->mob->animations);
    Rectangle r = g->currentScene->entrance;
    g->player->pos.x = r.x + (r.width / 2);
    g->player->pos.y = r.y + (r.height / 2);
    drawScene(g->currentScene);
    printf("scene set to %s\n", g->currentScene->name);
}

ControlBlock *mapIntermediateToReal(Game *g, ControlBlockInt *cbi) {
    printf("mapping int control blocks to real %d\n", cbi->whenCount);
    ControlBlock *c = createControlBlock(cbi->control);
    c->whenCount = cbi->whenCount;
    c->thenCount = cbi->thenCount;
    for (int i = 0; i < cbi->whenCount; i++) {
        printf("start when loop\n");
        char *source = strtok(cbi->when[i][0], ".");
        char *adjective = strtok(NULL, ".");
        printf("source, adj -- %s, %s\n", source, adjective);
        c->when[i] = malloc(sizeof(When));
        if (strcmp(source, "player") == 0) {
            c->when[i]->source = g->player->mob;
            continue;
        }
        printf("looking in scenes\n");
        for (int s = 0; s < MAX_SCENES; s++) {
            if (g->scenes[s] == NULL) {
                break;
            }
            for (int m = 0; m < MAX_MOBILES; m++) {
                if (g->scenes[s]->mobiles[m] == NULL) {
                    break;
                }
                printf("check mob %s\n", g->scenes[s]->mobiles[m]->id);
                if (strcmp(g->scenes[s]->mobiles[m]->id, source) == 0) {
                    c->when[i]->source = g->scenes[s]->mobiles[m];
                    printf("found source %s\n", source);
                }
            }
        }
        if (c->when[i]->source == NULL) {
            fprintf(stderr, "source not found: %s!", source);
        }
    }
    for (int i = 0; i < cbi->thenCount; i++) {
        printf("start then loop\n");
        char *target = strtok(cbi->then[i][0], ".");
        char *adjective = strtok(NULL, ".");
        printf("target, adj -- %s, %s\n", target, adjective);
        printf("looking in scenes\n");
        c->then[i] = malloc(sizeof(Then));
        if (strcmp(target, "player") == 0) {
            c->then[i]->target = g->player->mob;
            continue;
        } else if (strcmp(target, OUTCOME_WAIT) == 0) {
            printf("found a wait\n");
            c->then[i]->outcome = OUTCOME_WAIT_ID;
            continue;
        }
        for (int s = 0; s < MAX_SCENES; s++) {
            if (g->scenes[s] == NULL) {
                break;
            }
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
        if (c->then[i]->target == NULL) {
            fprintf(stderr, "target not found: %s!\n", target);
        }
    }
    return c;
}

void loadScenes(Game *g, int showCollisions, char *indexDir, char *scenes[MAX_SCENES]) {
    for (int i = 0; i < MAX_SCENES; i++) {
        if (scenes[i] == NULL) {
            break;
        }
        g->scenes[i] = loadScene(indexDir, scenes[i], showCollisions);
    }
    for (int i = 0; i < MAX_SCENES; i++) {
        if (scenes[i] == NULL) {
            break;
        }
        for (int c = 0; c < MAX_CONTROLS; c++) {
            if (g->scenes[i]->controlBlocksInt[c] == NULL) {
                break;
            }
            g->scenes[i]->controlBlocks[c] = mapIntermediateToReal(g, g->scenes[i]->controlBlocksInt[c]);
        }
    }
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    char *scenes[MAX_SCENES];
    for (int i = 0; i < MAX_SCENES; i++) {
        g->scenes[i] = NULL;
        scenes[i] = NULL;
    }
    char *sceneDir = pathCat(r->indexDir, "/scenes");
    getFilesInDirectory(sceneDir, scenes);
    g->player = loadPlayer(r->indexDir);
    loadScenes(g, r->showCollisions, r->indexDir, scenes);
    setScene(g, g->scenes[r->sceneIndex]);
    return g;
}

void processAnimations(Game *g) {
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i] != NULL && g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
}

void evaluateExits(Game *g) {
    int exit = atExit(g->currentScene, g->player);
    if (exit > -1) {
        char *to = g->currentScene->exits[exit]->to;
        for (int i = 0; i < MAX_SCENES; i++) {
            if (g->scenes[i] == NULL) {
                break;
            }
            if (strcmp(to, g->scenes[i]->name) == 0) {
                g->player->mob->position.x = (float) g->currentScene->exits[exit]->x;
                g->player->mob->position.y = (float) g->currentScene->exits[exit]->y;
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
        EndDrawing();
        processAnimations(g);
        evaluateMovement(g->currentScene, g->player);
        evaluateExits(g);
    }
}
