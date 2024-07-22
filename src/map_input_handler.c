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

bool setCollision(const Map *m, Player *p, Direction direction, Vector2D pos) {
    Mobile *mob = getPartyLeader(p);
    const Rectangle *collision = getMobAnimation(mob)->spriteSheet->collision;
    Rectangle rect = {
            (float) pos.x + collision->x,
            (float) pos.y + collision->y,
            collision->width,
            collision->height,
    };
    if (mob->moving[direction]) {
        resetBlocking(p);
        const Chest *c = getBlockingChest(m, rect);
        if (c != NULL) {
            p->blocking->chest = c;
            return true;
        }
        const Tile *t = getBlockingMapTile(m, rect);
        if (t != NULL) {
            p->blocking->tile = t;
            return true;
        }
        Mobile *blockingMob = getBlockingMob(m, rect);
        if (blockingMob != NULL) {
            p->blocking->mob = blockingMob;
            return true;
        }
        p->engageable = NULL;
    }
    return false;
}

void evaluateMovement(const Player *p) {
    Mobile *mob = getPartyLeader(p);
    if (mob->isBeingMoved) {
        return;
    }
    if (mob->amountToMove > 0) {
        int amount = 1;
        if (mob->direction == DIRECTION_UP) {
            mob->position.y -= amount;
        } else if (mob->direction == DIRECTION_DOWN) {
            mob->position.y += amount;
        } else if (mob->direction == DIRECTION_LEFT) {
            mob->position.x -= amount;
        } else if (mob->direction == DIRECTION_RIGHT) {
            mob->position.x += amount;
        }
        mob->amountToMove -= amount;
    } else {
        resetMoving(mob);
    }
}

bool openChest(Player *p, int sceneId) {
    const Chest *c = p->blocking->chest;
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
    if (player->blocking->mob != NULL) {
        engageWithMobile(player);
        Response *r = createResponse(ACTION_TAKEN_ENGAGE_DIALOG);
        return r;
    }
    if (player->blocking->chest != NULL && openChest(player, m->sceneId)) {
        Response *r = createResponse(ACTION_TAKEN_OPENED_CHEST);
        r->chest = player->blocking->chest;
        return r;
    }
    for (int i = 0; i < m->shopTileCount; i++) {
        Mobile *mob = getPartyLeader(player);
        if (CheckCollisionRecs(
                (Rectangle) {
                        mob->position.x,
                        mob->position.y,
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
    addInfo("player coordinates: %f, %f", position.x / tileSize, position.y / tileSize);
}
