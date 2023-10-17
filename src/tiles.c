#include <mm_malloc.h>
#include "headers/util/util.h"

typedef struct {
    Vector2D size;
} Tileset;

typedef struct {
    Vector2D size;
    Image source;
    Tileset *tileset;
} Tilemap;

Tilemap *createTilemap() {
    Tilemap *t = malloc(sizeof(Tilemap));
    t->tileset = malloc(sizeof(Tileset));
    return t;
}
