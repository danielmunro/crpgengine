#include "header.h"

//int showCollisions = 0;

int main(int argc, char *argv[]) {
//    int sceneIndex = 0;
//    for (int i = 1; i < argc; i++) {
//        if (argv[i][0] == '-') {
//            if (argv[i][1] == 'c') {
//                showCollisions = 1;
//            } else if (argv[i][1] == 's') {
//                sceneIndex = atoi(&argv[i][2]);
//            }
//        }
//    }
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");

    Mobile *mob = malloc(sizeof(Mobile));
    mob->direction = DIRECTION_DOWN;
    loadAnimations("resources/animations/fireas.txt", mob->animations);
    
    Vector2 position = {100, 100};
    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_DOWN)) {
            mob->direction = DIRECTION_DOWN;
        }
        if (IsKeyDown(KEY_UP)) {
            mob->direction = DIRECTION_UP;
        }
        if (IsKeyDown(KEY_LEFT)) {
            mob->direction = DIRECTION_LEFT;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            mob->direction = DIRECTION_RIGHT;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        int anim = getAnimationFromDirection(mob->direction);
        drawAnimation(findAnimation(mob->animations, anim), position);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}