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
    int mobileCount;
    StorylineData *storylines[MAX_STORIES];
    int storylineCount;
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
    scene->storylineCount = 0;
    scene->mobileCount = 0;
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
        scene->controlBlocks[i] = NULL;
    }
    return scene;
}

void addMobile(Scene *scene, Mobile *mob) {
    scene->mobiles[scene->mobileCount] = mob;
    scene->mobileCount++;
}

void addStoryline(Scene *scene, StorylineData *storyline) {
    scene->storylines[scene->storylineCount] = storyline;
    scene->storylineCount++;
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
    if (cb != NULL && cb->progress < cb->thenCount) {
        int p = cb->progress;
        if (cb->then[p]->outcome == OUTCOME_SPEAK && isSpeakingTo(player, cb->then[p]->target)) {
            drawDialogBox(cb->then[p]->message);
        }
    }
}

void controlThen(Scene *s, Player *p) {
    if (s->activeControlBlock == NULL || s->activeControlBlock->progress > s->activeControlBlock->thenCount) {
        return;
    }
    ControlBlock *cb = s->activeControlBlock;
    if (cb->then[cb->progress]->outcome == OUTCOME_ADD_STORY) {
        printf("add story %s\n", cb->then[cb->progress]->story);
        addStory(p, cb->then[cb->progress]->story);
        cb->progress++;
    }
}

int needsEngagedAndIsNot(int condition, Player *p, Mobile *mobileTrigger) {
    return condition == CONDITION_ENGAGED && !isSpeakingTo(p, mobileTrigger);
}

int needsStoryAndMissing(int condition, Player *p, char *story) {
    return condition == CONDITION_HAS_STORY && !hasStory(p, story);
}

int needsNotHaveStoryAndPresent(int condition, Player *p, char *story) {
    return condition == CONDITION_NOT_HAS_STORY && hasStory(p, story);
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
        int matched = true;
        for (int c = 0; c < cb->whenCount; c++) {
            if (needsEngagedAndIsNot(cb->when[c]->condition, p, cb->when[c]->mobileTrigger)
                || needsStoryAndMissing(cb->when[c]->condition, p, cb->when[c]->story)
                || needsNotHaveStoryAndPresent(cb->when[c]->condition, p, cb->when[c]->story)) {
                matched = false;
            }
            if (!matched) {
                break;
            }
        }
        if (matched) {
            s->activeControlBlock = cb;
            printf("set active control block %d, progress %d\n", i, s->activeControlBlock->progress);
            return;
        }
    }
    printf("done control check\n");
}

void activeControlRemoveCheck(Scene *s) {
    if (s->activeControlBlock != NULL && s->activeControlBlock->progress >= s->activeControlBlock->thenCount) {
        printf("unset active control block %d\n", s->activeControlBlock->control);
        s->activeControlBlock->progress = 0;
        s->activeControlBlock = NULL;
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

void createMobileLayer(Mobile *mobLayer[MAX_LAYER_SIZE][MAX_MOBILES]) {
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
}

void drawMobiles(Scene *s, Player *p) {
    /**
     * Start by putting mobs on a layer. This is necessary for drawing them in
     * the right order.
     */
    Mobile *mobLayer[MAX_LAYER_SIZE][MAX_MOBILES];
    createMobileLayer(mobLayer);
    int count[MAX_LAYER_SIZE];
    for (int i = 0; i < MAX_LAYER_SIZE; i++) {
        count[i] = 0;
    }
    for (int i = 0; i < s->mobileCount; i++) {
        int y = (int) s->mobiles[i]->position.y / s->tilemap->size.y;
        mobLayer[y][count[y]] = s->mobiles[i];
        count[y]++;
    }

    /**
     * The player goes on the layer too.
     */
    int playerY = (int) p->mob->position.y / s->tilemap->size.y;
    mobLayer[playerY][count[playerY]] = p->mob;

    /**
     * Now go through the layer and draw mobs in order.
     */
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int m = 0; m < MAX_MOBILES; m++) {
            if (mobLayer[y][m] == NULL) {
                break;
            }
            drawAnimation(getMobAnimation(mobLayer[y][m]), mobLayer[y][m]->position);
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
        p->engageable = NULL;
    }
    if (p->moving.down == 1 && !isBlocked(s, p, (Vector2){pos.x, pos.y + 1})) {
        p->mob->position.y += 1;
        p->engageable = NULL;
    }
    if (p->moving.left == 1 && !isBlocked(s, p, (Vector2){pos.x - 1, pos.y})) {
        p->mob->position.x -= 1;
        p->engageable = NULL;
    }
    if (p->moving.right == 1 && !isBlocked(s, p, (Vector2){pos.x + 1, pos.y})) {
        p->mob->position.x += 1;
        p->engageable = NULL;
    }
}


void checkInput(Scene *s, Player *p) {
    resetMoving(p);
    getMobAnimation(p->mob)->isPlaying = 0;
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
    if (IsKeyDown(KEY_C)) {
        printf("player coordinates: %f, %f\n", p->mob->position.x, p->mob->position.y);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        printf("======================\nspace key pressed\n");
        if (s->activeControlBlock != NULL) {
            printf("active control block progress: %d\n", s->activeControlBlock->progress);
        } else {
            printf("no active control blocks set\n");
        }
        if (p->engaged && s->activeControlBlock == NULL) {
            printf("player is engaged with no active control blocks\n");
            return;
        }
        if (p->engaged && s->activeControlBlock->then[s->activeControlBlock->progress]->outcome == OUTCOME_SPEAK) {
            if (s->activeControlBlock != NULL) {
                s->activeControlBlock->progress++;
                printf("active control block progress at %d\n", s->activeControlBlock->progress);
            }
            if (s->activeControlBlock->progress >= s->activeControlBlock->thenCount
                    || s->activeControlBlock->then[s->activeControlBlock->progress]->outcome != OUTCOME_SPEAK) {
                printf("unset engaged\n");
                p->engaged = false;
            }
            if (s->activeControlBlock->progress >= s->activeControlBlock->thenCount) {
                printf("unsetting active control block\n");
                s->activeControlBlock->progress = 0;
                s->activeControlBlock = NULL;
            }
        } else if (p->blockedBy != NULL) {
            p->engageable = p->blockedBy;
            printf("engaging with %s\n", p->engageable->name);
            p->engaged = true;
        }
    }
}
