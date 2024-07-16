#include "headers/graphics/animation.h"

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

void processAnimations(const AnimationManager *am) {
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

int loadAnimationsByName(const AnimationManager *am, const char *name, Animation *animations[MAX_ANIMATIONS]) {
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
