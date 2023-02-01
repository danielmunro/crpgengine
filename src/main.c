#include "header.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "cjrpgengine");
    struct Game *g = createGameInstance();
    SetWindowTitle(g->scenes[g->scene]->name);

    while (!WindowShouldClose()) {
        checkInput(g->scenes[g->scene]);
        BeginDrawing();
        drawScene(g->scenes[g->scene]);
        EndDrawing();
    }

    UnloadTexture(g->scenes[g->scene]->player->sprite->source);

    CloseWindow();

    return 0;
}