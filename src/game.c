typedef struct Game {
    Scene *scenes[MAX_SCENES];
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
    int animIndex;
    int scene;
} Game;

Scene *getScene(Game *g) {
    return g->scenes[g->scene];
}

void addAnimation(Game *g, Animation *a) {
    g->animations[g->animIndex] = a;
    g->animIndex++;
}

Game *createGameInstance(int sceneIndex, int showCollisions) {
    Game *g = malloc(sizeof(Game));
    g->scene = sceneIndex;
    g->animIndex = 0;
    g->scenes[0] = loadScene("./resources/firsttown.scene", showCollisions);
    g->scenes[1] = loadScene("./resources/firstdungeon.scene", showCollisions);
    g->scenes[g->scene]->type = SCENE_TYPE_TOWN;
    Player *p = createTestPlayer();
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (p->mob->animations[i] == NULL) {
            break;
        }
        addAnimation(g, p->mob->animations[i]);
    }
    Rectangle r = g->scenes[g->scene]->entrance;
    p->pos.x = r.x + (r.width / 2);
    p->pos.y = r.y + (r.height / 2);
    g->player = p;
    return g;
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        Scene *s = getScene(g);
        checkInput(g->player);
        BeginDrawing();
        drawScene(s, g->player);
        EndDrawing();

        // move to animation manager
        for (int i = 0; i < g->animIndex; i++) {
            if (g->animations[i]->isPlaying) {
                incrementAnimFrame(g->animations[i]);
            }
        }
    }
}
