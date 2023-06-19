typedef struct {
    Rectangle area;
    char *to;
    int x;
    int y;
} Exit;

typedef struct {
    int type;
    char data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int showCollisions;
} Layer;

typedef struct {
    Tilemap *tilemap;
    Layer *layers[LAYER_COUNT];
    int layerCount;
    Texture2D renderedLayers[LAYER_COUNT];
    Rectangle entrance;
    Exit *exits[MAX_EXITS];
    Object *objects[MAX_OBJECTS];
    Log *log;
    int showCollisions;
    Mobile *mobiles[MAX_MOBILES];
    int mobileCount;
} Exploration;

Exit *createExit() {
    Exit *e = malloc(sizeof(Exit));
    e->x = 0;
    e->y = 0;
    return e;
}

Exploration *createExploration(Log *log, int showCollisions) {
    Exploration *exploration = malloc(sizeof(Exploration));
    exploration->layerCount = 0;
    exploration->showCollisions = showCollisions;
    for (int i = 0; i < MAX_OBJECTS; i++) {
        exploration->objects[i] = NULL;
    }
    for (int i = 0; i < MAX_EXITS; i++) {
        exploration->exits[i] = NULL;
    }
    exploration->mobileCount = 0;
    for (int i = 0; i < MAX_MOBILES; i++) {
        exploration->mobiles[i] = NULL;
    }
    exploration->log = log;
    return exploration;
}

Vector2d getTileCount(Exploration *e) {
    int x = SCREEN_WIDTH / e->tilemap->size.x + 1;
    int y = SCREEN_HEIGHT / e->tilemap->size.y + 2;
    return (Vector2d){x, y};
}

void checkExplorationInput(Exploration *exploration, Player *player, ControlBlock *controlBlock) {
    resetMoving(player);
    getMobAnimation(player->mob)->isPlaying = 0;
    checkMoveKey(
            player,
            KEY_UP,
            UP
    );
    checkMoveKey(
            player,
            KEY_DOWN,
            DOWN
    );
    checkMoveKey(
            player,
            KEY_RIGHT,
            RIGHT
    );
    checkMoveKey(
            player,
            KEY_LEFT,
            LEFT
    );
    if (IsKeyDown(KEY_C)) {
        addDebug(exploration->log, "player coordinates: %f, %f",
                 player->mob->position.x, player->mob->position.y);
    }
    if (IsKeyPressed(KEY_SPACE)) {
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
}


void renderExplorationLayer(Tilemap *tilemap, Object *objects[255], Layer *layers[3], Texture2D renderedLayers[3], int showCollisions, int layer) {
    int width = 56;
    int height = 200;
    Vector2d sz = tilemap->size;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = -1; x < width; x++) {
            int index = (int) layers[layer]->data[y][x];
            if (index <= 0) {
                continue;
            }
            Vector2 pos = {
                    (float) (sz.x * x),
                    (float) (sz.y * y),
            };
            Rectangle rect = getRectForTile(tilemap, index);
            ImageDraw(
                    &renderedLayer,
                    tilemap->source,
                    rect,
                    (Rectangle) { pos.x, pos.y, (float) sz.x, (float) sz.y },
                    WHITE
            );
            if (showCollisions) {
                Object *o = getObject(objects, index - 1);
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
    renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
}

void createMobileLayer(Mobile *mobLayer[MAX_LAYER_SIZE][MAX_MOBILES]) {
    for (int y = 0; y < MAX_LAYER_SIZE; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
}

void drawExplorationMobiles(int mobileCount, Mobile *mobiles[255], Player *p, Vector2 offset) {
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
//        int y = (int) mobiles[i]->position.y / tilemap->size.y;
        int y = (int) mobiles[i]->position.y;
        mobLayer[y][count[y]] = mobiles[i];
        count[y]++;
    }

    /**
     * The player goes on the layer too.
     */
//    int playerY = (int) p->mob->position.y / s->tilemap->size.y;
    int playerY = (int) p->mob->position.y;
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

int isBlocked(Exploration *e, Player *p, Vector2 pos) {
    Rectangle pRect = {
            pos.x,
            pos.y + MOB_COLLISION_HEIGHT_OFFSET,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT
    };
    Vector2d tiles = getTileCount(e);
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
        for (int y = -1; y < tiles.y; y++) {
            for (int x = -1; x < tiles.x; x++) {
                int index = (int) e->layers[l]->data[y][x];
                Object *o = getObject(e->objects, index - 1);
                if (o != NULL) {
                    Rectangle oRect = {
                            (float) (e->tilemap->size.x * x) + o->rect.x,
                            (float) (e->tilemap->size.y * y) + o->rect.y,
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
        if (e->mobiles[i] == NULL) {
            break;
        }
        Rectangle mRect = {
                e->mobiles[i]->position.x,
                e->mobiles[i]->position.y + MOB_COLLISION_HEIGHT_OFFSET,
                MOB_COLLISION_WIDTH,
                MOB_COLLISION_HEIGHT,
        };
        Rectangle c = GetCollisionRec(pRect, mRect);
        if (c.height > 0 || c.width > 0) {
            p->blockedBy = e->mobiles[i];
            return 1;
        }
    }
    p->blockedBy = NULL;
    return 0;
}

int atExit(Exploration *e, Player *p) {
    for (int i = 0; i < MAX_EXITS; i++) {
        if (e->exits[i] == NULL) {
            return -1;
        }
        Rectangle pRect = {
                p->mob->position.x,
                p->mob->position.y + 12,
                16,
                12,
        };
        Rectangle c = GetCollisionRec(e->exits[i]->area, pRect);
        if (c.height > 0 || c.width > 0) {
            return i;
        }
    }
    return -1;
}

void evaluateMovement(Exploration *e, Player *p) {
    Vector2 pos = p->mob->position;
    if (p->moving.up == 1 && !isBlocked(e, p, (Vector2){pos.x, pos.y - 1})) {
        p->mob->position.y -= 1;
        p->engageable = NULL;
    }
    if (p->moving.down == 1 && !isBlocked(e, p, (Vector2){pos.x, pos.y + 1})) {
        p->mob->position.y += 1;
        p->engageable = NULL;
    }
    if (p->moving.left == 1 && !isBlocked(e, p, (Vector2){pos.x - 1, pos.y})) {
        p->mob->position.x -= 1;
        p->engageable = NULL;
    }
    if (p->moving.right == 1 && !isBlocked(e, p, (Vector2){pos.x + 1, pos.y})) {
        p->mob->position.x += 1;
        p->engageable = NULL;
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
    ClearBackground(BLACK);
    Vector2 offset = {
            ((float) SCREEN_WIDTH / 2) - p->mob->position.x,
            ((float) SCREEN_HEIGHT / 2) - p->mob->position.y
    };
    DrawTextureEx(e->renderedLayers[BACKGROUND], offset, 0, SCALE, WHITE);
    DrawTextureEx(e->renderedLayers[MIDGROUND], offset, 0, SCALE, WHITE);
    drawExplorationMobiles(e->mobileCount, e->mobiles, p, offset);
    DrawTextureEx(e->renderedLayers[FOREGROUND], offset, 0, SCALE, WHITE);
    drawExplorationControls(p, c);
}

void addMobile(Exploration *exploration, Mobile *mob) {
    exploration->mobiles[exploration->mobileCount] = mob;
    exploration->mobileCount++;
}

void renderExplorationLayers(Exploration *e) {
    ClearBackground(BLACK);
    renderExplorationLayer(e->tilemap, e->objects, e->layers, e->renderedLayers, e->showCollisions, BACKGROUND);
    renderExplorationLayer(e->tilemap, e->objects, e->layers, e->renderedLayers, e->showCollisions, MIDGROUND);
    renderExplorationLayer(e->tilemap, e->objects, e->layers, e->renderedLayers, e->showCollisions, FOREGROUND);
    addDebug(e->log, "exploration successfully rendered");
}
