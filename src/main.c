#include "header.h"

int main(void) {
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cjrpgengine");
    struct Game *g = createGameInstance();
    SetWindowTitle(g->scenes[g->scene]->name);
    struct timeval start, end;
    while (!WindowShouldClose()) {
        gettimeofday(&start, NULL);
        checkInput(g->scenes[g->scene]);
        BeginDrawing();
        drawScene(g->scenes[g->scene]);
        EndDrawing();
        gettimeofday(&end, NULL);
        if (g->scenes[g->scene]->player->isMoving) {
            animateSprite(g->scenes[g->scene]->player->sprite);
        }
    }

    UnloadTexture(g->scenes[g->scene]->player->sprite->source);

    CloseWindow();

    return 0;
}