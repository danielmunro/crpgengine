typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Player *player;
    Tilemap *tilemap;
    int layers;
    int type;
    char name[SCENE_NAME_MAX_LENGTH];
} Scene;

typedef struct SceneType {
    int code;
    const char *scene;
} SceneType;

const SceneType sceneTypes[] = {
    {SCENE_TYPE_FREE_MOVE, "free-move"},
    {SCENE_TYPE_FIGHT, "fight"},
    {SCENE_TYPE_MENU, "menu"},
};

void movePlayer(Scene *s, int direction) {
    s->player->sprite->direction = direction;
    if (direction == DIRECTION_UP) {
        s->player->y = s->player->y - 1;
    } else if (direction == DIRECTION_DOWN) {
        s->player->y = s->player->y + 1;
    }
    if (direction == DIRECTION_LEFT) {
        s->player->x = s->player->x - 1;
    } else if (direction == DIRECTION_RIGHT) {
        s->player->x = s->player->x + 1;
    }
    s->player->isMoving = 1;
}

void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_FREE_MOVE) {
        if (IsKeyDown(KEY_RIGHT)) movePlayer(s, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) movePlayer(s, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) movePlayer(s, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) movePlayer(s, DIRECTION_DOWN);
        else s->player->isMoving = 0;
    }
}

void drawLayer(Scene *s, Layer layer) {
    int tiles_x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int tiles_y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    int start_y = s->player->y - (tiles_y / 2);
    int start_x = s->player->x - (tiles_x / 2);
    Vector2D sz = s->tilemap->size;
    int i = 0;
    while (i < MAX_LAYER_COUNT && i < s->layers) {
        for (int y = -1; y < tiles_y; y++) {
            for (int x = -1; x < tiles_x; x++) {
                if (start_x + x < 0 || start_y + y < 0 || start_x + x > MAX_LAYER_SIZE ||
                    start_y + y > MAX_LAYER_SIZE) {
                    continue;
                }
                int index = layer[i][start_y + y][start_x + x];
                if (index < 0) {
                    continue;
                }
                Vector2D t = getTileFromIndex(s->tilemap, index);
                Rectangle frameRec = {
                        (float) (t.x * sz.x),
                        (float) (t.y * sz.y),
                        (float) sz.x,
                        (float) sz.y
                };
                Vector2 pos = {
                        (float) (sz.x * x),
                        (float) (sz.y * y)
                };
                DrawTextureRec(
                        s->tilemap->source,
                        frameRec,
                        pos,
                        WHITE
                );
            }
        }
        i++;
    }
}

void drawScene(Scene *s) {
    ClearBackground(RAYWHITE);
    drawLayer(s, s->tilemap->layers);
    drawSprite(s->player->sprite);
}
