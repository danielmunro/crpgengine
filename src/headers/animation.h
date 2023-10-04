#ifndef CJRPGENGINE_ANIMATION_H
#define CJRPGENGINE_ANIMATION_H

#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440

const AnimationType ANIMATION_TYPES[] = {UP, DOWN, LEFT, RIGHT};

const char *AnimationTypeStrings[] = {
        "up",
        "down",
        "left",
        "right",
};

const AnimationType DIRECTIONS[] = {UP, DOWN, LEFT, RIGHT};
const int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

#define ANIMATION_TYPE_COUNT sizeof(AnimationTypeStrings) / sizeof(AnimationTypeStrings[0])
#define DIRECTION_COUNT sizeof(DIRECTIONS) / sizeof(DIRECTIONS[0])

AnimationType getOppositeDirection(AnimationType direction) {
    if (direction == UP) {
        return DOWN;
    } else if (direction == DOWN) {
        return UP;
    } else if (direction == LEFT) {
        return RIGHT;
    } else if (direction == RIGHT) {
        return LEFT;
    }
    return DOWN;
}

#endif //CJRPGENGINE_ANIMATION_H
