typedef struct {
    Mobile *mob;
    Vector2 destination;
} MobileMovement;

typedef struct {
    LayerType type;
    char data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer;

typedef struct {
    Tilemap *tilemap;
    Layer *layers[LAYER_COUNT];
    int layerCount;
    Texture2D renderedLayers[LAYER_COUNT];
    ArriveAt *arriveAt[MAX_ARRIVE_AT];
    int arriveAtCount;
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
//    Menu **menus;
//    int menuCount;
    MobileMovement *mobMovements[MAX_MOBILE_MOVEMENTS];
    RuntimeArgs *runtimeArgs;
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

Exploration *createExploration(Log *log, RuntimeArgs *runtimeArgs) {
    Exploration *exploration = malloc(sizeof(Exploration));
    exploration->layerCount = 0;
    exploration->mobileCount = 0;
    exploration->entranceCount = 0;
    exploration->exitCount = 0;
    exploration->objectCount = 0;
    exploration->arriveAtCount = 0;
    exploration->log = log;
    exploration->runtimeArgs = runtimeArgs;
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        exploration->mobMovements[i] = NULL;
    }
    return exploration;
}

MobileMovement *createMobileMovement(Mobile *mob, Vector2 destination) {
    MobileMovement *mobMovement = malloc(sizeof(MobileMovement));
    mobMovement->mob = mob;
    mobMovement->destination = destination;
    return mobMovement;
}

void addMobileMovement(Exploration *e, MobileMovement *mobMovement) {
    addInfo(e->log, "add mob movement, %s target to %f, %f",
            mobMovement->mob->name, mobMovement->destination.x, mobMovement->destination.y);
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        if (e->mobMovements[i] == NULL) {
            e->mobMovements[i] = mobMovement;
            return;
        }
    }
}

Layer *createLayer() {
    Layer *layer = malloc(sizeof(Layer));
    layer->type = -1;
    layer->width = 0;
    layer->height = 0;
    return layer;
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

void explorationDebugKeyPressed(Exploration *e, Vector2 position) {
    addInfo(e->log, "player coordinates: %f, %f", position.x, position.y);
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
    if (e->runtimeArgs->showObjectCollisions) {
        drawObjectCollision(e, layer, index - 1, x, y);
    }
}

void drawWarpCollisions(Exploration *e, Image *image) {
    for (int i = 0; i < e->exitCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (e->exits[i]->area.x),
                (int) (e->exits[i]->area.y),
                (int) (e->exits[i]->area.width),
                (int) (e->exits[i]->area.height),
                WHITE);
    }
    for (int i = 0; i < e->entranceCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (e->entrances[i]->area.x),
                (int) (e->entrances[i]->area.y),
                (int) (e->entrances[i]->area.width),
                (int) (e->entrances[i]->area.height),
                BLACK);
    }
}

void renderExplorationLayer(Exploration *e, LayerType layer) {
    Vector2D sz = e->tilemap->size;
    int width = SCREEN_WIDTH / sz.x;
    int height = SCREEN_HEIGHT / sz.y;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = -1; x < width; x++) {
            if (x >= e->layers[layer]->width || y >= e->layers[layer]->height) {
                continue;
            }
            drawTile(
                    e,
                    renderedLayer,
                    (int) e->layers[layer]->data[y][x],
                    x,
                    y
            );
        }
    }
    if (e->runtimeArgs->showWarpCollisions) {
        drawWarpCollisions(e, &renderedLayer);
    }
    e->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
    UnloadImage(renderedLayer);
}

void unloadLayers(Exploration *e) {
    for (int i = 0; i < e->layerCount; i++) {
        UnloadTexture(e->renderedLayers[i]);
    }
}

void createMobileLayer(Mobile *mobiles[MAX_MOBILES], Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES], int mobileCount, int mobsByYPosition[MAX_LAYERS]) {
    for (int y = 0; y < MAX_LAYERS; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
    for (int i = 0; i < MAX_LAYERS; i++) {
        mobsByYPosition[i] = 0;
    }
    for (int i = 0; i < mobileCount; i++) {
        int y = (int) roundf(mobiles[i]->position.y);
        mobLayer[y][mobsByYPosition[y]] = mobiles[i];
        mobsByYPosition[y]++;
    }
}

void drawExplorationMobiles(Exploration *e, Player *p, Vector2 offset) {
    /**
     * Start by putting mobs on a layer. This is necessary for drawing them in
     * the right order.
     */
    Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES];
    int mobsByYPosition[MAX_LAYERS];
    createMobileLayer(e->mobiles, mobLayer, e->mobileCount, mobsByYPosition);

    /**
     * The player goes on the layer too.
     */
    Mobile *mob = getPartyLeader(p);
    int playerY = (int) roundf(mob->position.y);
    mobLayer[playerY][mobsByYPosition[playerY]] = mob;

    /**
     * Now go through the layer and draw mobs in order.
     */
    for (int y = 0; y < MAX_LAYERS; y++) {
        for (int m = 0; m < MAX_MOBILES; m++) {
            if (mobLayer[y][m] == NULL) {
                break;
            }
            drawAnimation(
                    getMobAnimation(mobLayer[y][m]),
                    (Vector2) {
                            mobLayer[y][m]->position.x + offset.x,
                            floorf(mobLayer[y][m]->position.y + offset.y),
                    }
            );
        }
    }

    if (e->runtimeArgs->showPlayerCollision) {
        DrawRectangle(
                (int) (mob->position.x + offset.x + MOB_COLLISION_WIDTH_OFFSET),
                (int) (mob->position.y + offset.y + MOB_COLLISION_HEIGHT_OFFSET),
                MOB_COLLISION_WIDTH,
                MOB_COLLISION_HEIGHT,
                GREEN);
    }
}

Rectangle getObjectSize(Exploration *e, Object *o, int x, int y) {
    return (Rectangle) {
            (float) (e->tilemap->size.x * x) + o->rect.x,
            (float) (e->tilemap->size.y * y) + o->rect.y,
            o->rect.width,
            o->rect.height,
    };
}

bool isBlockedByMapObject(Exploration *e, Rectangle player) {
    Vector2D tiles = getTileCount(e);
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
        for (int y = -1; y < tiles.y; y++) {
            for (int x = -1; x < tiles.x; x++) {
                int index = (int) e->layers[l]->data[y][x];
                Object *o = getObject(e, index - 1);
                if (o != NULL) {
                    Rectangle c = GetCollisionRec(player, getObjectSize(e, o, x, y));
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

void tryToMove(Exploration *e, Player *p, AnimationType direction, Vector2 pos) {
    Rectangle rect = {
            pos.x + MOB_COLLISION_WIDTH_OFFSET,
            pos.y + MOB_COLLISION_HEIGHT,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT,
    };
    Mobile *mob = getPartyLeader(p);
    if (mob->moving[direction]) {
        if (isBlockedByMapObject(e, rect)) {
            p->blockedBy = NULL;
            return;
        }
        p->blockedBy = getBlockingMob(e, rect);
        if (p->blockedBy != NULL) {
            return;
        }
        mob->position = pos;
        p->engageable = NULL;
    }
}

void evaluateMovement(Exploration *e, Player *p) {
    Mobile *mob = getPartyLeader(p);
    if (mob->isBeingMoved) {
        return;
    }
    addDebug(e->log, "exploration -- evaluate movement -- %f, %f",
             mob->position.x,
             mob->position.y);
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        tryToMove(e, p, DIRECTIONS[i], getMoveFor(mob, DIRECTIONS[i]));
    }
}

void drawExplorationControls(Player *player, ControlBlock *cb[MAX_ACTIVE_CONTROLS], FontStyle *font) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cb[i] != NULL && cb[i]->progress < cb[i]->thenCount) {
            int p = cb[i]->progress;
            if (cb[i]->then[p]->outcome == SPEAK && isSpeakingTo(player, cb[i]->then[p]->target)) {
                drawTextInArea(cb[i]->then[p]->message, drawBottomMenu(), font);
            }
        }
    }
}

void drawNotifications(NotificationManager *nm, FontStyle *font) {
    for (int i = 0; i < nm->count; i++) {
        drawMenuRect(nm->notifications[i]->rect);
        drawTextInArea(nm->notifications[i]->message, nm->notifications[i]->rect, font);
    }
}

void drawExplorationView(
        Exploration *e,
        Player *p,
        NotificationManager *nm,
        ControlBlock *c[MAX_ACTIVE_CONTROLS],
        FontStyle *font) {
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
    drawExplorationMobiles(e, p, offset);
    DrawTextureEx(e->renderedLayers[FOREGROUND], offset, 0, SCALE, WHITE);
    drawNotifications(nm, font);
    drawExplorationControls(p, c, font);
    EndDrawing();
}

void addMobileToExploration(Exploration *exploration, Mobile *mob) {
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

void dialogEngaged(Exploration *exploration, Player *player, ControlBlock *controlBlock) {
    addInfo(exploration->log, "speaking with '%s' mob", player->engageable->name);
    if (controlBlock != NULL) {
        controlBlock->progress++;
        addDebug(exploration->log, "active control block progress at %d", controlBlock->progress);
    }
    if (controlBlock->progress >= controlBlock->thenCount) {
        addDebug(exploration->log, "unsetting active control block");
        controlBlock->progress = 0;
        controlBlock = NULL;
        disengageWithMobile(player);
    }
}

void explorationSpaceKeyPressed(Exploration *exploration, Player *player, ControlBlock *controlBlocks[MAX_ACTIVE_CONTROLS]) {
    addInfo(exploration->log, "exploration space key pressed");
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (controlBlocks[i] != NULL
                && player->engaged
                && isSpeakOutcome(controlBlocks[i]->then[controlBlocks[i]->progress])) {
            dialogEngaged(exploration, player, controlBlocks[i]);
            return;
        }
    }
    if (player->blockedBy != NULL) {
        engageWithMobile(player);
    }
}

void doMobileMovementUpdates(Exploration *exploration) {
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        if (exploration->mobMovements[i] == NULL) {
            continue;
        }
        Mobile *mob = exploration->mobMovements[i]->mob;
        bool moved = moveMob(mob, exploration->mobMovements[i]->destination);
        if (!moved) {
            addInfo(exploration->log, "mob done moving -- %s",
                    exploration->mobMovements[i]->mob->name);
            exploration->mobMovements[i] = NULL;
            mob->isBeingMoved = false;
        }
    }
}
