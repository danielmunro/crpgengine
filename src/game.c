typedef struct Game {
    Player *player;
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Game *createGameInstance() {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    g->player = malloc(sizeof(Player));
    g->player->sprite = createTestHumanoid();
    g->scenes[0] = createTestScene();
    return g;
}
