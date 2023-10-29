#include <raylib.h>
#include <libxml/xmlreader.h>
#include "headers/util/util.h"

const char *TileTypes[] = {
        "other",
        "chest_full",
        "chest_empty",
};

typedef enum {
    TILE_TYPE_OTHER,
    TILE_TYPE_CHEST_FULL,
    TILE_TYPE_CHEST_EMPTY,
    TILE_TYPE_NONE,
} TileType;

const char *ObjectTypes[] = {
        "other",
        "exit",
        "entrance",
};

typedef enum {
    OBJECT_TYPE_OTHER,
    OBJECT_TYPE_EXIT,
    OBJECT_TYPE_ENTRANCE,
} ObjectType;

const char *NodeNames[] = {
        "other",
        "object",
        "properties",
        "property",
        "objectgroup",
        "data",
};

typedef enum {
    NODE_NAME_OTHER,
    NODE_NAME_OBJECT,
    NODE_NAME_PROPERTIES,
    NODE_NAME_PROPERTY,
    NODE_NAME_OBJECT_GROUP,
    NODE_NAME_DATA,
} NodeName;

const char *TilemapObjectGroupNames[] = {
        "other",
        "warps",
        "arrive_at",
        "chests",
};

typedef enum {
    TILEMAP_OBJECT_GROUP_OTHER,
    TILEMAP_OBJECT_GROUP_WARPS,
    TILEMAP_OBJECT_GROUP_ARRIVE_AT,
    TILEMAP_OBJECT_GROUP_CHESTS,
} TilemapObjectGroupName;

const char *TilemapNodeTypes[] = {
        "other",
        "tileset",
        "layer",
        "objectgroup",
};

typedef enum {
    TILEMAP_NODE_TYPE_OTHER,
    TILEMAP_NODE_TYPE_TILESET,
    TILEMAP_NODE_TYPE_LAYER,
    TILEMAP_NODE_TYPE_OBJECTGROUP,
} TilemapNodeType;

const char *TileSetNodeTypes[] = {
        "other",
        "tileset",
        "image",
        "tile",
};

typedef enum {
    TILESET_NODE_TYPE_OTHER,
    TILESET_NODE_TYPE_TILESET,
    TILESET_NODE_TYPE_IMAGE,
    TILESET_NODE_TYPE_TILE,
} TilesetNodeType;

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
    int id;
    Property **properties;
    int propertyCount;
    Object **objects;
    int objectCount;
    TileType type;
} Tile;

typedef struct {
    Tile **tiles;
    int tilesCount;
    xmlTextReaderPtr reader;
    Vector2D size;
    Image source;
    Texture sourceTexture;
} Tileset;

typedef struct {
    const char *name;
    int data[MAX_LAYER_SIZE][MAX_LAYER_SIZE];
    int width;
    int height;
    bool showCollisions;
} Layer;

typedef struct {
    int id;
    ItemWithQuantity *iq;
    int coins;
    int opened;
    Rectangle area;
} Chest;

TileType getTileTypeFromString(const char *type) {
    if (type == NULL) {
        return TILE_TYPE_NONE;
    }
    int count = sizeof(TileTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TileTypes[i], type) == 0) {
            return i;
        }
    }
    addDebug("object type not found :: %s", type);
    return TILE_TYPE_OTHER;
}

ObjectType getObjectTypeFromString(const char *type) {
    int count = sizeof(ObjectTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(ObjectTypes[i], type) == 0) {
            return i;
        }
    }
    addDebug("object type not found :: %s", type);
    return OBJECT_TYPE_OTHER;
}

NodeName getNodeNameFromString(const char *name) {
    int count = sizeof(NodeNames) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(NodeNames[i], name) == 0) {
            return i;
        }
    }
    addDebug("node name not found :: %s", name);
    return NODE_NAME_OTHER;
}

TilemapObjectGroupName getTilemapObjectGroupNameFromString(const char *groupName) {
    int count = sizeof(TilemapObjectGroupNames) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TilemapObjectGroupNames[i], groupName) == 0) {
            return i;
        }
    }
    addDebug("tile map object group name not found :: %s", groupName);
    return TILEMAP_OBJECT_GROUP_OTHER;
}

TilemapNodeType getTileMapNodeTypeFromString(const char *nodeType) {
    int count = sizeof(TilemapNodeTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TilemapNodeTypes[i], nodeType) == 0) {
            return i;
        }
    }
    addDebug("tile map node type not found :: %s", nodeType);
    return TILEMAP_NODE_TYPE_OTHER;
}

TilesetNodeType getTilesetNodeTypeFromString(const char *tileSetNodeType) {
    int count = sizeof(TileSetNodeTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TileSetNodeTypes[i], tileSetNodeType) == 0) {
            return i;
        }
    }
    addDebug("tileset node type not found :: %s", tileSetNodeType);
    return TILESET_NODE_TYPE_OTHER;
}

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

Tile *createTile(int id, TileType type) {
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

Chest *createChest(int id, ItemWithQuantity *iq, Rectangle area) {
    Chest *chest = malloc(sizeof(Chest));
    chest->id = id;
    chest->iq = iq;
    chest->opened = false;
    chest->area = area;
    return chest;
}
