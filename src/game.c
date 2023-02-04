typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int scene;
} Game;

Game *createGameInstance() {
    Game *g = malloc(sizeof(Game));
    g->scene = 0;
    g->scenes[g->scene] = loadScene("./resources/firsttown.scene");
    g->scenes[g->scene]->player = createTestPlayer();
    g->scenes[g->scene]->type = SCENE_TYPE_FREE_MOVE;
//    streamXmlFile("./resources/tiled/tinytown.tsx");
    return g;
}
