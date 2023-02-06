typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Player *player;
    Tilemap *tilemap;
    int layers;
    int type;
    char name[SCENE_NAME_MAX_LENGTH];
    int showCollisions;
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

Object *getObject(Scene *s, int index) {
    for (int l = 0; l < s->layers; l++) {
        int o = 0;
        while (s->tilemap->objects[o] != NULL) {
            if (s->tilemap->objects[o]->tile == index) {
                return s->tilemap->objects[o];
            }
            o++;
        }
    }
    return NULL;
}

int checkCollision(Scene *s, Vector2 playerPos) {
    int tiles_x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int tiles_y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    int start_y = (int)playerPos.y - (tiles_y / 2);
    int start_x = (int)playerPos.x - (tiles_x / 2);
    Vector2D sz = s->tilemap->size;
    int i = 0;
    while (i < MAX_LAYER_COUNT && i < s->layers) {
        for (int y = -1; y < tiles_y; y++) {
            for (int x = -1; x < tiles_x; x++) {
                if (start_x + x < 0 || start_y + y < 0 || start_x + x > MAX_LAYER_SIZE ||
                    start_y + y > MAX_LAYER_SIZE) {
                    continue;
                }
                int index = s->tilemap->layers[i][start_y + y][start_x + x];
                if (index <= 0) {
                    continue;
                }
                int xint = (int)playerPos.x;
                float xdiff = playerPos.x - (float)xint;
                int yint = (int)playerPos.y;
                float ydiff = playerPos.y - (float)yint;
                Object *o = getObject(s, index - 1);
                if (o != NULL) {
                    Rectangle r = {
                            (float) (sz.x * x) - (xdiff * (float) sz.x) + o->rect.x,
                            (float) (sz.y * y) - (ydiff * (float) sz.y) + o->rect.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    Rectangle pr = {
                            s->player->sprite->position.x + 3,
                            s->player->sprite->position.y + 12,
                            HUMANOID_WIDTH - 3,
                            HUMANOID_HEIGHT - 12,
                    };
                    if (CheckCollisionRecs(pr, r)) {
                        return 1;
                    }
                }
            }
        }
        i++;
    }
    return 0;
}


void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_FREE_MOVE) {
        if (IsKeyDown(KEY_RIGHT)) {
            Vector2 p = {s->player->pos.x + MOVE_AMOUNT, s->player->pos.y};
            if (checkCollision(s, p)) {
                return;
            }
            movePlayer(s->player, DIRECTION_RIGHT, p);
        }
        else if (IsKeyDown(KEY_LEFT)) {
            Vector2 p = {s->player->pos.x - MOVE_AMOUNT, s->player->pos.y};
            if (checkCollision(s, p)) {
                return;
            }
            movePlayer(s->player, DIRECTION_LEFT, p);
        }
        else if (IsKeyDown(KEY_UP)) {
            Vector2 p = {s->player->pos.x, s->player->pos.y - MOVE_AMOUNT};
            if (checkCollision(s, p)) {
                return;
            }
            movePlayer(s->player, DIRECTION_UP, p);
        }
        else if (IsKeyDown(KEY_DOWN)) {
            Vector2 p = {s->player->pos.x, s->player->pos.y + MOVE_AMOUNT};
            if (checkCollision(s, p)) {
                return;
            }
            movePlayer(s->player, DIRECTION_DOWN, p);
        }
        else s->player->isMoving = 0;
    }
}

void drawLayers(Scene *s) {
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
                int index = s->tilemap->layers[i][start_y + y][start_x + x];
                if (index <= 0) {
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
                if (s->showCollisions) {
                    Object *o = getObject(s, index - 1);
                    if (o != NULL) {
                        Rectangle r = {
                                (float) (sz.x * x) - (xdiff * (float) sz.x) + o->rect.x,
                                (float) (sz.y * y) - (ydiff * (float) sz.y) + o->rect.y,
                                o->rect.width,
                                o->rect.height,
                        };
                        Color collision = PINK;
                        Rectangle pr = {
                                s->player->sprite->position.x,
                                s->player->sprite->position.y,
                                HUMANOID_WIDTH,
                                HUMANOID_HEIGHT,
                        };
                        if (CheckCollisionRecs(pr, r)) {
                            collision = PURPLE;
                        }
                        DrawRectangleRec(r, collision);
                    }
                }
            }
        }
        i++;
    }
}

void drawScene(Scene *s) {
    ClearBackground(RAYWHITE);
    drawLayers(s);
    drawSprite(s->player->sprite);
}
