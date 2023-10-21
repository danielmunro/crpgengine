#include <string.h>
#include <math.h>
#include <raylib.h>
#include "headers/util/log.h"
#include "headers/mobile.h"
#include "headers/player.h"
#include "headers/graphics/animation.h"
#include "headers/control.h"
#include "headers/graphics/ui/ui.h"
#include "headers/notification.h"
#include "headers/warp.h"
#include "headers/tile.h"

typedef struct {
    Mobile *mob;
    Vector2 destination;
} MobileMovement;

typedef struct {
    Tileset *tileset;
    Layer **layers;
    int layerCount;
    Texture2D renderedLayers[LAYER_COUNT];
    ArriveAt *arriveAt[MAX_ARRIVE_AT];
    int arriveAtCount;
    Exit *exits[MAX_EXITS];
    int exitCount;
    Entrance *entrances[MAX_ENTRANCES];
    int entranceCount;
    int objectCount;
    bool showCollisions;
    Mobile *mobiles[MAX_MOBILES];
    int mobileCount;
    MobileMovement *mobMovements[MAX_MOBILE_MOVEMENTS];
    Tile **tiles;
    int tilesCount;
} Map;

Entrance *findEntrance(Map *m, const char *name) {
    for (int i = 0; i < m->entranceCount; i++) {
        if (strcmp(m->entrances[i]->name, name) == 0) {
            return m->entrances[i];
        }
    }
    addError("entrance not found :: %s", name);
    exit(RuntimeErrorUnknownEntrance);
}

Map *createMap() {
    Map *map = malloc(sizeof(Map));
    map->layers = calloc(MAX_LAYERS, sizeof(Layer));
    map->layerCount = 0;
    map->mobileCount = 0;
    map->entranceCount = 0;
    map->exitCount = 0;
    map->arriveAtCount = 0;
    map->tilesCount = 0;
    map->tiles = calloc(MAX_TILESETS, sizeof(Tile));
    map->tileset = malloc(sizeof(Tileset));
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        map->mobMovements[i] = NULL;
    }
    return map;
}

MobileMovement *createMobileMovement(Mobile *mob, Vector2 destination) {
    MobileMovement *mobMovement = malloc(sizeof(MobileMovement));
    mobMovement->mob = mob;
    mobMovement->destination = destination;
    return mobMovement;
}

void addMobileMovement(Map *m, MobileMovement *mobMovement) {
    addInfo("add mob movement, %s target to %f, %f",
            mobMovement->mob->name, mobMovement->destination.x, mobMovement->destination.y);
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        if (m->mobMovements[i] == NULL) {
            m->mobMovements[i] = mobMovement;
            return;
        }
    }
}

Rectangle *getObject(const Map *m, int id) {
    for (int i = 0; i < m->tilesCount; i++) {
        if (m->tiles[i]->id == id) {
            return m->tiles[i]->object;
        }
    }
    return NULL;
}

Vector2D getTileCount(const Map *m) {
    int x = ui->screen->width / m->tileset->size.x + 1;
    int y = ui->screen->height / m->tileset->size.y + 2;
    return (Vector2D) {x, y};
}

void mapDebugKeyPressed(Vector2 position) {
    addInfo("player coordinates: %f, %f", position.x, position.y);
}

void drawObjectCollision(const Map *m, Image layer, int index, int x, int y) {
    const Rectangle *o = getObject(m, index);
    if (o != NULL) {
        Rectangle r = {
                (float) (m->tileset->size.x * x) + o->x,
                (float) (m->tileset->size.y * y) + o->y,
                o->width,
                o->height,
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

void drawTile(const Map *m, Image layer, int index, int x, int y) {
    if (index <= 0) {
        return;
    }
    Vector2D sz = m->tileset->size;
    Vector2 pos = {
            (float) (sz.x * x),
            (float) (sz.y * y),
    };
    Rectangle rect = getRectForTile(m->tileset, index);
    ImageDraw(
            &layer,
            m->tileset->source,
            rect,
            (Rectangle) {pos.x, pos.y, (float) sz.x, (float) sz.y},
            WHITE
    );
    if (config->showCollisions->objects) {
        drawObjectCollision(m, layer, index - 1, x, y);
    }
}

void drawWarpCollisions(const Map *m, Image *image) {
    for (int i = 0; i < m->exitCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (m->exits[i]->area.x),
                (int) (m->exits[i]->area.y),
                (int) (m->exits[i]->area.width),
                (int) (m->exits[i]->area.height),
                WHITE);
    }
    for (int i = 0; i < m->entranceCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (m->entrances[i]->area.x),
                (int) (m->entrances[i]->area.y),
                (int) (m->entrances[i]->area.width),
                (int) (m->entrances[i]->area.height),
                BLACK);
    }
}

void renderMapLayer(Map *m, LayerType layer) {
    Vector2D sz = m->tileset->size;
    int width = ui->screen->width / sz.x;
    int height = ui->screen->height / sz.y;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x >= m->layers[layer]->width || y >= m->layers[layer]->height) {
                continue;
            }
            drawTile(
                    m,
                    renderedLayer,
                    m->layers[layer]->data[y][x],
                    x,
                    y
            );
        }
    }
    if (config->showCollisions->warps) {
        drawWarpCollisions(m, &renderedLayer);
    }
    m->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
    UnloadImage(renderedLayer);
}

void unloadLayers(const Map *m) {
    UnloadTexture(m->renderedLayers[BACKGROUND]);
    UnloadTexture(m->renderedLayers[MIDGROUND]);
    UnloadTexture(m->renderedLayers[FOREGROUND]);
}

void createMobileLayer(
        Mobile *mobiles[MAX_MOBILES],
        Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES],
        int mobileCount,
        int mobsByYPosition[MAX_LAYERS]) {
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

void drawExplorationMobiles(Map *m, const Player *p, Vector2 offset) {
    /**
     * Start by putting mobs on a layer. This is necessary for drawing them in
     * the right order.
     */
    Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES];
    int mobsByYPosition[MAX_LAYERS];
    createMobileLayer(m->mobiles, mobLayer, m->mobileCount, mobsByYPosition);

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
        for (int n = 0; n < MAX_MOBILES; n++) {
            if (mobLayer[y][n] == NULL) {
                break;
            }
            drawAnimation(
                    getMobAnimation(mobLayer[y][n]),
                    (Vector2) {
                            mobLayer[y][n]->position.x + offset.x,
                            floorf(mobLayer[y][n]->position.y + offset.y),
                    }
            );
        }
    }

    if (config->showCollisions->player) {
        Rectangle *collision = getMobAnimation(mob)->spriteSheet->collision;
        if (collision == NULL) {
            addWarning("configured to show player collision but collision data not set in mob spritesheet");
            return;
        }
        DrawRectangle(
                (int) (mob->position.x + offset.x + collision->x),
                (int) (mob->position.y + offset.y + collision->y),
                (int) collision->width,
                (int) collision->height,
                GREEN);
    }
}

Rectangle getObjectSize(const Map *m, const Rectangle *o, int x, int y) {
    return (Rectangle) {
            (float) (m->tileset->size.x * x) + o->x,
            (float) (m->tileset->size.y * y) + o->y,
            o->width,
            o->height,
    };
}

bool isObjectBlocking(const Map *m, const Rectangle *o, Rectangle player, int x, int y) {
    Rectangle objRect = getObjectSize(m, o, x, y);
    Rectangle c = GetCollisionRec(player, objRect);
    return c.height > 0 || c.width > 0;
}

bool checkLayerForBlockingObject(const Map *m, Rectangle player, int layer) {
    Vector2D tiles = getTileCount(m);
    for (int y = 0; y < tiles.y; y++) {
        for (int x = 0; x < tiles.x; x++) {
            int index = m->layers[layer]->data[y][x];
            const Rectangle *o = getObject(m, index - 1);
            if (o != NULL && isObjectBlocking(m, o, player, x, y)) {
                return true;
            }
        }
    }
    return false;
}

bool isBlockedByMapObject(const Map *m, Rectangle player) {
    for (int l = 0; l < LAYER_COUNT - 1; l++) {
        if (checkLayerForBlockingObject(m, player, l)) {
            return true;
        }
    }
    return false;
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

int atExit(const Map *m, Player *p) {
    Mobile *mob = getPartyLeader(p);
    Rectangle rect = getMobileRectangle(mob);
    for (int i = 0; i < m->exitCount; i++) {
        Rectangle c = GetCollisionRec(m->exits[i]->area, rect);
        if (c.height > 0 || c.width > 0) {
            return i;
        }
    }
    return -1;
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
        if (isBlockedByMapObject(m, rect)) {
            p->blockedBy = NULL;
            return;
        }
        p->blockedBy = getBlockingMob(m, rect);
        if (p->blockedBy != NULL) {
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
    addDebug("map -- evaluate movement -- %f, %f",
             mob->position.x,
             mob->position.y);
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        tryToMove(m, p, DIRECTIONS[i], getMoveFor(mob, DIRECTIONS[i]));
    }
}

void drawExplorationControls(Player *player, ControlBlock *cb[MAX_ACTIVE_CONTROLS], const FontStyle *font) {
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cb[i] == NULL) {
            continue;
        }
        int p = cb[i]->progress;
        if (cb[i]->thenCount > p
                && cb[i]->then[p]->outcome == SPEAK
                && isSpeakingTo(player, cb[i]->then[p]->target)) {
            Rectangle area = ui->textAreas->bottom;
            if (player->dialog == NULL) {
                player->dialog = createDialog(
                        cb[i]->then[p]->message,
                        area,
                        font);
            }
            drawMenuRect(area);
            drawDialog(player->dialog);
        }
    }
}

void drawMapView(
        Map *m,
        Player *p,
        NotificationManager *nm,
        ControlBlock *c[MAX_ACTIVE_CONTROLS],
        FontStyle *font) {
    addDebug("map -- draw");
    Mobile *mob = getPartyLeader(p);
    BeginDrawing();
    ClearBackground(BLACK);
    Vector2 offset = {
            ((float) ui->screen->width / 2) - mob->position.x,
            ((float) ui->screen->height / 2) - mob->position.y
    };
    DrawTextureEx(m->renderedLayers[BACKGROUND], offset, 0, ui->screen->scale, WHITE);
    DrawTextureEx(m->renderedLayers[MIDGROUND], offset, 0, ui->screen->scale, WHITE);
    drawExplorationMobiles(m, p, offset);
    DrawTextureEx(m->renderedLayers[FOREGROUND], offset, 0, ui->screen->scale, WHITE);
    drawNotifications(nm, font);
    drawExplorationControls(p, c, font);
    if (config->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
}

void addMobileToExploration(Map *m, Mobile *mob) {
    m->mobiles[m->mobileCount] = mob;
    m->mobileCount++;
}

void renderExplorationLayers(Map *m) {
    ClearBackground(BLACK);
    renderMapLayer(m, BACKGROUND);
    renderMapLayer(m, MIDGROUND);
    renderMapLayer(m, FOREGROUND);
    addDebug("map successfully rendered");
}

void dialogEngaged(const Player *player, ControlBlock *controlBlock) {
    addInfo("speaking with mob :: %s", player->engageable->name);
    if (controlBlock != NULL) {
        controlBlock->progress++;
        addDebug("active control block progress at %d", controlBlock->progress);
    }
}

void mapSpaceKeyPressed(Player *player, ControlBlock *controlBlocks[MAX_ACTIVE_CONTROLS]) {
    addInfo("map space key pressed");
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (controlBlocks[i] != NULL
            && player->engaged
            && isSpeakOutcome(controlBlocks[i]->then[controlBlocks[i]->progress])) {
            clearDialog(player);
            dialogEngaged(player, controlBlocks[i]);
            return;
        }
    }
    if (player->blockedBy != NULL) {
        engageWithMobile(player);
    }
}

void doMobileMovementUpdates(Map *m) {
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        if (m->mobMovements[i] == NULL) {
            continue;
        }
        Mobile *mob = m->mobMovements[i]->mob;
        bool moved = moveMob(mob, m->mobMovements[i]->destination);
        if (!moved) {
            addInfo("mob done moving -- %s",
                    m->mobMovements[i]->mob->name);
            m->mobMovements[i] = NULL;
            mob->isBeingMoved = false;
        }
    }
}
