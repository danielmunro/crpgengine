#include <raylib.h>
#include <mm_malloc.h>
#include "headers/direction.h"

typedef struct {
    Rectangle area;
    char *to;
    char *scene;
} Exit;

typedef struct {
    char *name;
    Rectangle area;
    Direction direction;
} Entrance;

Exit *createExit() {
    Exit *e = malloc(sizeof(Exit));
    e->to = "";
    e->scene = "";
    e->area = (Rectangle) {0, 0, 0, 0};
    return e;
}

Entrance *createEntrance(char *name, Rectangle area) {
    Entrance *e = malloc(sizeof(Entrance));
    e->name = &name[0];
    e->direction = DIRECTION_DOWN;
    e->area = area;
    return e;
}
