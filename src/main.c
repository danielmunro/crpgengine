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
    mob->animations[0] = createAnimation("right", sp, 8, 11);
    Vector2 position = {100, 100};
    while (!WindowShouldClose()) {
        BeginDrawing();
        drawAnimation(findAnimation(mob->animations, "right"), position);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}