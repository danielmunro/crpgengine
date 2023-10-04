#ifndef CJRPGENGINE_OBJECT_H
#define CJRPGENGINE_OBJECT_H

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
    fprintf(stderr, "tile map node type not found :: %s", nodeType);
//    exit(ConfigurationErrorUnknownTileMapNodeType);
    return TILEMAP_NODE_TYPE_OTHER;
}

const char *ObjectTypes[] = {
        "entrance",
        "exit",
        "arrive_at",
        "chest",
};

typedef enum {
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
    fprintf(stderr, "object type not found :: %s", objectType);
    exit(ConfigurationErrorUnknownTileMapNodeType);
}

#endif //CJRPGENGINE_OBJECT_H
