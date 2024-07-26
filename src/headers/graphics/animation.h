#ifndef CJRPGENGINE_ANIMATION_H
#define CJRPGENGINE_ANIMATION_H

#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440

typedef enum {
    ANIMATION_UP,
    ANIMATION_DOWN,
    ANIMATION_LEFT,
    ANIMATION_RIGHT,
    ANIMATION_SLEEP,
} AnimationType;

const AnimationType AnimationTypeEnums[] = {
        ANIMATION_UP,
        ANIMATION_DOWN,
        ANIMATION_LEFT,
        ANIMATION_RIGHT,
        ANIMATION_SLEEP,
};

const char *AnimationTypes[] = {
        "up",
        "down",
        "left",
        "right"
        "sleep",
};

#include "graphics/animation.c"

#endif //CJRPGENGINE_ANIMATION_H
