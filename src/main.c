#include "header.h"

int main(void) {
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
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