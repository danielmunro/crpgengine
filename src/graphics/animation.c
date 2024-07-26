#include <mm_malloc.h>
#include "headers/graphics/ui/ui.h"
#include "headers/graphics/spritesheet.h"
#include "headers/context.h"

typedef struct {
    Context *context;
    const char *name;
    Direction type;
    Spritesheet *spriteSheet;
    int firstFrame;
    int lastFrame;
    int currentFrame;
    int repeat;
    int frameRate;
    int frameRateCount;
    bool isPlaying;
} Animation;

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
        Direction type,
        Spritesheet *spriteSheet,
        int firstFrame,
        int lastFrame,
        int frameRate,
        int repeat) {
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
            a->repeat);
}

void drawAnimation(const Animation *a, Vector2 position) {
    drawImageFromSprite(a->spriteSheet, position, a->currentFrame);
}

Animation *findAnimation(Animation *animation[MAX_ANIMATIONS], Direction type) {
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
