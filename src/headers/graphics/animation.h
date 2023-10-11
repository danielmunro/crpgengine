#ifndef CJRPGENGINE_ANIMATION_H
#define CJRPGENGINE_ANIMATION_H

#include "_deps/raylib-src/src/raylib.h"

#define MAX_ANIMATIONS 25
#define MAX_ANIMATIONS_IN_GAME 1440

const int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

#define ANIMATION_TYPE_COUNT sizeof(AnimationTypeStrings) / sizeof(AnimationTypeStrings[0])

#include "graphics/animation.c"

#endif //CJRPGENGINE_ANIMATION_H
