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
    Vector2D tileSize;
} MapConfig;

typedef struct {
    MapConfig *config;
    Tileset *tileset;
    Layer **layers;
    int layersCount;
    Texture2D renderedLayers[LAYER_COUNT];
    ArriveAt *arriveAt[MAX_ARRIVE_AT];
    int arriveAtCount;
    Exit *exits[MAX_EXITS];
    int exitCount;
    Entrance *entrances[MAX_ENTRANCES];
    int entranceCount;
    bool showCollisions;
    Mobile *mobiles[MAX_MOBILES];
    int mobileCount;
    MobileMovement *mobMovements[MAX_MOBILE_MOVEMENTS];
    Chest **chests;
    int chestCount;
} Map;

Map *createMap() {
    Map *map = malloc(sizeof(Map));
    map->config = malloc(sizeof(MapConfig));
    map->config->tileSize = (Vector2D) {0, 0};
    map->layers = calloc(MAX_LAYERS, sizeof(Layer));
    map->layersCount = 0;
    map->mobileCount = 0;
    map->entranceCount = 0;
    map->exitCount = 0;
    map->arriveAtCount = 0;
    map->tileset = createTileset();
    map->chests = calloc(MAX_CHESTS, sizeof(Chest));
    map->chestCount = 0;
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

Entrance *findEntrance(Map *m, const char *name) {
    for (int i = 0; i < m->entranceCount; i++) {
        if (strcmp(m->entrances[i]->name, name) == 0) {
            return m->entrances[i];
        }
    }
    addError("entrance not found :: %s", name);
    exit(RuntimeErrorUnknownEntrance);
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

Tile *getTile(const Map *m, int tileNumber) {
    for (int i = 0; i < m->tileset->tilesCount; i++) {
        if (m->tileset->tiles[i]->id == tileNumber - 1) {
            return m->tileset->tiles[i];
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

void drawObjectCollision(const Map *m, Image layer, int tileNumber, int x, int y) {
    const Tile *t = getTile(m, tileNumber);
    if (t == NULL) {
        return;
    }
    for (int i = 0; i < t->objectCount; i++) {
        if (t->objects[i] == NULL) {
            return;
        }
        Rectangle r = {
                (float) (m->tileset->size.x * x) + t->objects[i]->area.x,
                (float) (m->tileset->size.y * y) + t->objects[i]->area.y,
                t->objects[i]->area.width,
                t->objects[i]->area.height,
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

Vector2D getTileFromIndex(const Map *m, int index) {
    int width = m->tileset->source.width / m->tileset->size.x;
    int y = index / width;
    int x = (index % width);
    if (x - 1 < 0) {
        y--;
        x = width;
    }
    Vector2D pos = {x - 1, y};
    return pos;
}

Rectangle getRectForTile(const Map *m, int index) {
    Vector2D tile = getTileFromIndex(m, index);
    return (Rectangle) {
            (float) (tile.x * m->config->tileSize.x),
            (float) (tile.y * m->config->tileSize.y),
            (float) m->config->tileSize.x,
            (float) m->config->tileSize.y,
    };
}

void drawTile(const Map *m, Image layer, int index, int x, int y) {
    if (index <= 0) {
        return;
    }
    Vector2D sz = m->config->tileSize;
    Vector2 pos = {
            (float) (sz.x * x),
            (float) (sz.y * y),
    };
    Rectangle rect = getRectForTile(m, index);
    ImageDraw(
            &layer,
            m->tileset->source,
            rect,
            (Rectangle) {pos.x, pos.y, (float) sz.x, (float) sz.y},
            WHITE
    );
    if (config->showCollisions->objects) {
        drawObjectCollision(m, layer, index, x, y);
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

Rectangle getObjectSize(const Map *m, const Object *o, int x, int y) {
    return (Rectangle) {
            (float) (m->tileset->size.x * x) + o->area.x,
            (float) (m->tileset->size.y * y) + o->area.y,
            o->area.width,
            o->area.height,
    };
}

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
        const Tile *t = getBlockingMapTile(m, rect);
        if (t != NULL) {
            p->blockedBy = NULL;
            setBlockedByTile(p, t);
            return;
        }
        Mobile *blockingMob = getBlockingMob(m, rect);
        if (blockingMob != NULL) {
            p->blockedBy = blockingMob;
            setBlockedByMob(p, p->blockedBy);
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
        tryToMove(m, p, DirectionEnums[i], getMoveFor(mob, DirectionEnums[i]));
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
