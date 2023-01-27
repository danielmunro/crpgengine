/*******************************************************************************************
*
*   raylib [textures] example - Sprite animation
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "header.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - sprite anim");

    struct Game *g = createGameInstance();

    // Main game loop
    while (!WindowShouldClose())
    {
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