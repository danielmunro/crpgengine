typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Scene *getScene(Game *g) {
    return g->scenes[g->scene];
}

Game *createGameInstance(char *resourceFile, int showCollisions) {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    g->scenes[g->scene] = loadScene(resourceFile, showCollisions);
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
