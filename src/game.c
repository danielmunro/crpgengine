typedef struct Game {
    Scene *scenes[MAX_SCENES];
    Scene *currentScene;
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
    char indexDir[255];
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
        printf("add animation %d\n", i);
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
    memset(g->animations, 0, sizeof(g->animations));
    g->animIndex = 0;
    addAllAnimations(g, g->player->mob->animations);
    Rectangle r = g->currentScene->entrance;
    g->player->pos.x = r.x + (r.width / 2);
    g->player->pos.y = r.y + (r.height / 2);
    drawScene(g->currentScene);
    printf("scene set to %s\n", g->currentScene->name);
}

Game *createGameInstance(int sceneIndex, int showCollisions, char *indexDir) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    strcpy(g->indexDir, indexDir);
    for (int i = 0; i < MAX_SCENES; i++) {
        g->scenes[i] = NULL;
    }
    char *scenes[MAX_SCENES];
    loadIndex(indexDir, scenes);
    for (int i = 0; i < MAX_SCENES; i++) {
        if (scenes[i] == NULL) {
            break;
        }
        g->scenes[i] = loadScene(indexDir, scenes[i], showCollisions);
    }
    g->player = loadPlayer(indexDir);
    setScene(g, g->scenes[sceneIndex]);
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
