#include <string.h>

#define UP "up"
#define DOWN "down"
#define LEFT "left"
#define RIGHT "right"

const char *Directions[] = {
        UP,
        DOWN,
        LEFT,
        RIGHT,
};

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
} Direction;

const Direction DirectionEnums[] = {
        DIRECTION_UP,
        DIRECTION_DOWN,
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
};

#define DIRECTION_COUNT sizeof(Directions) / sizeof(Directions[0])

Direction getDirectionFromString(const char *value) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        if (strcmp(Directions[i], value) == 0) {
            return i;
        }
    }
    addError("direction not found :: %s", value);
    exit(ConfigurationErrorNoDirectionForName);
}

Direction getOppositeDirection(Direction direction) {
    if (direction == DIRECTION_UP) {
        return DIRECTION_DOWN;
    } else if (direction == DIRECTION_DOWN) {
        return DIRECTION_UP;
    } else if (direction == DIRECTION_LEFT) {
        return DIRECTION_RIGHT;
    } else if (direction == DIRECTION_RIGHT) {
        return DIRECTION_LEFT;
    }
    return DIRECTION_DOWN;
}
