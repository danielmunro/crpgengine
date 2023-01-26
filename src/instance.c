GameT *createGameInstance() {
    GameT *g = malloc(sizeof(struct Game));
//    struct Scene scenes[MAX_SCENES];
    g->sp = createTestHumanoid();
//    g->scenes = scenes;
    return g;
}
