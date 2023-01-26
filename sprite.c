#include "raylib.h"

#define HUMANOID_WIDTH     32.0f
#define HUMANOID_HEIGHT    48.0f
#define DIRECTION_UP       1
#define DIRECTION_DOWN     2
#define DIRECTION_LEFT     3
#define DIRECTION_RIGHT    4

struct Sprite {
    Texture2D source;
    Vector2 position;
    Rectangle frameRec;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    int direction;
};

void incrementFrame(struct Sprite *sp) {
    sp->framesCounter = 0;
    sp->currentFrame++;

    if (sp->currentFrame > 3) {
        sp->currentFrame = 0;
    }

    sp->frameRec.x = (float) sp->currentFrame * HUMANOID_WIDTH;
}

void incrementFrameCounter(struct Sprite *sp) {
    sp->framesCounter++;

    if (sp->framesCounter >= (60 / sp->framesSpeed)) {
        incrementFrame(sp);
    }
}
