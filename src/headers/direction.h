#ifndef CJRPGENGINE_DIRECTION_H
#define CJRPGENGINE_DIRECTION_H

#include <string.h>
#include "errors.h"
#include "log.h"

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

Direction getDirectionFromName(const char *name) {
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        if (strcmp(AnimationTypeStrings[i], name) == 0) {
            return DIRECTIONS[i];
        }
    }
    addError("no direction for name :: %s", name);
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

#endif //CJRPGENGINE_DIRECTION_H
