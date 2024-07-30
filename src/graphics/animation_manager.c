#include "headers/graphics/animation.h"

typedef struct {
    Context *context;
    Animation **library;
    int libraryCount;
    Animation **animations;
    int animationCount;
} AnimationManager;

AnimationManager *createAnimationManager(Context *c) {
    AnimationManager *animationManager = malloc(sizeof(AnimationManager));
    animationManager->context = c;
    animationManager->animationCount = 0;
    animationManager->libraryCount = 0;
    animationManager->library = calloc(MAX_ANIMATIONS_IN_GAME, sizeof(Animation));
    animationManager->animations = calloc(MAX_ANIMATIONS, sizeof(Animation));
    return animationManager;
}

void processAnimations(const AnimationManager *am) {
    for (int i = 0; i < am->animationCount; i++) {
        if (am->animations[i]->isPlaying) {
            Animation *a = am->animations[i];
            incrementAnimationFrame(
                    a,
                    (float) am->context->ui->screen->targetFrameRate);
            if (a->display == DISPLAY_RELATIVE) {
                int ts = tileSize(am->context);
                int w = a->spriteSheet->frameWidth;
                int h = a->spriteSheet->frameHeight;
                Rectangle src = getRectForSpriteByIndex(
                        a->spriteSheet->source.width,
                        w,
                        h,
                        a->currentFrame);
                Rectangle dest = {
                        (((float) am->context->user->resolution.width / 2) + ((float) (a->position.x * ts) * am->context->ui->screen->scale)),
                        (((float) am->context->user->resolution.height / 2) + ((float) (a->position.y * ts) * am->context->ui->screen->scale)) - (float) am->context->game->tileSize,
                        (float) w * am->context->ui->screen->scale,
                        (float) h * am->context->ui->screen->scale,
                };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(
                        a->spriteSheet->source,
                        src,
                        dest,
                        origin,
                        0,
                        WHITE);
            }
        }
    }
}

void addAnimation(AnimationManager *am, Animation *a) {
    addInfo("add animation to manager :: %s, %s, %d",
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
    for (int i = 0; i < am->animationCount; i++) {
        am->animations[i] = NULL;
    }
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
