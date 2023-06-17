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
    char *name;
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
    Encounters *encounters;
    Fight *fight;
    Log *log;
} Scene;

const SceneType sceneTypes[] = {
    {SCENE_TYPE_TOWN,    "town"},
    {SCENE_TYPE_DUNGEON, "dungeon"},
};

void setSceneTypeFromString(Scene *s, const char *sceneType) {
    int count = sizeof(sceneTypes) / sizeof(SceneType);
    for (int i = 0; i < count; i++) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            s->type = sceneTypes[i].code;
            addDebug(s->log, "scene '%s' type set to '%s'", s->name, sceneType);
            return;
        }
    }
    addError(s->log, "scene type not found: %s, setting to default of 'town'", sceneType);
    s->type = SCENE_TYPE_TOWN;
}

Scene *createScene(Log *log) {
    Scene *scene = malloc(sizeof(Scene));
    scene->layerCount = 0;
    scene->nextExit = 0;
    scene->storylineCount = 0;
    scene->mobileCount = 0;
    scene->activeControlBlock = NULL;
    scene->fight = NULL;
    scene->encounters = createEncounters();
    scene->log = log;
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

int isFighting(Scene *s) {
    return s->fight != NULL;
}

int isDungeon(Scene *s) {
    return s->type == SCENE_TYPE_DUNGEON;
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
        if (cb->then[p]->outcome == SPEAK && isSpeakingTo(player, cb->then[p]->target)) {
            drawDialogBox(cb->then[p]->message);
        }
    }
}

void controlThenCheck(Scene *s, Player *p) {
    if (s->activeControlBlock == NULL || s->activeControlBlock->progress > s->activeControlBlock->thenCount) {
        return;
    }
    ControlBlock *cb = s->activeControlBlock;
    if (cb->then[cb->progress]->outcome == ADD_STORY) {
        addDebug(s->log, "add storyline '%s' for player", cb->then[cb->progress]->story);
        addStory(p, cb->then[cb->progress]->story);
        cb->progress++;
    }
}

int needsEngagedAndIsNot(int condition, Player *p, Mobile *mobileTrigger) {
    return condition == ENGAGED && !isSpeakingTo(p, mobileTrigger);
}

int needsStoryAndMissing(int condition, Player *p, char *story) {
    return condition == HAS_STORY && !hasStory(p, story);
}

int needsNotHaveStoryAndPresent(int condition, Player *p, char *story) {
    return condition == NOT_HAS_STORY && hasStory(p, story);
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
            addDebug(s->log, "set active control block %d, progress %d", i, s->activeControlBlock->progress);
            return;
        }
    }
    addDebug(s->log, "done control check");
}

void activeControlRemoveCheck(Scene *s) {
    if (s->activeControlBlock != NULL && s->activeControlBlock->progress >= s->activeControlBlock->thenCount) {
        addDebug(s->log, "unset active control block");
        s->activeControlBlock->progress = 0;
        s->activeControlBlock = NULL;
    }
}

void drawLayer(Scene *s, int layer) {
    int width = 56;
    int height = 200;
    Vector2d sz = s->tilemap->size;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = -1; x < width; x++) {
            int index = (int) s->layers[layer]->data[y][x];
            if (index <= 0) {
                continue;
            }
            Vector2 pos = {
                    (float) (sz.x * x),
                    (float) (sz.y * y),
            };
            Rectangle rect = getRectForTile(s->tilemap, index);
            ImageDraw(
                    &renderedLayer,
                    s->tilemap->source,
                    rect,
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
    addDebug(s->log, "scene '%s' layer %d drawing", s->name, BACKGROUND);
    drawLayer(s, BACKGROUND);
    addDebug(s->log, "scene '%s' layer %s drawn", s->name, BACKGROUND);
    drawLayer(s, MIDGROUND);
    drawLayer(s, FOREGROUND);
    addDebug(s->log, "scene '%s' successfully drawn", s->name);
}

void createMobileLayer(Mobile *mobLayer[MAX_LAYER_SIZE][MAX_MOBILES]) {
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
}

void drawMobiles(Scene *s, Player *p, Vector2 offset) {
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
            Vector2 position = {
                    mobLayer[y][m]->position.x + offset.x,
                    mobLayer[y][m]->position.y + offset.y,
            };
            drawAnimation(getMobAnimation(mobLayer[y][m]), position);
        }
    }
}

void renderFight(Scene *s, Player *p) {
    float scale = (float) SCREEN_WIDTH / (float) s->encounters->background.width;
    DrawTextureEx(s->encounters->background, (Vector2) {0, 0}, 0, scale, WHITE);
    int width = 0;
    int height = 0;
    for (int i = 0; i < s->fight->beastCount; i++) {
        const int x = i % 3;
        DrawTextureEx(s->fight->beasts[i]->image,
                      (Vector2) {(float) width, (float) height},
                      0, SCALE, WHITE);
        width += s->fight->beasts[i]->image.width;
        if (x > 0 && x % 2 == 0) {
            height += s->fight->beasts[i]->image.height;
            width = 0;
        }
    }
    drawAnimation(findAnimation(p->mob->animations, LEFT), (Vector2) {SCREEN_WIDTH * .8, 100 });
}

void renderDefaultView(Scene *s, Player *p) {
    Vector2 offset = {
            ((float) SCREEN_WIDTH / 2) - p->mob->position.x,
            ((float) SCREEN_HEIGHT / 2) - p->mob->position.y
    };
    DrawTextureEx(s->renderedLayers[BACKGROUND], offset, 0, SCALE, WHITE);
    DrawTextureEx(s->renderedLayers[MIDGROUND], offset, 0, SCALE, WHITE);
    drawMobiles(s, p, offset);
    DrawTextureEx(s->renderedLayers[FOREGROUND], offset, 0, SCALE, WHITE);
}

void renderScene(Scene *s, Player *p) {
    ClearBackground(BLACK);
    if (isFighting(s)) {
        renderFight(s, p);
    } else {
        renderDefaultView(s, p);
    }
}

void drawScreen(Scene *s, Player *p) {
    renderScene(s, p);
    drawControl(p, s->activeControlBlock);
}

int isBlocked(Scene *s, Player *p, Vector2 pos) {
    Rectangle pRect = {
            pos.x,
            pos.y + MOB_COLLISION_HEIGHT_OFFSET,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT
    };
    Vector2d tiles = getTileCount(s);
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
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
    if (isFighting(s)) {
        return;
    }
    getMobAnimation(p->mob)->isPlaying = 0;
    checkMoveKey(
            p,
            KEY_UP,
            UP
    );
    checkMoveKey(
            p,
            KEY_DOWN,
            DOWN
    );
    checkMoveKey(
            p,
            KEY_RIGHT,
            RIGHT
    );
    checkMoveKey(
            p,
            KEY_LEFT,
            LEFT
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
        if (p->engaged && s->activeControlBlock->then[s->activeControlBlock->progress]->outcome == SPEAK) {
            if (s->activeControlBlock != NULL) {
                s->activeControlBlock->progress++;
                printf("active control block progress at %d\n", s->activeControlBlock->progress);
            }
            if (s->activeControlBlock->progress >= s->activeControlBlock->thenCount
                    || s->activeControlBlock->then[s->activeControlBlock->progress]->outcome != SPEAK) {
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

void checkToInitiateFight(Scene *s, Player *p) {
    if (!isDungeon(s) || isFighting(s) || !isMoving(p)) {
        return;
    }
    int chance = rand() % 100 + 1;
    if (chance == 1) {
        Beast *beasts[MAX_BEASTS_IN_FIGHT];
        int beastsToCreate = rand() % MAX_BEASTS_IN_FIGHT + 1;
        if (beastsToCreate > MAX_BEASTS_IN_FIGHT) {
            beastsToCreate = MAX_BEASTS_IN_FIGHT;
        }
        int created = 0;
        while (created < beastsToCreate) {
            int e = rand() % s->encounters->beastEncountersCount + 0;
            int max = s->encounters->beastEncounters[e]->max;
            int amount = rand() % max + 1;
            if (amount > beastsToCreate) {
                amount = beastsToCreate;
            }
            for (int i = 0; i < amount; i++) {
                beasts[created] = cloneBeast(s->encounters->beastEncounters[e]->beast);
                created++;
            }
        }
        s->fight = createFight(beasts);
        s->fight->beastCount = created;
        addDebug(s->log, "fight encountered with %d opponents", s->fight->beastCount);
    }
}

void checkControls(Scene *s, Player *p) {
    controlWhenCheck(s, p);
    controlThenCheck(s, p);
    activeControlRemoveCheck(s);
}

void checkFights(Scene *s, Player *p) {
    checkToInitiateFight(s, p);
}
