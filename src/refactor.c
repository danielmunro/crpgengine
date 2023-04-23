typedef struct SpriteSheet {
    Texture2D source;
    int frameWidth;
    int frameHeight;
} SpriteSheet;

typedef struct Animation {
    SpriteSheet *spriteSheet;
    char name[255];
    int firstFrame;
    int lastFrame;
    int currentFrame;
    int frameRate;
    int frameRateCount;
} Animation;

Animation *createAnimation(char *name, SpriteSheet *spriteSheet, int firstFrame, int lastFrame) {
    Animation *a = malloc(sizeof(Animation));
    strcpy(a->name, name);
    a->spriteSheet = spriteSheet;
    a->firstFrame = firstFrame;
    a->lastFrame = lastFrame;
    a->currentFrame = firstFrame;
    a->frameRate = 20;
    a->frameRateCount = 0;
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
    a->frameRateCount++;
    if (a->frameRateCount > a->frameRate) {
        a->frameRateCount = 0;
        a->currentFrame++;
        if (a->currentFrame > a->lastFrame) {
            a->currentFrame = a->firstFrame;
        }
    }
}

Animation *findAnimation(Animation *animation[MAX_ANIMATIONS], char *name) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animation[i] == NULL) {
            break;
        }
        if (strcmp(animation[i]->name, name) == 0) {
            return animation[i];
        }
    }
    return NULL;
}

typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    int direction;
} Mobile;

Mobile loadAnimation(const char *file) {
    printf("%s", file);
    exit(1);
}
