#ifndef CJRPGENGINE_DIRECTION_H
#define CJRPGENGINE_DIRECTION_H

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

#endif //CJRPGENGINE_DIRECTION_H
