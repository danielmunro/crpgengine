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
    g->scenes[g->scene]->type = SCENE_TYPE_FREE_MOVE;
    Player *p = createTestPlayer();
    Rectangle r = g->scenes[g->scene]->entrance;
    p->pos.x = r.x + (r.width / 2);
    p->pos.y = r.y + (r.height / 2);
    g->scenes[g->scene]->player = p;
    return g;
}

void run(Game *g) {
    printf("run game\n");
    while (!WindowShouldClose()) {
        Scene *s = getScene(g);
        checkInput(s);
        BeginDrawing();
        drawScene(s);
        EndDrawing();
        Rectangle pr = {
                s->player->pos.x,
                s->player->pos.y,
                HUMANOID_WIDTH,
                HUMANOID_HEIGHT,
        };
        DrawRectangleRec(s->exit->area, PINK);
        DrawRectangleRec(pr, BLUE);
        if (CheckCollisionRecs(pr, s->exit->area)) {
            printf("exit collision detected\n");
            int i = 0;
            while (g->scenes[i] != NULL) {
                if (strcmp(g->scenes[i]->name, s->exit->to) == 0) {
                    g->scene = i;
                }
                s++;
            }
        }
        if (s->player->isMoving) {
            animateSprite(s->player->sprite);
        }
    }
}
