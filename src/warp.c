#include <mm_malloc.h>
#include "headers/direction.h"

typedef struct {
    int id;
    Rectangle area;
    const char *to;
    const char *scene;
} Exit;

typedef struct {
    int id;
    const char *name;
    Rectangle area;
    Direction direction;
} Entrance;

Exit *createExit(int id, const char *to, const char *scene, Rectangle area) {
    Exit *e = malloc(sizeof(Exit));
    e->id = id;
    e->to = to;
    e->scene = scene;
    e->area = area;
    return e;
}

Entrance *createEntrance(int id, const char *name, Direction direction, Rectangle area) {
    Entrance *e = malloc(sizeof(Entrance));
    e->id = id;
    e->name = name;
    e->direction = direction;
    e->area = area;
    return e;
}
