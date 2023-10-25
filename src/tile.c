#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/util/util.h"

typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

typedef struct {
    int id;
    Rectangle area;
} Object;

typedef struct {
    const char *name;
    const char *value;
} Property;

typedef struct {
    Property **properties;
    int propertyCount;
    Object **objects;
    int objectCount;
    int id;
    const char *type;
} Tile;

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
    Vector2D size;
    Image source;
} Tileset;

typedef struct {
    const char *name;
    int data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer;

Property *createProperty(const char *name, const char *value) {
    Property *p = malloc(sizeof(Property));
    p->name = name;
    p->value = value;
    return p;
}

Layer *createLayer(const char *name) {
    Layer *layer = malloc(sizeof(Layer));
    layer->name = name;
    layer->width = 0;
    layer->height = 0;
    return layer;
}

Tile *createTile(int id, const char *type) {
    Tile *t = malloc(sizeof(Tile));
    t->id = id;
    t->type = type;
    t->properties = calloc(MAX_PROPERTIES, sizeof(Property));
    t->propertyCount = 0;
    t->objects = calloc(MAX_OBJECTS, sizeof(Object));
    t->objectCount = 0;
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
    t->tiles = calloc(MAX_TILES, sizeof(Tile));
    t->tilesCount = 0;
    return t;
}

Property *findProperty(const Tile *t, const char *propertyName) {
    for (int i = 0; i < t->propertyCount; i++) {
        if (strcmp(t->properties[i]->name, propertyName) == 0) {
            return t->properties[i];
        }
    }
    return NULL;
}
