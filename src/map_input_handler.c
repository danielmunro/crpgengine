#include "headers/util/log.h"
#include "headers/player.h"
#include "headers/control.h"

bool isObjectBlocking(const Map *m, const Object *o, Rectangle player, int x, int y) {
    Rectangle objRect = getObjectSize(m, o, x, y);
    Rectangle c = GetCollisionRec(player, objRect);
    return c.height > 0 || c.width > 0;
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
        Rectangle c = GetCollisionRec(playerRect, getMobileRectangle(m->mobiles[i]));
        if (c.height > 0 || c.width > 0) {
            return m->mobiles[i];
        }
    }
    return NULL;
}

Chest *getBlockingChest(const Map *m, Rectangle playerRect) {
    for (int i = 0; i < m->chestCount; i++) {
        Rectangle c = GetCollisionRec(playerRect, m->chests[i]->area);
        if (c.height > 0 || c.width > 0) {
            return m->chests[i];
        }
    }
    return NULL;
}

void tryToMove(const Map *m, Player *p, Direction direction, Vector2 pos) {
    Mobile *mob = getPartyLeader(p);
    const Rectangle *collision = getMobAnimation(mob)->spriteSheet->collision;
    Rectangle rect = {
            pos.x + collision->x,
            pos.y + collision->y,
            collision->width,
            collision->height,
    };
    if (mob->moving[direction]) {
        const Chest *c = getBlockingChest(m, rect);
        if (c != NULL) {
            setBlockedByChest(p, c);
            return;
        }
        const Tile *t = getBlockingMapTile(m, rect);
        if (t != NULL) {
            setBlockedByTile(p, t);
            return;
        }
        Mobile *blockingMob = getBlockingMob(m, rect);
        if (blockingMob != NULL) {
            setBlockedByMob(p, blockingMob);
            return;
        }
        mob->position = pos;
        p->engageable = NULL;
    }
}

void evaluateMovement(const Map *m, Player *p) {
    const Mobile *mob = getPartyLeader(p);
    if (mob->isBeingMoved) {
        return;
    }
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        tryToMove(
                m,
                p,
                DirectionEnums[i],
                getMoveFor(mob, DirectionEnums[i], (float) m->context->ui->screen->targetFrameRate));
    }
}

void openChest(Player *p, int sceneId) {
    const Chest *c = p->blocking->chest;
    for (int i = 0; i < p->openedChestsCount; i++) {
        if (p->openedChests[i]->chestId == c->id
            && p->openedChests[i]->sceneId == sceneId) {
            return;
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
    if (player->blocking->chest != NULL) {
        openChest(player, m->sceneId);
        Response *r = createResponse(ACTION_TAKEN_OPENED_CHEST);
        r->chest = player->blocking->chest;
        return r;
    }
    for (int i = 0; i < m->shopTileCount; i++) {
        Mobile *mob = getPartyLeader(player);
        Rectangle c = GetCollisionRec(
                (Rectangle) {
                        mob->position.x,
                        mob->position.y,
                        (float) m->tileset->size.x,
                        (float) m->tileset->size.y,
                },
                m->shopTiles[i]->object->area);
        if (c.height > 0 || c.width > 0) {
            Response *r = createResponse(ACTION_TAKEN_SHOPPING);
            r->shop = m->shopTiles[i];
            return r;
        }
    }
    return createResponse(ACTION_TAKEN_NONE);
}
