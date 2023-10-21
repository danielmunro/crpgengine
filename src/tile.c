#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/util/util.h"

const char *TilesetTypes[] = {
        "",
        "tile",
        "chest",
};

typedef enum {
    TILESET_TYPE_NONE,
    TILESET_TYPE_TILE,
    TILESET_TYPE_CHEST,
} TilesetType;

typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

const char *LayerTypes[] = {
        "background",
        "midground",
        "foreground",
};

LayerType getLayerTypeFromString(const char *layerType) {
    for (int i = 0; i < LAYER_COUNT; i++) {
        if (strcmp(LayerTypes[i], layerType) == 0) {
            return i;
        }
    }
    addError("layer type not found :: %s", layerType);
    exit(ConfigurationErrorLayerTypeNotFound);
}

typedef struct {
    const char *name;
    const char *value;
} Property;

typedef struct {
    Rectangle *object;
    Property **properties;
    int propertyCount;
    TilesetType type;
    int id;
} Tile;

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
    Vector2D size;
    Image source;
} Tileset;

typedef struct {
    LayerType type;
    int data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer;

typedef struct {
    Rectangle object;
    ItemWithQuantity *iq;
    int money;
} Chest;

typedef struct {
    const char *filePath;
    xmlTextReaderPtr reader;
    Tileset *tileset;
    Layer **layers;
    int layerCount;
    Entrance **entrances;
    int entranceCount;
    Exit **exits;
    int exitCount;
    ArriveAt **arriveAt;
    int arriveAtCount;
    Chest **chests;
    int chestCount;
} Tilemap;

Property *createProperty() {
    Property *p = malloc(sizeof(Property));
    return p;
}

Tile *createTile(int id, TilesetType type) {
    Tile *t = malloc(sizeof(Tile));
    t->id = id;
    t->type = type;
    t->properties = calloc(MAX_PROPERTIES, sizeof(Property));
    t->propertyCount = 0;
    return t;
}

Layer *createLayer() {
    Layer *layer = malloc(sizeof(Layer));
    layer->type = -1;
    layer->width = 0;
    layer->height = 0;
    return layer;
}

Tilemap *createTilemap(const char *filePath) {
    Tilemap *t = malloc(sizeof(Tilemap));
    t->filePath = filePath;
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tileset = NULL;
    t->layers = calloc(MAX_LAYERS, sizeof(Layer));
    t->layerCount = 0;
    t->entrances = calloc(MAX_ENTRANCES, sizeof(Entrance));
    t->entranceCount = 0;
    t->exits = calloc(MAX_EXITS, sizeof(Exit));
    t->exitCount = 0;
    t->arriveAt = calloc(MAX_ARRIVE_AT, sizeof(ArriveAt));
    t->arriveAtCount = 0;
    t->chests = calloc(MAX_CHESTS, sizeof(Chest));
    t->chestCount = 0;
    return t;
}

Tileset *createTileset(const char *filePath) {
    Tileset *t = malloc(sizeof(Tileset));
    t->reader = xmlReaderForFile(filePath, NULL, 0);
    t->tiles = calloc(MAX_TILES, sizeof(Tile));
    t->tilesCount = 0;
    return t;
}

Chest *createChest(Rectangle object, ItemWithQuantity *iq, int money) {
    Chest *chest = malloc(sizeof(Chest));
    chest->object = object;
    chest->iq = iq;
    chest->money = money;
    return chest;
}

void addProperty(Tile *tile, Property *property) {
    tile->properties[tile->propertyCount] = property;
    tile->propertyCount++;
}

TilesetType getTilesetTypeFromString(const char *type) {
    int TilesetTypeCount = sizeof(TilesetTypes) / sizeof(TilesetTypes[0]);
    for (int i = 0; i < TilesetTypeCount; i++) {
        if (strcmp(TilesetTypes[i], type) == 0) {
            return i;
        }
    }
    return TILESET_TYPE_NONE;
}

Vector2D getTileFromIndex(const Tileset *t, int index) {
    int width = t->source.width / t->size.x;
    int y = index / width;
    int x = (index % width);
    if (x - 1 < 0) {
        y--;
        x = width;
    }
    Vector2D pos = {x - 1, y};
    return pos;
}

Rectangle getRectForTile(const Tileset *t, int index) {
    Vector2D tile = getTileFromIndex(t, index);
    return (Rectangle) {
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}
