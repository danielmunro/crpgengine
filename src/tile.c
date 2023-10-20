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

typedef struct {
    Rectangle *object;
    ItemWithQuantity *iq;
    int money;
} Chest;

Chest *createChest(Rectangle *object, ItemWithQuantity *iq, int money) {
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
