typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Player *player;
    Tilemap *tilemap;
    int type;
    char name[SCENE_NAME_MAX_LENGTH];
} Scene;

Scene *createTestScene() {
    Scene *scene = malloc(sizeof(Scene));
    scene->tilemap = createTestTilemap();
    scene->player = createTestPlayer();
    scene->type = SCENE_TYPE_FREE_MOVE;
    strcpy(scene->name, "test town map");
    return scene;
}

void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_FREE_MOVE) {
        if (IsKeyDown(KEY_RIGHT)) move(s->player->sprite, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) move(s->player->sprite, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) move(s->player->sprite, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) move(s->player->sprite, DIRECTION_DOWN);
    }
}
