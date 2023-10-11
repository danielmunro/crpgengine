#include <raylib.h>
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
