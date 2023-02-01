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
        if (IsKeyDown(KEY_RIGHT)) movePlayer(s->player, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) movePlayer(s->player, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) movePlayer(s->player, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) movePlayer(s->player, DIRECTION_DOWN);
    }
}

void drawScene(Scene *s) {
    ClearBackground(RAYWHITE);
    int tiles_y = 28;
    int tiles_x = 48;
    int start_y = s->player->y - (tiles_y / 2);
    int start_x = s->player->x - (tiles_x / 2);
    int cur_x = 0;
    int cur_y = 0;
    for (int y = 0; y < tiles_y; y++) {
        cur_x = 0;
        for (int x = 0; x < tiles_x; x++) {
            if (start_x + x < 0 || start_y + y < 0) {
                continue;
            }
            int index = s->tilemap->background[start_y + y][start_x + x];
            Tile t = s->tilemap->tiles[index];
            Rectangle frameRec = {(float)t.x, (float)t.y, 16.0f, 16.0f};
            Vector2 pos = {16.0f * (float)x, 16.0f * (float)y};
            DrawTextureRec(
                  s->tilemap->source,
                  frameRec,
                  pos,
                  WHITE
            );
            cur_x = cur_x + 1;
        }
        cur_y = cur_y + 1;
    }

    DrawTextureRec(
            s->player->sprite->source,
            s->player->sprite->frameRec,
            s->player->sprite->position,
            WHITE
    );
}
