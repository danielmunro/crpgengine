#ifndef CJRPGENGINE_TILE_H
#define CJRPGENGINE_TILE_H

const char *TilesetTypes[] = {
        "",
        "chest",
};

typedef enum {
    TILESET_TYPE_NONE,
    TILESET_TYPE_CHEST,
} TilesetType;

int TilesetTypeCount = sizeof(TilesetTypes) / sizeof(TilesetTypes[0]);

#include "tile.c"

#endif //CJRPGENGINE_TILE_H
