#include <raylib.h>
#include <mm_malloc.h>

typedef struct {
    Rectangle rect;
    int tile;
} Object;

typedef struct {
    Object *object;

} Chest;

Object *createTileObject(int tile, Rectangle rect) {
    Object *o = malloc(sizeof(Object));
    o->tile = tile;
    o->rect = rect;
    return o;
}
