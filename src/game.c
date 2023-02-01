typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Game *createGameInstance() {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    g->scenes[g->scene] = createTestScene();
    return g;
}
