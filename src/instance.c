Game *createGameInstance() {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    g->player = malloc(sizeof(Player));
    g->player->sprite = createTestHumanoid();
    return g;
}
