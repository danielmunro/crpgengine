#include "header.h"

int showCollisions = 0;

int main(int argc, char *argv[]) {
    int sceneIndex = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                showCollisions = 1;
            } else if (argv[i][1] == 's') {
                sceneIndex = atoi(&argv[i][2]);
            }
        }
    }
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");

    Game *g = createGameInstance(sceneIndex, showCollisions);
    run(g);

//    while (!WindowShouldClose()) {
//        mob->isMoving = 0;
//        if (IsKeyDown(KEY_DOWN)) {
//            mob->direction = DIRECTION_DOWN;
//            mob->position.y += 1;
//            mob->isMoving = 1;
//        }
//        if (IsKeyDown(KEY_UP)) {
//            mob->direction = DIRECTION_UP;
//            mob->position.y -= 1;
//            mob->isMoving = 1;
//        }
//        if (IsKeyDown(KEY_LEFT)) {
//            mob->direction = DIRECTION_LEFT;
//            mob->position.x -= 1;
//            mob->isMoving = 1;
//        }
//        if (IsKeyDown(KEY_RIGHT)) {
//            mob->direction = DIRECTION_RIGHT;
//            mob->position.x += 1;
//            mob->isMoving = 1;
//        }
//        BeginDrawing();
//        ClearBackground(BLACK);
//        int animName = getAnimationFromDirection(mob->direction);
//        Animation *anim = findAnimation(mob->animations, animName);
//        drawAnimation(anim, mob->position);
//        if (mob->isMoving) {
//            incrementAnimFrame(anim);
//        }
//        EndDrawing();
//    }
//    CloseWindow();
    return 0;
}