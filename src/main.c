#include "header.h"

int main(void) {
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
    struct Game *g = createGameInstance();
    SetWindowTitle(g->scenes[g->scene]->name);
    struct timeval start, end;
    long elapsed = 0;
    while (!WindowShouldClose()) {
        gettimeofday(&start, NULL);
        checkInput(g->scenes[g->scene]);
        BeginDrawing();
        drawScene(g->scenes[g->scene]);
        EndDrawing();
        gettimeofday(&end, NULL);
        elapsed = elapsed + (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
        if (elapsed > 10000) {
            elapsed = elapsed - 10000;
            animatePlayer(g->scenes[g->scene]->player);
        }
    }

    UnloadTexture(g->scenes[g->scene]->player->sprite->source);

    CloseWindow();

    return 0;
}