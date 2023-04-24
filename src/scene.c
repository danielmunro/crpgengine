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

void checkInput(Scene *s, Player *p) {
    if (s->type == SCENE_TYPE_TOWN) {
        checkMoveKey(
                p,
                KEY_UP,
                DIRECTION_UP
        );
        checkMoveKey(
                p,
                KEY_DOWN,
                DIRECTION_DOWN
        );
        checkMoveKey(
                p,
                KEY_RIGHT,
                DIRECTION_RIGHT
        );
        checkMoveKey(
                p,
                KEY_LEFT,
                DIRECTION_LEFT
        );
    }
}

void drawLayer(Scene *s, Player *p, int layer) {
    Vector2d tiles = getTileCount(s);
    Vector2d start = getStartTileCoords(s->tilemap->size, p->pos, tiles);
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
            Vector2 offset = getOffset(s->tilemap->size, p->pos);
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

void drawScene(Scene *s, Player *p) {
    ClearBackground(BLACK);
    drawLayer(s, p, LAYER_TYPE_BACKGROUND);
    drawLayer(s, p, LAYER_TYPE_MIDGROUND);
    drawAnimation(getMobAnimation(p->mob), p->mob->position);
    drawLayer(s, p, LAYER_TYPE_FOREGROUND);
}
