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

void movePlayer(Scene *s, int direction) {
    if (s->player->offset.x != 0 || s->player->offset.y != 0) {
        return;
    }
    s->player->sprite->direction = direction;
    if (direction == DIRECTION_UP) {
        s->player->y = s->player->y - 1;
        s->player->offset.y = -s->tilemap->size.y;
    } else if (direction == DIRECTION_DOWN) {
        s->player->y = s->player->y + 1;
        s->player->offset.y = s->tilemap->size.y;
    } else if (direction == DIRECTION_LEFT) {
        s->player->x = s->player->x - 1;
        s->player->offset.x = -s->tilemap->size.x;
    } else if (direction == DIRECTION_RIGHT) {
        s->player->x = s->player->x + 1;
        s->player->offset.x = s->tilemap->size.x;
    }
}

void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_FREE_MOVE) {
        if (IsKeyDown(KEY_RIGHT)) movePlayer(s, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) movePlayer(s, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) movePlayer(s, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) movePlayer(s, DIRECTION_DOWN);
    }
}

void drawLayer(Scene *s, Layer layer) {
    int tiles_x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int tiles_y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    int start_y = s->player->y - (tiles_y / 2);
    int start_x = s->player->x - (tiles_x / 2);
    Vector2D sz = s->tilemap->size;
    for (int y = -1; y < tiles_y; y++) {
        for (int x = -1; x < tiles_x; x++) {
            if (start_x + x < 0 || start_y + y < 0 || start_x + x > MAX_LAYER_SIZE || start_y + y > MAX_LAYER_SIZE) {
                continue;
            }
            int index = layer[start_y + y][start_x + x];
            Tile t = s->tilemap->tiles[index];
            Rectangle frameRec = {(float)t.x, (float)t.y, (float)sz.x, (float)sz.y};
            Vector2 pos = {
                    (float)((sz.x * x) + s->player->offset.x),
                    (float)((sz.y * y) + s->player->offset.y)
            };
            DrawTextureRec(
                    s->tilemap->source,
                    frameRec,
                    pos,
                    WHITE
            );
        }
    }
}

void drawScene(Scene *s) {
    ClearBackground(RAYWHITE);
    drawLayer(s, s->tilemap->background);
    drawSprite(s->player->sprite);
}
