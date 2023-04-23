typedef struct Exit {
    Rectangle area;
    char *to;
} Exit;

typedef struct SceneType {
    int code;
    const char *scene;
} SceneType;

typedef struct Object {
    int id;
    Rectangle rect;
    int tile;
} Object;

typedef struct Layer {
    int type;
    char data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
} Layer;

typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Player *player;
    Tilemap *tilemap;
    Layer *layers[LAYER_COUNT];
    Object *objects[MAX_OBJECTS];
    int layerCount;
    int type;
    char name[255];
    char filename[255];
    int showCollisions;
    Rectangle entrance;
    Exit *exit;
} Scene;

const SceneType sceneTypes[] = {
    {SCENE_TYPE_TOWN,    "town"},
    {SCENE_TYPE_DUNGEON, "dungeon"},
};

Vector2 getOffset(Vector2d tileSize, Vector2 pos) {
    float xf = pos.x / (float)tileSize.x;
    int xint = (int)xf;
    float xdiff = xf - (float)xint;
    float yf = pos.y / (float)tileSize.y;
    int yint = (int)yf;
    float ydiff = yf - (float)yint;
    return (Vector2){xdiff, ydiff};
}

Object *getObject(Scene *s, int index) {
    for (int l = 0; l < s->layerCount; l++) {
        int o = 0;
        while (s->objects[o] != NULL) {
            if (s->objects[o]->tile == index) {
                return s->objects[o];
            }
            o++;
        }
    }
    return NULL;
}

Vector2d getTileCount(Scene *s) {
    int x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    return (Vector2d){x, y};
}

Vector2d getStartTileCoords(Vector2d tileSize, Vector2 playerPos, Vector2d tiles) {
    return (Vector2d){
            (int)(playerPos.x / (float)tileSize.x) - (tiles.x / 2),
            (int)(playerPos.y / (float)tileSize.y) - (tiles.y / 2),
    };
}

int isInBounds(Vector2d v, Vector2d c) {
    return v.x + c.x > 0 &&
           v.y + c.y > 0 &&
           v.x + c.x < MAX_LAYER_SIZE &&
           v.y + c.y < MAX_LAYER_SIZE;
}

int checkCollision(Scene *s, Vector2 playerPos) {
    Vector2d tiles = getTileCount(s);
    Vector2d start = getStartTileCoords(s->tilemap->size, playerPos, tiles);
    Vector2d sz = s->tilemap->size;
    int i = 0;
    while (i < s->layerCount) {
        if (s->layers[i]->type == LAYER_TYPE_FOREGROUND) {
            i++;
            continue;
        }
        for (int y = -1; y < tiles.y; y++) {
            for (int x = -1; x < tiles.x; x++) {
                if (isInBounds(start, (Vector2d){x, y}) == 0) {
                    continue;
                }
                int index = s->layers[i]->data[start.y + y][start.x + x];
                if (index <= 0) {
                    continue;
                }
                Vector2 offset = getOffset(s->tilemap->size, playerPos);
                Object *o = getObject(s, index - 1);
                if (o != NULL) {
                    Rectangle or = {
                            (float) (sz.x * x) - (offset.x * (float) sz.x) + o->rect.x,
                            (float) (sz.y * y) - (offset.y * (float) sz.y) - (float) sz.y + o->rect.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    Rectangle pr = {
                            s->player->sprite->position.x + 3,
                            s->player->sprite->position.y + 12,
                            HUMANOID_WIDTH - 3,
                            HUMANOID_HEIGHT - 12,
                    };
                    if (CheckCollisionRecs(pr, or)) {
                        return 1;
                    }
                }
            }
        }
        i++;
    }
    return 0;
}

int checkMoveKey(Scene *s, int key, int direction) {
    if (IsKeyDown(key)) {
        Vector2 p = s->player->mob->position;
        if (direction == DIRECTION_UP) {
            p.y -= 1;
        }
        if (direction == DIRECTION_DOWN) {
            p.y += 1;
        }
        if (direction == DIRECTION_LEFT) {
            p.x -= 1;
        }
        if (direction == DIRECTION_RIGHT) {
            p.x += 1;
        }
        movePlayer(s->player, direction, p);
        incrementAnimFrame(getMobAnimation(s->player->mob));
        return 1;
    }
    return 0;
}

void checkInput(Scene *s) {
    if (s->type == SCENE_TYPE_TOWN) {
        checkMoveKey(
                s,
                KEY_RIGHT,
                DIRECTION_RIGHT
        );
        checkMoveKey(
                s,
                KEY_LEFT,
                DIRECTION_LEFT
        );
        checkMoveKey(
                s,
                KEY_UP,
                DIRECTION_UP
        );
        checkMoveKey(
                s,
                KEY_DOWN,
                DIRECTION_DOWN
        );
    }
}

void drawLayer(Scene *s, int layer) {
    Vector2d tiles = getTileCount(s);
    Vector2d start = getStartTileCoords(s->tilemap->size, s->player->pos, tiles);
    Vector2d sz = s->tilemap->size;
    for (int y = -1; y < tiles.y; y++) {
        for (int x = -1; x < tiles.x; x++) {
            if (isInBounds(start, (Vector2d){x, y}) == 0) {
                continue;
            }
            int index = s->layers[layer]->data[start.y + y][start.x + x];
            if (index <= 0) {
                continue;
            }
            Vector2 offset = getOffset(s->tilemap->size, s->player->pos);
            Vector2 pos = {
                    (float) (sz.x * x) - (offset.x * (float)sz.x),
                    (float) (sz.y * y) - (offset.y * (float)sz.y) - (float) sz.y,
            };
            DrawTextureRec(
                    s->tilemap->source,
                    getRectForTile(s->tilemap, index),
                    pos,
                    WHITE
            );
            if (s->showCollisions) {
                Object *o = getObject(s, index - 1);
                if (o != NULL) {
                    Rectangle r = {
                            (float) (sz.x * x) - (offset.x * (float) sz.x) + o->rect.x,
                            (float) (sz.y * y) - (offset.y * (float) sz.y) + o->rect.y - (float) sz.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    DrawRectangleRec(r, PINK);
                }
            }
        }
    }
}

void drawScene(Scene *s) {
    ClearBackground(BLACK);
    drawLayer(s, 0);
    drawLayer(s, 1);
    drawAnimation(getMobAnimation(s->player->mob), s->player->mob->position);
    drawLayer(s, 2);
}
