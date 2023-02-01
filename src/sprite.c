#include "raylib.h"

#define HUMANOID_WIDTH     32.0f
#define HUMANOID_HEIGHT    48.0f
#define DIRECTION_UP       1
#define DIRECTION_DOWN     2
#define DIRECTION_LEFT     3
#define DIRECTION_RIGHT    4

typedef struct Sprite {
    Texture2D source;
    Vector2 position;
    Rectangle frameRec;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    int direction;
} Sprite;

Sprite *createTestHumanoid() {
    struct Sprite *sp = malloc(sizeof(Sprite));

    sp->source = LoadTexture("resources/hatman.png");

    Vector2 position = { ((float)SCREEN_WIDTH / 2) - (HUMANOID_WIDTH / 2), ((float)SCREEN_HEIGHT / 2) - (HUMANOID_HEIGHT / 2) };
    Rectangle frameRec = { 0.0f, 0.0f, HUMANOID_WIDTH, HUMANOID_HEIGHT };

    sp->position = position;
    sp->frameRec = frameRec;
    sp->currentFrame = 0;
    sp->framesCounter = 0;
    sp->framesSpeed = 8;
    sp->direction = DIRECTION_DOWN;
    return sp;
}

void incrementFrame(struct Sprite *sp) {
    sp->framesCounter = 0;
    sp->currentFrame++;

    if (sp->currentFrame > 3) {
        sp->currentFrame = 0;
    }

    sp->frameRec.x = (float) sp->currentFrame * HUMANOID_WIDTH;
}

void animateSprite(Sprite *sp) {
    sp->framesCounter = sp->framesCounter + 1;
    if (sp->framesCounter > 10) {
        sp->framesCounter = 0;
        incrementFrame(sp);
    }
    if (sp->direction == DIRECTION_UP) {
        sp->frameRec.y = HUMANOID_HEIGHT * 3;
    } else if (sp->direction == DIRECTION_DOWN) {
        sp->frameRec.y = 0;
    } else if (sp->direction == DIRECTION_LEFT) {
        sp->frameRec.y = HUMANOID_HEIGHT;
    } else if (sp->direction == DIRECTION_RIGHT) {
        sp->frameRec.y = HUMANOID_HEIGHT * 2;
    }
}

void drawSprite(Sprite *sp) {
    DrawTextureRec(
            sp->source,
            sp->frameRec,
            sp->position,
            WHITE
    );
}
