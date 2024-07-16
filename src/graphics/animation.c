#include <mm_malloc.h>
#include <string.h>
#include "headers/util/log.h"
#include "headers/graphics/ui/ui.h"
#include "headers/graphics/spritesheet.h"
#include "headers/context.h"

const int MOVE_KEYS[] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

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

typedef struct {
    Context *context;
    Animation *library[MAX_ANIMATIONS_IN_GAME];
    int libraryCount;
    Animation *animations[MAX_ANIMATIONS];
    int animationCount;
} AnimationManager;

AnimationManager *createAnimationManager(Context *c) {
    AnimationManager *animationManager = malloc(sizeof(AnimationManager));
    animationManager->context = c;
    animationManager->animationCount = 0;
    animationManager->libraryCount = 0;
    return animationManager;
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

void processAnimations(AnimationManager *am) {
    for (int i = 0; i < am->animationCount; i++) {
        if (am->animations[i]->isPlaying) {
            incrementAnimationFrame(
                    am->animations[i],
                    (float) am->context->ui->screen->targetFrameRate);
        }
    }
}

void addAnimation(AnimationManager *am, Animation *a) {
    addDebug("add animation to manager :: %s, %s, %d",
            a->name, a->spriteSheet->name, am->animationCount);
    am->animations[am->animationCount] = a;
    am->animationCount++;
}

void addAllAnimations(AnimationManager *am, Animation *animations[MAX_ANIMATIONS]) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == NULL) {
            break;
        }
        addAnimation(am, animations[i]);
    }
}

void clearAnimations(AnimationManager *am) {
    memset(am->animations, 0, sizeof(am->animations));
    am->animationCount = 0;
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
    a->isPlaying = 0;
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

int loadAnimationsByName(AnimationManager *am, const char *name, Animation *animations[MAX_ANIMATIONS]) {
    memset(animations, 0, sizeof(Animation));
    int count = 0;
    for (int i = 0; i < am->libraryCount; i++) {
        if (strcmp(am->library[i]->name, name) == 0) {
            animations[count] = cloneAnimation(am->library[i]);
            count++;
        }
    }
    return count;
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
