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
    Texture2D renderedLayers[LAYER_COUNT];
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

void drawLayer(Scene *s, int layer) {
    Vector2d tiles = getTileCount(s);
    Vector2d sz = s->tilemap->size;
    Image renderedLayer = GenImageColor(tiles.x * sz.x, tiles.y * sz.y, BLANK);
    for (int y = -1; y < tiles.y; y++) {
        for (int x = -1; x < tiles.x; x++) {
            int index = s->layers[layer]->data[y][x];
            if (index <= 0) {
                continue;
            }
            Vector2 pos = {
                    (float) (sz.x * x),
                    (float) (sz.y * y),
            };
            ImageDraw(
                    &renderedLayer,
                    s->tilemap->source,
                    getRectForTile(s->tilemap, index),
                    (Rectangle) { pos.x, pos.y, (float) sz.x, (float) sz.y },
                    WHITE
            );
//            if (s->showCollisions) {
//                Object *o = getObject(s, index - 1);
//                if (o != NULL) {
//                    Rectangle r = {
//                            (float) (sz.x * x) + o->rect.x,
//                            (float) (sz.y * y) + o->rect.y,
//                            o->rect.width,
//                            o->rect.height,
//                    };
//                    DrawRectangleRec(r, PINK);
//                }
//            }
        }
    }
    s->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
}

void drawScene(Scene *s, Player *p) {
    ClearBackground(BLACK);
    drawLayer(s, LAYER_TYPE_BACKGROUND);
    drawLayer(s, LAYER_TYPE_MIDGROUND);
    drawLayer(s, LAYER_TYPE_FOREGROUND);
}

void renderScene(Scene *s, Player *p) {
    ClearBackground(BLACK);
    DrawTexture(s->renderedLayers[LAYER_TYPE_BACKGROUND], 0, 0, WHITE);
    DrawTexture(s->renderedLayers[LAYER_TYPE_MIDGROUND], 0, 0, WHITE);
    drawAnimation(getMobAnimation(p->mob), p->mob->position);
    DrawTexture(s->renderedLayers[LAYER_TYPE_FOREGROUND], 0, 0, WHITE);

}
