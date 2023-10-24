#include <string.h>

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
} Direction;

const char *AnimationTypeStrings[] = {
        "up",
        "down",
        "left",
        "right",
};

const Direction DIRECTIONS[] = {
        DIRECTION_UP,
        DIRECTION_DOWN,
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
};

#define DIRECTION_COUNT sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0])

Direction getDirectionFromString(const char *value) {
    if (strcmp(value, "up") == 0) {
        return DIRECTION_UP;
    } else if (strcmp(value, "down") == 0) {
        return DIRECTION_DOWN;
    } else if (strcmp(value, "left") == 0) {
        return DIRECTION_LEFT;
    } else if (strcmp(value, "right") == 0) {
        return DIRECTION_RIGHT;
    } else {
        fprintf(stderr, "invalid direction: %s\n", value);
        return 0;
    }
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