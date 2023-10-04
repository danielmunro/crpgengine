#ifndef CJRPGENGINE_ANIMATION_H
#define CJRPGENGINE_ANIMATION_H

#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440

const int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

#define ANIMATION_TYPE_COUNT sizeof(AnimationTypeStrings) / sizeof(AnimationTypeStrings[0])

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

#endif //CJRPGENGINE_ANIMATION_H
