typedef struct Object {
    Rectangle *rect;
    int tile;
} Object;

typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Player *player;
    Tilemap *tilemap;
    Object objects[MAX_OBJECTS];
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

void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_FREE_MOVE) {
        if (IsKeyDown(KEY_RIGHT)) movePlayer(s->player, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) movePlayer(s->player, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) movePlayer(s->player, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) movePlayer(s->player, DIRECTION_DOWN);
        else s->player->isMoving = 0;
    }
}

void drawLayer(Scene *s, Layer layer) {
    int tiles_x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int tiles_y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    int start_y = (int)s->player->pos.y - (tiles_y / 2);
    int start_x = (int)s->player->pos.x - (tiles_x / 2);
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
                int xint = (int)s->player->pos.x;
                float xdiff = s->player->pos.x - (float)xint;
                int yint = (int)s->player->pos.y;
                float ydiff = s->player->pos.y - (float)yint;
                Vector2 pos = {
                        (float) (sz.x * x) - (xdiff * (float)sz.x),
                        (float) (sz.y * y) - (ydiff * (float)sz.y)
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
