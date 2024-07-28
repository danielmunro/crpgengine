#include <mm_malloc.h>
#include "headers/graphics/ui/ui.h"
#include "headers/graphics/spritesheet.h"
#include "headers/context.h"

typedef struct {
    Context *context;
    const char *name;
    AnimationType type;
    Spritesheet *spriteSheet;
    int firstFrame;
    int lastFrame;
    int currentFrame;
    int repeat;
    int frameRate;
    int frameRateCount;
    bool isPlaying;
    Display display;
    Vector2D position;
    PlayUntil playUntil;
} Animation;

Display getDisplayFromString(const char *display) {
    for (int i = 0; i < sizeof(DisplayTypes) / sizeof(char *); i++) {
        if (strcmp(display, DisplayTypes[i]) == 0) {
            return Displays[i];
        }
    }
    addError("unknown display type :: %s", display);
    exit(1);
}

PlayUntil getPlayUntilFromString(const char *playUntil) {
    for (int i = 0; i < sizeof(PlayUntilTypes) / sizeof(char *); i++) {
        if (strcmp(playUntil, PlayUntilTypes[i]) == 0) {
            return PlayUntils[i];
        }
    }
    addError("unknown play until type :: %s", playUntil);
    exit(1);
}

void incrementAnimationFrame(Animation *a, float targetFPS) {
    a->frameRateCount++;
    if (a->frameRateCount >= ((int) targetFPS / a->frameRate)) {
        a->frameRateCount = 0;
        a->currentFrame++;
        if (a->currentFrame > a->lastFrame) {
            a->currentFrame = a->firstFrame;
        }
    }
}

Animation *createAnimation(
        const char *name,
        AnimationType type,
        Spritesheet *spriteSheet,
        int firstFrame,
        int lastFrame,
        int frameRate,
        int repeat,
        Display display,
        Vector2D position,
        PlayUntil playUntil) {
    Animation *a = malloc(sizeof(Animation));
    a->name = name;
    a->type = type;
    a->spriteSheet = spriteSheet;
    a->firstFrame = firstFrame;
    a->lastFrame = lastFrame;
    a->currentFrame = firstFrame;
    a->frameRate = frameRate;
    a->repeat = repeat;
    a->frameRateCount = 0;
    a->isPlaying = false;
    a->display = display;
    a->position = position;
    a->playUntil = playUntil;
    return a;
}

Animation *cloneAnimation(Animation *a) {
    return createAnimation(
            a->name,
            a->type,
            a->spriteSheet,
            a->firstFrame,
            a->lastFrame,
            a->frameRate,
            a->repeat,
            a->display,
            a->position,
            a->playUntil);
}

void drawAnimation(const Animation *a, Vector2 position) {
    drawImageFromSprite(a->spriteSheet, position, a->currentFrame);
}

Animation *findAnimation(Animation *animation[MAX_ANIMATIONS], AnimationType type) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animation[i] == NULL) {
            break;
        }
        if (animation[i]->type == type) {
            return animation[i];
        }
    }
    return NULL;
}

AnimationType getAnimationTypeFromString(const char *animationType) {
    for (int i = 0; i < sizeof(AnimationTypes) / sizeof(char *); i++) {
        if (strcmp(AnimationTypes[i], animationType) == 0) {
            return AnimationTypeEnums[i];
        }
    }
    addError("unknown animation type :: %s", animationType);
    exit(1);
}

AnimationType getAnimationTypeFromDirection(Direction direction) {
    if (direction == DIRECTION_UP) {
        return ANIMATION_UP;
    } else if (direction == DIRECTION_DOWN) {
        return ANIMATION_DOWN;
    } else if (direction == DIRECTION_LEFT) {
        return ANIMATION_LEFT;
    } else if (direction == DIRECTION_RIGHT) {
        return ANIMATION_RIGHT;
    }
    addError("unknown direction for animation type :: %d", direction);
    exit(1);
}
