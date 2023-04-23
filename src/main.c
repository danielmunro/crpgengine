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
//    struct Game *g = createGameInstance(sceneIndex, showCollisions);
//    Scene *s = getScene(g);
//    SetWindowTitle(s->name);
//    run(g);

//    loadAnimation("animations/fireas.txt");

    Texture2D tex = LoadTexture("resources/fireas.png");
    SpriteSheet *sp = malloc(sizeof(SpriteSheet));
    sp->source = tex;
    sp->frameWidth = 16;
    sp->frameHeight = 24;

    Mobile *mob = malloc(sizeof(Mobile));
    mob->direction = DIRECTION_DOWN;
    mob->animations[0] = createAnimation(ANIM_DOWN, sp, 0, 3);
    mob->animations[1] = createAnimation(ANIM_LEFT, sp, 4, 7);
    mob->animations[2] = createAnimation(ANIM_RIGHT, sp, 8, 11);
    mob->animations[3] = createAnimation(ANIM_UP, sp, 12, 15);

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
        drawAnimation(findAnimation(mob->animations, getAnimationFromDirection(mob->direction)), position);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}