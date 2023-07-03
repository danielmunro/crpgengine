typedef struct {
    Rectangle area;
    char *to;
    char *scene;
} Exit;

typedef struct {
    char *name;
    Rectangle area;
    AnimationDirection direction;
} Entrance;

typedef struct {
    LayerType type;
    char data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    bool showCollisions;
} Layer;

typedef struct {
    Tilemap *tilemap;
    Layer *layers[LAYER_COUNT];
    int layerCount;
    Texture2D renderedLayers[LAYER_COUNT];
    Exit *exits[MAX_EXITS];
    int exitCount;
    Entrance *entrances[MAX_ENTRANCES];
    int entranceCount;
    Object *objects[MAX_OBJECTS];
    int objectCount;
    Log *log;
    bool showCollisions;
    Mobile *mobiles[MAX_MOBILES];
    int mobileCount;
    Menu *menus[MAX_MENUS];
    int menuCount;
} Exploration;

Exit *createExit() {
    Exit *e = malloc(sizeof(Exit));
    e->to = "";
    e->scene = "";
    e->area = (Rectangle){0, 0, 0, 0};
    return e;
}

Entrance *createEntrance(char *name, Rectangle area) {
    Entrance *e = malloc(sizeof(Entrance));
    e->name = &name[0];
    e->direction = DOWN;
    e->area = area;
    return e;
}

Entrance *findEntrance(Exploration *e, char *name) {
    for (int i = 0; i < e->entranceCount; i++) {
        if (strcmp(e->entrances[i]->name, name) == 0) {
            return e->entrances[i];
        }
    }
    addError(e->log, "entrance not found: %s", name);
    return NULL;
}

Exploration *createExploration(Log *log, int showCollisions) {
    Exploration *exploration = malloc(sizeof(Exploration));
    exploration->layerCount = 0;
    exploration->showCollisions = showCollisions;
    exploration->mobileCount = 0;
    exploration->entranceCount = 0;
    exploration->exitCount = 0;
    exploration->menuCount = 0;
    exploration->objectCount = 0;
    exploration->log = log;
    return exploration;
}

Object *getObject(Exploration *e, int index) {
    for (int i = 0; i < e->objectCount; i++) {
        if (e->objects[i]->tile == index) {
            return e->objects[i];
        }
    }
    return NULL;
}

void addObject(Exploration *e, Object *o) {
    e->objects[e->objectCount] = o;
    e->objectCount++;
}

Vector2D getTileCount(Exploration *e) {
    int x = SCREEN_WIDTH / e->tilemap->size.x + 1;
    int y = SCREEN_HEIGHT / e->tilemap->size.y + 2;
    return (Vector2D) {x, y};
}

void addMenu(Exploration *e, Menu *m) {
    addInfo(e->log, "adding in-game menu %d", m->type);
    e->menus[e->menuCount] = m;
    e->menuCount++;
}

Menu *getCurrentMenu(Exploration *e) {
    if (e->menuCount < 1) {
        return NULL;
    }
    return e->menus[e->menuCount - 1];
}

void explorationDebugKeyPressed(Exploration *e, Vector2 position) {
    addDebug(e->log, "player coordinates: %f, %f", position.x, position.y);
}

void drawObjectCollision(Exploration *e, Image layer, int index, int x, int y) {
    Object *o = getObject(e, index);
    if (o != NULL) {
        Rectangle r = {
                (float) (e->tilemap->size.x * x) + o->rect.x,
                (float) (e->tilemap->size.y * y) + o->rect.y,
                o->rect.width,
                o->rect.height,
        };
        ImageDrawRectangle(
                &layer,
                (int) r.x,
                (int) r.y,
                (int) r.width,
                (int) r.height,
                PINK
        );
    }
}

void drawTile(Exploration *e, Image layer, int index, int x, int y) {
    if (index <= 0) {
        return;
    }
    Vector2D sz = e->tilemap->size;
    Vector2 pos = {
            (float) (sz.x * x),
            (float) (sz.y * y),
    };
    Rectangle rect = getRectForTile(e->tilemap, index);
    ImageDraw(
            &layer,
            e->tilemap->source,
            rect,
            (Rectangle) {pos.x, pos.y, (float) sz.x, (float) sz.y},
            WHITE
    );
    if (e->showCollisions) {
        drawObjectCollision(e, layer, index - 1, x, y);
    }
}

void renderExplorationLayer(Exploration *e, int layer) {
    Vector2D sz = e->tilemap->size;
    int width = SCREEN_WIDTH / sz.x;
    int height = SCREEN_HEIGHT / sz.y;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = -1; x < width; x++) {
            drawTile(
                    e,
                    renderedLayer,
                    (int) e->layers[layer]->data[y][x],
                    x,
                    y
            );
        }
    }
    e->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
}

void createMobileLayer(Mobile *mobLayer[MAX_LAYER_SIZE][MAX_MOBILES]) {
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
}

void drawExplorationMobiles(int mobileCount, Mobile *mobiles[MAX_MOBILES], Player *p, Vector2 offset) {
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
    for (int i = 0; i < mobileCount; i++) {
        int y = (int) mobiles[i]->position.y;
        mobLayer[y][count[y]] = mobiles[i];
        count[y]++;
    }

    /**
     * The player goes on the layer too.
     */
    Mobile *mob = getPartyLeader(p);
    int playerY = (int) mob->position.y;
    mobLayer[playerY][count[playerY]] = mob;

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

bool isBlockedByMapObject(Exploration *e, Rectangle playerRect) {
    Vector2D tiles = getTileCount(e);
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
        for (int y = -1; y < tiles.y; y++) {
            for (int x = -1; x < tiles.x; x++) {
                int index = (int) e->layers[l]->data[y][x];
                Object *o = getObject(e, index - 1);
                if (o != NULL) {
                    Rectangle oRect = {
                            (float) (e->tilemap->size.x * x) + o->rect.x,
                            (float) (e->tilemap->size.y * y) + o->rect.y,
                            o->rect.width,
                            o->rect.height,
                    };
                    Rectangle c = GetCollisionRec(playerRect, oRect);
                    if (c.height > 0 || c.width > 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Mobile *getBlockingMob(Exploration *e, Rectangle playerRect) {
    for (int i = 0; i < e->mobileCount; i++) {
        Rectangle c = GetCollisionRec(playerRect, getMobileRectangle(e->mobiles[i]));
        if (c.height > 0 || c.width > 0) {
            return e->mobiles[i];
        }
    }
    return NULL;
}

int atExit(Exploration *e, Player *p) {
    Mobile *mob = getPartyLeader(p);
    Rectangle rect = getMobileRectangle(mob);
    for (int i = 0; i < e->exitCount; i++) {
        Rectangle c = GetCollisionRec(e->exits[i]->area, rect);
        if (c.height > 0 || c.width > 0) {
            return i;
        }
    }
    return -1;
}

void tryToMove(Exploration *e, Player *p, AnimationDirection direction, Vector2 pos) {
    Rectangle rect = {
            pos.x,
            pos.y + MOB_COLLISION_HEIGHT,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT,
    };
    if (p->moving[direction]) {
        if (isBlockedByMapObject(e, rect)) {
            p->blockedBy = NULL;
            return;
        }
        p->blockedBy = getBlockingMob(e, rect);
        if (p->blockedBy != NULL) {
            return;
        }
        getPartyLeader(p)->position = pos;
        p->engageable = NULL;
    }
}

void evaluateMovement(Exploration *e, Player *p) {
    Mobile *mob = getPartyLeader(p);
    addDebug(e->log, "exploration -- evaluate movement -- %f, %f",
             mob->position.x,
             mob->position.y);
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        tryToMove(e, p, DIRECTIONS[i], getMoveFor(mob, DIRECTIONS[i]));
    }
}

void drawExplorationControls(Player *player, ControlBlock *cb) {
    if (cb != NULL && cb->progress < cb->thenCount) {
        int p = cb->progress;
        if (cb->then[p]->outcome == SPEAK && isSpeakingTo(player, cb->then[p]->target)) {
            drawDialogBox(cb->then[p]->message);
        }
    }
}

void drawExplorationView(Exploration *e, Player *p, ControlBlock *c) {
    addDebug(e->log, "exploration -- draw");
    Mobile *mob = getPartyLeader(p);
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 offset = {
            ((float) SCREEN_WIDTH / 2) - mob->position.x,
            ((float) SCREEN_HEIGHT / 2) - mob->position.y
    };
    DrawTextureEx(e->renderedLayers[BACKGROUND], offset, 0, SCALE, WHITE);
    DrawTextureEx(e->renderedLayers[MIDGROUND], offset, 0, SCALE, WHITE);
    drawExplorationMobiles(e->mobileCount, e->mobiles, p, offset);
    DrawTextureEx(e->renderedLayers[FOREGROUND], offset, 0, SCALE, WHITE);
    drawExplorationControls(p, c);
    EndDrawing();
}

void addMobile(Exploration *exploration, Mobile *mob) {
    exploration->mobiles[exploration->mobileCount] = mob;
    exploration->mobileCount++;
}

void renderExplorationLayers(Exploration *e) {
    ClearBackground(BLACK);
    renderExplorationLayer(e, BACKGROUND);
    renderExplorationLayer(e, MIDGROUND);
    renderExplorationLayer(e, FOREGROUND);
    addDebug(e->log, "exploration successfully rendered");
}

void removeMenu(Exploration *exploration) {
    exploration->menuCount--;
    exploration->menus[exploration->menuCount]->cursor = 0;
    exploration->menus[exploration->menuCount] = NULL;
}

void explorationSpaceKeyPressed(Exploration *exploration, Player *player, ControlBlock *controlBlock) {
    addInfo(exploration->log, "space key pressed");
    if (controlBlock != NULL) {
        addDebug(exploration->log, "active control block progress: %d", controlBlock->progress);
    } else {
        addDebug(exploration->log, "no active control blocks set");
    }
    if (player->engaged && controlBlock == NULL) {
        player->engaged = false;
        return;
    }
    if (player->engaged && controlBlock->then[controlBlock->progress]->outcome == SPEAK) {
        if (controlBlock != NULL) {
            controlBlock->progress++;
            addDebug(exploration->log, "active control block progress at %d", controlBlock->progress);
        }
        if (controlBlock->progress >= controlBlock->thenCount
            || controlBlock->then[controlBlock->progress]->outcome != SPEAK) {
            addDebug(exploration->log, "unset engaged");
            player->engaged = false;
        }
        if (controlBlock->progress >= controlBlock->thenCount) {
            addDebug(exploration->log, "unsetting active control block");
            controlBlock->progress = 0;
            controlBlock = NULL;
        }
    } else if (player->blockedBy != NULL) {
        player->engageable = player->blockedBy;
        addInfo(exploration->log, "engaging with %s", player->engageable->name);
        player->engaged = true;
    }
}
