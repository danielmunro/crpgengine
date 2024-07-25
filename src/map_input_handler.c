#include "headers/util/log.h"
#include "headers/player.h"
#include "headers/control.h"

bool isObjectBlocking(const Map *m, const Object *o, Rectangle player, int x, int y) {
        return CheckCollisionRecs(player, getObjectSize(m, o, x, y));
}

const Tile *getBlockingTile(const Map *m, Rectangle player, int layer, int x, int y) {
    const Tile *t = getTile(m, m->layers[layer]->data[y][x]);
    if (t == NULL) {
        return NULL;
    }
    for (int i = 0; i < t->objectCount; i++) {
        if (t->objects[i] == NULL) {
            break;
        }
        if (isObjectBlocking(m, t->objects[i], player, x, y)) {
            return t;
        }
    }
    return NULL;
}

const Tile *getLayerBlockingObject(const Map *m, Rectangle player, int layer) {
    Vector2D tiles = getTileCount(m);
    for (int y = 0; y < tiles.y; y++) {
        for (int x = 0; x < tiles.x; x++) {
            const Tile *t = getBlockingTile(m, player, layer, x, y);
            if (t != NULL) {
                return t;
            }
        }
    }
    return NULL;
}

const Tile *getBlockingMapTile(const Map *m, Rectangle player) {
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
        const Tile *t = getLayerBlockingObject(m, player, l);
        if (t != NULL) {
            return t;
        }
    }
    return NULL;
}

Mobile *getBlockingMob(const Map *m, Rectangle playerRect) {
    for (int i = 0; i < m->mobileCount; i++) {
        if (CheckCollisionRecs(playerRect, getMobileRectangle(m->mobiles[i]))) {
            return m->mobiles[i];
        }
    }
    return NULL;
}

Chest *getBlockingChest(const Map *m, Rectangle playerRect) {
    for (int i = 0; i < m->chestCount; i++) {
        if (CheckCollisionRecs(playerRect, rectangleDtoRectangle(m->chests[i]->area))) {
            return m->chests[i];
        }
    }
    return NULL;
}

bool isCollisionDetected(const Map *m, Player *p, Direction direction, Vector2D pos) {
    Mobile *mob = getPartyLeader(p);
    const Rectangle *collision = getMobAnimation(mob)->spriteSheet->collision;
    Rectangle rect = {
            (float) pos.x + collision->x,
            (float) pos.y + collision->y,
            collision->width,
            collision->height,
    };
    if (mob->moving[direction]) {
        const Chest *c = getBlockingChest(m, rect);
        if (c != NULL) {
            return true;
        }
        const Tile *t = getBlockingMapTile(m, rect);
        if (t != NULL) {
            return true;
        }
        Mobile *blockingMob = getBlockingMob(m, rect);
        if (blockingMob != NULL) {
            return true;
        }
    }
    return false;
}

void evaluateMovement(const MobileManager *mm) {
    const int t = tileSize(mm->context);
    for (int i = 0; i < mm->mobileCount; i++) {
        Mobile *mob = mm->mobiles[i];
        if (mob->amountToMove > 0) {
            if (mob->direction == DIRECTION_UP) {
                mob->position.y -= 1;
            } else if (mob->direction == DIRECTION_DOWN) {
                mob->position.y += 1;
            } else if (mob->direction == DIRECTION_LEFT) {
                mob->position.x -= 1;
            } else if (mob->direction == DIRECTION_RIGHT) {
                mob->position.x += 1;
            }
            mob->amountToMove -= 1;
        } else if (mob->position.x > mob->destination.x) {
            mob->direction = DIRECTION_LEFT;
            mob->moving[DIRECTION_LEFT] = true;
            mob->amountToMove = t;
            getMobAnimation(mob)->isPlaying = true;
        } else if (mob->position.x < mob->destination.x) {
            mob->direction = DIRECTION_RIGHT;
            mob->moving[DIRECTION_RIGHT] = true;
            mob->amountToMove = t;
            getMobAnimation(mob)->isPlaying = true;
        } else if (mob->position.y < mob->destination.y) {
            mob->direction = DIRECTION_DOWN;
            mob->moving[DIRECTION_DOWN] = true;
            mob->amountToMove = t;
            getMobAnimation(mob)->isPlaying = true;
        } else if (mob->position.y > mob->destination.y) {
            mob->direction = DIRECTION_UP;
            mob->moving[DIRECTION_UP] = true;
            mob->amountToMove = t;
            getMobAnimation(mob)->isPlaying = true;
        } else {
            resetMoving(mob);
        }
    }
}

bool openChest(Player *p, Chest *c, int sceneId) {
    for (int i = 0; i < p->openedChestsCount; i++) {
        if (p->openedChests[i]->chestId == c->id
            && p->openedChests[i]->sceneId == sceneId) {
            return false;
        }
    }
    addInfo("chest opened :: %s", c->iq->item->name);
    for (int i = 0; i < c->iq->quantity; i++) {
        addItem(p, c->iq->item);
    }
    p->coins += c->coins;
    p->openedChests[p->openedChestsCount] = createOpenedChest(
            sceneId,
            c->id);
    p->openedChestsCount++;
    return true;
}

void dialogEngaged(const Player *player, ControlBlock *controlBlock) {
    addInfo("speaking with mob :: %s", player->engageable->name);
    if (controlBlock != NULL) {
        controlBlock->progress++;
        addDebug("active control block progress at %d", controlBlock->progress);
    }
}

Response *createResponse(ActionTaken actionTaken) {
    Response *response = malloc(sizeof(Response));
    response->actionTaken = actionTaken;
    response->chest = NULL;
    response->shop = NULL;
    return response;
}

Response *mapSpaceKeyPressed(const Map *m, Player *player, ControlBlock *controlBlocks[MAX_ACTIVE_CONTROLS]) {
    addInfo("map space key pressed");
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (controlBlocks[i] != NULL
            && player->engaged
            && isSpeakOutcome(controlBlocks[i]->then[controlBlocks[i]->progress])) {
            clearDialog(player);
            dialogEngaged(player, controlBlocks[i]);
            Response *r = createResponse(ACTION_TAKEN_ENGAGE_DIALOG);
            return r;
        }
    }
    if (player->dialog) {
        clearDialog(player);
        player->engaged = false;
        return createResponse(ACTION_TAKEN_NONE);
    }
    const Mobile *mob = getPartyLeader(player);
    Vector2D focusPos = mob->position;
    if (mob->direction == DIRECTION_UP) {
        focusPos.y -= tileSize(m->context);
    } else if (mob->direction == DIRECTION_DOWN) {
        focusPos.y += tileSize(m->context);
    } else if (mob->direction == DIRECTION_LEFT) {
        focusPos.x -= tileSize(m->context);
    } else if (mob->direction == DIRECTION_RIGHT) {
        focusPos.x += tileSize(m->context);
    }
    for (int i = 0; i < m->mobileCount; i++) {
        if (m->mobiles[i]->position.x == focusPos.x && m->mobiles[i]->position.y == focusPos.y) {
            engageWithMobile(player, m->mobiles[i]);
            Response *r = createResponse(ACTION_TAKEN_ENGAGE_DIALOG);
            return r;
        }
    }
    for (int i = 0; i < m->chestCount; i++) {
        if (CheckCollisionRecs(
                (Rectangle) {
                        (float) focusPos.x,
                        (float) focusPos.y,
                        (float) tileSize(m->context),
                        (float) tileSize(m->context),
                }, rectangleDtoRectangle(m->chests[i]->area)) && openChest(player, m->chests[i], m->sceneId)) {
            Response *r = createResponse(ACTION_TAKEN_OPENED_CHEST);
            r->chest = m->chests[i];
            return r;
        }
    }
    for (int i = 0; i < m->shopTileCount; i++) {
        if (CheckCollisionRecs(
                (Rectangle) {
                        (float) mob->position.x,
                        (float) mob->position.y,
                        (float) m->context->game->tileSize,
                        (float) m->context->game->tileSize,
                },
                rectangleDtoRectangle(m->shopTiles[i]->object->area))) {
            Response *r = createResponse(ACTION_TAKEN_SHOPPING);
            r->shop = m->shopTiles[i];
            return r;
        }
    }
    return createResponse(ACTION_TAKEN_NONE);
}

void mapDebugKeyPressed(Vector2D position, int tileSize) {
    addInfo("player coordinates: %d, %d", position.x / tileSize, position.y / tileSize);
}
