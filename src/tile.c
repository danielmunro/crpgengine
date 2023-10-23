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
    int id;
    Rectangle area;
} Object;

typedef struct {
    Object *object;
    Property **properties;
    int propertyCount;
    TilesetType type;
    int id;
} Tile;

typedef struct {
    const char *name;
    const char *value;
} Property2;

typedef struct {
    Property2 **properties;
    int propertyCount;
    Object *object;
    int id;
    const char *type;
} Tile2;

typedef struct {
    Tile2 **tiles;
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
    const char *name;
    int data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer2;

Property2 *createProperty2(const char *name, const char *value) {
    Property2 *p = malloc(sizeof(Property2));
    p->name = name;
    p->value = value;
    return p;
}

Layer2 *createLayer2(const char *name) {
    Layer2 *layer = malloc(sizeof(Layer2));
    layer->name = name;
    layer->width = 0;
    layer->height = 0;
    return layer;
}

Tile2 *createTile2(int id, const char *type) {
    Tile2 *t = malloc(sizeof(Tile2));
    t->id = id;
    t->type = type;
    t->properties = calloc(MAX_PROPERTIES, sizeof(Property));
    t->propertyCount = 0;
    return t;
}

Object *createObject(int id, Rectangle area) {
    Object *o = malloc(sizeof(Object));
    o->id = id;
    o->area = area;
    return o;
}

Tileset *createTileset() {
    Tileset *t = malloc(sizeof(Tileset));
    t->reader = NULL;
    t->tiles = calloc(MAX_TILES, sizeof(Tile2));
    t->tilesCount = 0;
    return t;
}
