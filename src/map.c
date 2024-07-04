#include <string.h>
#include <raylib.h>
#include "headers/util/log.h"
#include "headers/mobile.h"
#include "headers/player.h"
#include "headers/graphics/animation.h"
#include "headers/control.h"
#include "headers/warp.h"
#include "headers/tile.h"
#include "headers/shop.h"

typedef enum {
  ACTION_TAKEN_NONE,
  ACTION_TAKEN_ENGAGE_DIALOG,
  ACTION_TAKEN_OPENED_CHEST,
  ACTION_TAKEN_SHOPPING,
} ActionTaken;

typedef struct {
    ActionTaken actionTaken;
    const Chest *chest;
    const ShopTile *shop;
} Response;

typedef struct {
    Mobile *mob;
    Vector2 destination;
} MobileMovement;

typedef struct {
    Vector2D tileSize;
} MapConfig;

typedef struct {
    Context *context;
    int sceneId;
    const char *sceneName;
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
    Tile *openedChest;
    ShopTile *shopTiles[MAX_SHOP_TILES];
    int shopTileCount;
} Map;

Map *createMap(int sceneId, const char *sceneName, Context *c) {
    Map *map = malloc(sizeof(Map));
    map->sceneId = sceneId;
    map->sceneName = sceneName;
    map->context = c;
    map->config = malloc(sizeof(MapConfig));
    map->config->tileSize = (Vector2D) {0, 0};
    map->layers = calloc(MAX_LAYERS, sizeof(Layer));
    map->layersCount = 0;
    map->mobileCount = 0;
    map->entranceCount = 0;
    map->exitCount = 0;
    map->arriveAtCount = 0;
    map->shopTileCount = 0;
    map->tileset = createTileset();
    map->chests = calloc(MAX_CHESTS, sizeof(Chest));
    map->chestCount = 0;
    for (int i = 0; i < MAX_SHOP_TILES; i++) {
        map->shopTiles[i] = NULL;
    }
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        map->mobMovements[i] = NULL;
    }
    map->openedChest = NULL;
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
    int x = m->context->user->resolution.width / m->tileset->size.x + 1;
    int y = m->context->user->resolution.height / m->tileset->size.y + 2;
    return (Vector2D) {x, y};
}

Vector2D getTileFromIndex(const Map *m, int index) {
    int width = m->tileset->sourceTexture.width / m->tileset->size.x;
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

Rectangle getObjectSize(const Map *m, const Object *o, int x, int y) {
    return (Rectangle) {
            (float) (m->tileset->size.x * x) + o->area.x,
            (float) (m->tileset->size.y * y) + o->area.y,
            o->area.width,
            o->area.height,
    };
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

void addMobileToExploration(Map *m, Mobile *mob) {
    m->mobiles[m->mobileCount] = mob;
    m->mobileCount++;
}

void doMobileMovementUpdates(Map *m) {
    for (int i = 0; i < MAX_MOBILE_MOVEMENTS; i++) {
        if (m->mobMovements[i] == NULL) {
            continue;
        }
        Mobile *mob = m->mobMovements[i]->mob;
        bool moved = moveMob(
                mob,
                m->mobMovements[i]->destination,
                (float) m->context->ui->screen->targetFrameRate);
        if (!moved) {
            addInfo("mob done moving -- %s",
                    m->mobMovements[i]->mob->name);
            m->mobMovements[i] = NULL;
            mob->isBeingMoved = false;
        }
    }
}
