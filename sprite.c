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

struct Sprite createTestHumanoid() {
    struct Sprite sp;

    sp.source = LoadTexture("resources/hatman.png");

    Vector2 position = { 350.0f, 280.0f };
    Rectangle frameRec = { 0.0f, 0.0f, HUMANOID_WIDTH, HUMANOID_HEIGHT };

    SetTargetFPS(60);

    sp.position = position;
    sp.frameRec = frameRec;
    sp.currentFrame = 0;
    sp.framesCounter = 0;
    sp.framesSpeed = 8;
    sp.direction = DIRECTION_DOWN;
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

void move(struct Sprite *sp, int direction) {
    sp->direction = direction;
    sp->framesCounter = sp->framesCounter + 1;
    if (sp->framesCounter > 10) {
        sp->framesCounter = 0;
        incrementFrame(sp);
    }
    if (direction == DIRECTION_UP) {
        sp->frameRec.y = HUMANOID_HEIGHT * 3;
        sp->position.y = sp->position.y - 1;
    } else if (direction == DIRECTION_DOWN) {
        sp->frameRec.y = 0;
        sp->position.y = sp->position.y + 1;
    } else if (direction == DIRECTION_LEFT) {
        sp->frameRec.y = HUMANOID_HEIGHT;
        sp->position.x = sp->position.x - 1;
    } else if (direction == DIRECTION_RIGHT) {
        sp->frameRec.y = HUMANOID_HEIGHT * 2;
        sp->position.x = sp->position.x + 1;
    }
}
