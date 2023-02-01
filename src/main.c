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

        ClearBackground(RAYWHITE);

        DrawTextureRec(
            g->scenes[g->scene]->player->sprite->source,
            g->scenes[g->scene]->player->sprite->frameRec,
            g->scenes[g->scene]->player->sprite->position,
            WHITE
        );

        EndDrawing();
    }

    UnloadTexture(g->scenes[g->scene]->player->sprite->source);

    CloseWindow();

    return 0;
}