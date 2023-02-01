typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Game *createGameInstance() {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    Player *player = malloc(sizeof(Player));
    player->sprite = createTestHumanoid();
    g->scenes[g->scene] = createTestScene(player);
    return g;
}
