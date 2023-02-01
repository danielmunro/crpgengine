#include "header.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "cjrpgengine");

    struct Game *g = createGameInstance();

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) move(g->player->sprite, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) move(g->player->sprite, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) move(g->player->sprite, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) move(g->player->sprite, DIRECTION_DOWN);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(
            g->player->sprite->source,
            g->player->sprite->frameRec,
            g->player->sprite->position,
            WHITE
        );

        EndDrawing();
    }

    UnloadTexture(g->player->sprite->source);

    CloseWindow();

    return 0;
}