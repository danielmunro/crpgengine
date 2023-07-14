typedef struct {
    Texture2D source;
    int frameWidth;
    int frameHeight;
} SpriteSheet;

typedef struct {
    SpriteSheet *spriteSheet;
    int type;
    int firstFrame;
    int lastFrame;
    int currentFrame;
    int repeat;
    int frameRate;
    int frameRateCount;
    bool isPlaying;
} Animation;

typedef struct {
    Animation *library[MAX_ANIMATIONS_IN_GAME];
    int libraryCount;
    Animation *animations[MAX_ANIMATIONS];
    int animationCount;
    Log *log;
} AnimationManager;

AnimationManager *createAnimationManager(Log *log) {
    AnimationManager *animationManager = malloc(sizeof(AnimationManager));
    animationManager->animationCount = 0;
    animationManager->log = log;
    return animationManager;
}

void incrementAnimationFrame(Animation *a) {
    a->frameRateCount++;
    if (a->frameRateCount >= (TARGET_FRAMERATE / a->frameRate)) {
        a->frameRateCount = 0;
        a->currentFrame++;
        if (a->currentFrame > a->lastFrame) {
            a->currentFrame = a->firstFrame;
        }
    }
}

void processAnimations(AnimationManager *am) {
    addDebug(am->log, "process animations");
    for (int i = 0; i < am->animationCount; i++) {
        if (am->animations[i]->isPlaying) {
            incrementAnimationFrame(am->animations[i]);
        }
    }
}

void addAnimation(AnimationManager *am, Animation *a) {
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

SpriteSheet *createSpriteSheet(const char *filename, int width, int height) {
    Texture2D tex = LoadTexture(filename);
    SpriteSheet *sp = malloc(sizeof(SpriteSheet));
    sp->source = tex;
    sp->frameWidth = width;
    sp->frameHeight = height;
    return sp;
}

Animation *createAnimation(
        SpriteSheet *spriteSheet,
        int type,
        int firstFrame,
        int lastFrame,
        int frameRate,
        int repeat) {
    Animation *a = malloc(sizeof(Animation));
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

void drawAnimation(Animation *a, Vector2 position) {
    int xFrames = a->spriteSheet->source.width / a->spriteSheet->frameWidth;
    int x = a->currentFrame % xFrames;
    int y = a->currentFrame / xFrames;
    Rectangle rect = {
            (float) (x * a->spriteSheet->frameWidth),
            (float) (y * a->spriteSheet->frameHeight),
            (float) a->spriteSheet->frameWidth,
            (float) a->spriteSheet->frameHeight,
    };
    DrawTextureRec(
            a->spriteSheet->source,
            rect,
            position,
            WHITE
    );
}

Animation *findAnimation(Animation *animation[MAX_ANIMATIONS], int direction) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animation[i] == NULL) {
            break;
        }
        if (animation[i]->type == direction) {
            return animation[i];
        }
    }
    return NULL;
}

AnimationType getAnimationTypeFromName(const char *name) {
    for (int i = 0; i < ANIMATION_TYPE_COUNT; i++) {
        if (strcmp(AnimationTypeStrings[i], name) == 0) {
            return ANIMATION_TYPES[i];
        }
    }
    printf("no animation id for name: %s\n", name);
    exit(1);
}
