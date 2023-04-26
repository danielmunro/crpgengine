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
    free(g->currentScene);
    g->currentScene = malloc(sizeof(Scene));
    g->currentScene->tilemap = scene->tilemap;
    g->currentScene->entrance = scene->entrance;
    g->currentScene->exit = scene->exit;
    g->currentScene->showCollisions = scene->showCollisions;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (scene->objects[i] == NULL) {
            break;
        }
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
}

Game *createGameInstance(int sceneIndex, int showCollisions) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->scenes[0] = loadScene("./resources/firsttown.scene", showCollisions);
    g->scenes[1] = loadScene("./resources/firstdungeon.scene", showCollisions);
    g->player = createTestPlayer();
    setScene(g, g->scenes[sceneIndex]);
    return g;
}

void processAnimations(Game *g) {
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        checkInput(g->player);
        BeginDrawing();
        drawScene(g->currentScene, g->player);
        EndDrawing();
        processAnimations(g);
    }
}
