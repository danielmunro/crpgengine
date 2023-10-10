#include <raylib.h>
#include "src/headers/direction.h"

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
