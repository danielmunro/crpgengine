typedef struct Exit {
    Rectangle area;
    char *to;
    int x;
    int y;
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
    char *music;
    int showCollisions;
    Rectangle entrance;
    Exit *exits[MAX_EXITS];
    int nextExit;
    Mobile *mobiles[MAX_MOBILES];
    ControlBlockInt *controlBlocksInt[MAX_CONTROLS];
    ControlBlock *controlBlocks[MAX_CONTROLS];
    ControlBlock *activeControlBlock;
} Scene;

const SceneType sceneTypes[] = {
    {SCENE_TYPE_TOWN,    "town"},
    {SCENE_TYPE_DUNGEON, "dungeon"},
};

Scene *createScene() {
    Scene *scene = malloc(sizeof(Scene));
    scene->layerCount = 0;
    scene->nextExit = 0;
    scene->activeControlBlock = NULL;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        scene->objects[i] = NULL;
    }
    for (int i = 0; i < MAX_EXITS; i++) {
        scene->exits[i] = NULL;
    }
    for (int i = 0; i < MAX_MOBILES; i++) {
        scene->mobiles[i] = NULL;
    }
    for (int i = 0; i < MAX_CONTROLS; i++) {
        scene->controlBlocksInt[i] = NULL;
    }
    for (int i = 0; i < MAX_CONTROLS; i++) {
        scene->controlBlocks[i] = NULL;
    }
    return scene;
}

Object *getObject(Scene *s, int index) {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (s->objects[i] == NULL) {
            return NULL;
        }
        if (s->objects[i]->tile == index) {
            return s->objects[i];
        }
    }
    return NULL;
}

Vector2d getTileCount(Scene *s) {
    int x = SCREEN_WIDTH / s->tilemap->size.x + 1;
    int y = SCREEN_HEIGHT / s->tilemap->size.y + 2;
    return (Vector2d){x, y};
}

void drawControl(Player *player, ControlBlock *cb) {
    if (cb != NULL) {
        int p = cb->progress;
        if (cb->then[p]->outcome == OUTCOME_SPEAK && isSpeakingTo(player, cb->then[p]->target)) {
            drawDialogBox(cb->then[p]->message);
        }
    }
}

void controlWhenCheck(Scene *s, Player *p) {
    if (s->activeControlBlock != NULL) {
        return;
    }
    for (int i = 0; i < MAX_CONTROLS; i++) {
        if (s->controlBlocks[i] == NULL) {
            return;
        }
        ControlBlock *cb = s->controlBlocks[i];
        for (int c = 0; c < cb->whenCount; c++) {
            if (cb->when[c]->condition == CONDITION_ENGAGED &&
                    isSpeakingTo(p, cb->when[c]->mobileTrigger)) {
                s->activeControlBlock = cb;
                printf("set active control block %d\n", i);
            }
        }
    }
}

void drawLayer(Scene *s, int layer) {
    Vector2d tiles = getTileCount(s);
    Vector2d sz = s->tilemap->size;
    Image renderedLayer = GenImageColor(tiles.x * sz.x, tiles.y * sz.y, BLANK);
    for (int y = -1; y < tiles.y; y++) {
        for (int x = -1; x < tiles.x; x++) {
            int index = (int) s->layers[layer]->data[y][x];
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
            if (s->showCollisions) {
                Object *o = getObject(s, index - 1);
                if (o != NULL) {
                    Rectangle r = {
                            (float) (sz.x * x) + o->rect.x,
                            (float) (sz.y * y) + o->rect.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    ImageDrawRectangle(
                            &renderedLayer,
                            (int) r.x,
                            (int) r.y,
                            (int) r.width,
                            (int) r.height,
                            PINK
                    );
                }
            }
        }
    }
    s->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
}

void drawScene(Scene *s) {
    ClearBackground(BLACK);
    drawLayer(s, LAYER_TYPE_BACKGROUND);
    drawLayer(s, LAYER_TYPE_MIDGROUND);
    drawLayer(s, LAYER_TYPE_FOREGROUND);
}

void drawMobiles(Scene *s, Player *p) {
    Mobile *mobsByY[MAX_LAYER_SIZE][MAX_MOBILES];
    int indices[MAX_LAYER_SIZE];
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobsByY[y][i] = NULL;
        }
    }
    for (int i = 0; i < MAX_LAYER_SIZE; i++) {
        indices[i] = 0;
    }
    for (int i = 0; i < MAX_MOBILES; i++) {
        if (s->mobiles[i] == NULL) {
            break;
        }
        int y = (int) s->mobiles[i]->position.y / s->tilemap->size.y;
        mobsByY[y][indices[y]] = s->mobiles[i];
        indices[y]++;
    }
    int playerY = (int) p->mob->position.y / s->tilemap->size.y;
    mobsByY[playerY][indices[playerY]] = p->mob;
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int m = 0; m < MAX_MOBILES; m++) {
            if (mobsByY[y][m] == NULL) {
                break;
            }
            drawAnimation(getMobAnimation(mobsByY[y][m]), mobsByY[y][m]->position);
        }
    }
}

void renderScene(Scene *s, Player *p) {
    ClearBackground(BLACK);
    DrawTexture(s->renderedLayers[LAYER_TYPE_BACKGROUND], 0, 0, WHITE);
    DrawTexture(s->renderedLayers[LAYER_TYPE_MIDGROUND], 0, 0, WHITE);
    drawMobiles(s, p);
    DrawTexture(s->renderedLayers[LAYER_TYPE_FOREGROUND], 0, 0, WHITE);
}

int isBlocked(Scene *s, Player *p, Vector2 pos) {
    p->engageable = NULL;
    Rectangle pRect = {
            pos.x,
            pos.y + MOB_COLLISION_HEIGHT_OFFSET,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT
    };
    Vector2d tiles = getTileCount(s);
    for (int l = 0; l < LAYER_COUNT; l++) {
        for (int y = -1; y < tiles.y; y++) {
            for (int x = -1; x < tiles.x; x++) {
                int index = (int) s->layers[l]->data[y][x];
                Object *o = getObject(s, index - 1);
                if (o != NULL) {
                    Rectangle oRect = {
                            (float) (s->tilemap->size.x * x) + o->rect.x,
                            (float) (s->tilemap->size.y * y) + o->rect.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    Rectangle c = GetCollisionRec(pRect, oRect);
                    if (c.height > 0 || c.width > 0) {
                        return 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < MAX_MOBILES; i++) {
        if (s->mobiles[i] == NULL) {
            break;
        }
        Rectangle mRect = {
                s->mobiles[i]->position.x,
                s->mobiles[i]->position.y + MOB_COLLISION_HEIGHT_OFFSET,
                MOB_COLLISION_WIDTH,
                MOB_COLLISION_HEIGHT,
        };
        Rectangle c = GetCollisionRec(pRect, mRect);
        if (c.height > 0 || c.width > 0) {
            p->blockedBy = s->mobiles[i];
            return 1;
        }
    }
    return 0;
}

int atExit(Scene *s, Player *p) {
    for (int i = 0; i < MAX_EXITS; i++) {
        if (s->exits[i] == NULL) {
            return -1;
        }
        Rectangle pRect = {
                p->mob->position.x,
                p->mob->position.y + 12,
                16,
                12,
        };
        Rectangle c = GetCollisionRec(s->exits[i]->area, pRect);
        if (c.height > 0 || c.width > 0) {
            return i;
        }
    }
    return -1;
}

void evaluateMovement(Scene *s, Player *p) {
    Vector2 pos = p->mob->position;
    if (p->moving.up == 1 && !isBlocked(s, p, (Vector2){pos.x, pos.y - 1})) {
        p->mob->position.y -= 1;
    }
    if (p->moving.down == 1 && !isBlocked(s, p, (Vector2){pos.x, pos.y + 1})) {
        p->mob->position.y += 1;
    }
    if (p->moving.left == 1 && !isBlocked(s, p, (Vector2){pos.x - 1, pos.y})) {
        p->mob->position.x -= 1;
    }
    if (p->moving.right == 1 && !isBlocked(s, p, (Vector2){pos.x + 1, pos.y})) {
        p->mob->position.x += 1;
    }
}
