#include "raylib.h"

typedef struct SpriteSheet {
    Texture2D source;
    int frameWidth;
    int frameHeight;
} SpriteSheet;

SpriteSheet *createSpriteSheet(char *filename, int width, int height) {
    Texture2D tex = LoadTexture(filename);
    SpriteSheet *sp = malloc(sizeof(SpriteSheet));
    sp->source = tex;
    sp->frameWidth = width;
    sp->frameHeight = height;
    return sp;
}

typedef struct Animation {
    SpriteSheet *spriteSheet;
    int type;
    int firstFrame;
    int lastFrame;
    int currentFrame;
    int repeat;
    int frameRate;
    int frameRateCount;
    int isPlaying;
} Animation;

int getAnimationFromDirection(int direction) {
    if (direction == DIRECTION_DOWN) {
        return ANIM_DOWN;
    } else if (direction == DIRECTION_UP) {
        return ANIM_UP;
    } else if (direction == DIRECTION_LEFT) {
        return ANIM_LEFT;
    } else if (direction == DIRECTION_RIGHT) {
        return ANIM_RIGHT;
    } else {
        printf("no animation for direction %d\n", direction);
        exit(1);
    }
}

Animation *createAnimation(SpriteSheet *spriteSheet, int type, int firstFrame, int lastFrame, int frameRate, int repeat) {
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

void incrementAnimFrame(Animation *a) {
    a->frameRateCount++;
    if (a->frameRateCount >= (TARGET_FRAMERATE / a->frameRate)) {
        a->frameRateCount = 0;
        a->currentFrame++;
        if (a->currentFrame > a->lastFrame) {
            a->currentFrame = a->firstFrame;
        }
    }
}

Animation *findAnimation(Animation *animation[MAX_ANIMATIONS], int type) {
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

int getAnimIdFromName(char *name) {
    if (strcmp(name, "up") == 0) {
        return ANIM_UP;
    } else if (strcmp(name, "down") == 0) {
        return ANIM_DOWN;
    } else if (strcmp(name, "left") == 0) {
        return ANIM_LEFT;
    } else if (strcmp(name, "right") == 0) {
        return ANIM_RIGHT;
    } else {
        printf("no animation id for name: %s\n", name);
        exit(1);
    }
}
