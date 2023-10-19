#include <raylib.h>
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

typedef struct {
    Vector2D size;
    Image source;
} Tilemap;

typedef struct {
    Rectangle rect;
    int tile;
} Object;

typedef struct {
    const char *name;
    const char *value;
} Property;

typedef struct {
    Object *object;
    Property **properties;
    int propertyCount;
    TilesetType type;
    int id;
} Tile;

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

void addProperty(Tile *tile, Property *property) {
    tile->properties[tile->propertyCount] = property;
    tile->propertyCount++;
}

Vector2D getTileFromIndex(const Tilemap *t, int index) {
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

Rectangle getRectForTile(const Tilemap *t, int index) {
    Vector2D tile = getTileFromIndex(t, index);
    return (Rectangle) {
            (float) (tile.x * t->size.x),
            (float) (tile.y * t->size.y),
            (float) t->size.x,
            (float) t->size.y,
    };
}

Object *createTileObject(int tile, Rectangle rect) {
    Object *o = malloc(sizeof(Object));
    o->tile = tile;
    o->rect = rect;
    return o;
}

typedef struct {
    Object *object;
    ItemWithQuantity *iq;
    int money;
} Chest;

Chest *createChest(Object *object, ItemWithQuantity *iq, int money) {
    Chest *chest = malloc(sizeof(Chest));
    chest->object = object;
    chest->iq = iq;
    chest->money = money;
    return chest;
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
