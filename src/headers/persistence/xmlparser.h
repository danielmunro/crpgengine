#ifndef CJRPGENGINE_OBJECT_H
#define CJRPGENGINE_OBJECT_H

#include "src/headers/exploration.h"

const char *TileMapNodeTypes[] = {
        "other",
        "tileset",
        "layer",
        "data",
        "object",
};

typedef enum {
    TILEMAP_NODE_TYPE_OTHER,
    TILEMAP_NODE_TYPE_TILESET,
    TILEMAP_NODE_TYPE_LAYER,
    TILEMAP_NODE_TYPE_DATA,
    TILEMAP_NODE_TYPE_OBJECT,
} TileMapNodeType;

TileMapNodeType getTileMapNodeTypeFromString(const char *nodeType) {
    int count = sizeof(TileMapNodeTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TileMapNodeTypes[i], nodeType) == 0) {
            return i;
        }
    }
    addDebug("tile map node type not found :: %s", nodeType);
    return TILEMAP_NODE_TYPE_OTHER;
}

const char *ObjectTypes[] = {
        "other",
        "entrance",
        "exit",
        "arrive_at",
        "chest",
};

typedef enum {
    OBJECT_TYPE_OTHER,
    OBJECT_TYPE_ENTRANCE,
    OBJECT_TYPE_EXIT,
    OBJECT_TYPE_ARRIVE_AT,
    OBJECT_TYPE_CHEST,
} ObjectType;

ObjectType getObjectTypeFromString(const char *objectType) {
    int count = sizeof(ObjectTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(ObjectTypes[i], objectType) == 0) {
            return i;
        }
    }
    addError("object type not found :: %s", objectType);
    return OBJECT_TYPE_OTHER;
}

const char *TileSetNodeTypes[] = {
        "other",
        "tileset",
        "image",
        "tile",
        "object",
};

typedef enum {
    TILESET_NODE_TYPE_OTHER,
    TILESET_NODE_TYPE_TILESET,
    TILESET_NODE_TYPE_IMAGE,
    TILESET_NODE_TYPE_TILE,
    TILESET_NODE_TYPE_OBJECT,
} TileSetNodeType;

TileSetNodeType getTileSetNodeTypeFromString(const char *tileSetNodeType) {
    int count = sizeof(TileSetNodeTypes) / sizeof(const char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(TileSetNodeTypes[i], tileSetNodeType) == 0) {
            return i;
        }
    }
    addDebug("tileset type not found :: %s", tileSetNodeType);
    return TILESET_NODE_TYPE_OTHER;
}

#include "src/persistence/xmlparser.c"

#endif //CJRPGENGINE_OBJECT_H
