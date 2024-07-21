#include <raylib.h>
#include <mm_malloc.h>
#include "headers/direction.h"

typedef struct {
    int id;
    RectangleD area;
    const char *to;
    const char *scene;
} Exit;

typedef struct {
    int id;
    const char *name;
    RectangleD area;
    Direction direction;
} Entrance;

Exit *createExit(int id, const char *to, const char *scene, RectangleD area) {
    Exit *e = malloc(sizeof(Exit));
    e->id = id;
    e->to = to;
    e->scene = scene;
    e->area = area;
    return e;
}

Entrance *createEntrance(int id, const char *name, Direction direction, RectangleD area) {
    Entrance *e = malloc(sizeof(Entrance));
    e->id = id;
    e->name = name;
    e->direction = direction;
    e->area = area;
    return e;
}
