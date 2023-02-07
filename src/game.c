typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Scene *getScene(Game *g) {
    return g->scenes[g->scene];
}

Game *createGameInstance(int sceneIndex, int showCollisions) {
    Game *g = malloc(sizeof(Game));
    g->scene = sceneIndex;
    g->scenes[0] = loadScene("./resources/firsttown.scene", showCollisions);
    g->scenes[1] = loadScene("./resources/firstdungeon.scene", showCollisions);
    g->scenes[g->scene]->player = createTestPlayer();
    g->scenes[g->scene]->type = SCENE_TYPE_FREE_MOVE;
    return g;
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        Scene *s = getScene(g);
        checkInput(s);
        BeginDrawing();
        drawScene(s);
        EndDrawing();
        if (s->player->isMoving) {
            animateSprite(s->player->sprite);
        }
    }
}
