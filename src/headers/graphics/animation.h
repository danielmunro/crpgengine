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
    ANIMATION_SLEEP_BUBBLE,
} AnimationType;

const AnimationType AnimationTypeEnums[] = {
        ANIMATION_UP,
        ANIMATION_DOWN,
        ANIMATION_LEFT,
        ANIMATION_RIGHT,
        ANIMATION_SLEEP,
        ANIMATION_SLEEP_BUBBLE,
};

const char *AnimationTypes[] = {
        "up",
        "down",
        "left",
        "right",
        "sleep",
        "sleep-bubble",
};

typedef enum {
    DISPLAY_RELATIVE,
    DISPLAY_ABSOLUTE,
} Display;


const Display Displays[] = {
        DISPLAY_RELATIVE,
        DISPLAY_ABSOLUTE,
};

const char *DisplayTypes[] = {
        "relative",
        "absolute",
};

typedef enum {
    PLAY_UNTIL_INDEFINITE,
    PLAY_UNTIL_MOVE,
    PLAY_UNTIL_STOP_MOVE,
} PlayUntil;

const PlayUntil PlayUntils[] = {
        PLAY_UNTIL_INDEFINITE,
        PLAY_UNTIL_MOVE,
        PLAY_UNTIL_STOP_MOVE,
};

const char *PlayUntilTypes[] = {
        "indefinite",
        "move",
        "stopMove",
};

#include "graphics/animation.c"

#endif //CJRPGENGINE_ANIMATION_H
