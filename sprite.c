#include "raylib.h"

struct Sprite {
    Texture2D source;
    Vector2 position;
    Rectangle frameRec;
    int currentFrame;
    int framesCounter;
    int framesSpeed;
};

void incrementFrame(struct Sprite *sp) {
    sp->framesCounter = 0;
    sp->currentFrame++;

    if (sp->currentFrame > 5) sp->currentFrame = 0;

    sp->frameRec.x = (float)sp->currentFrame*(float)sp->source.width/6;
}

void incrementFrameCounter(struct Sprite *sp) {
    sp->framesCounter++;

    if (sp->framesCounter >= (60/sp->framesSpeed))
    {
        incrementFrame(sp);
    }
}
