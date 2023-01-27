Game *createGameInstance() {
    Game *g = malloc(sizeof(struct Game));
    g->scene = 0;
    g->player = malloc(sizeof(struct Player));
    g->player->sprite = createTestHumanoid();
    return g;
}
