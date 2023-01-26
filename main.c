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

#include "raylib.h"
#include "sprite.c"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void incrementFrame(struct Sprite *sp);
void incrementFrameCounter(struct Sprite *sp);

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;
    SetTargetFPS(60);

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - sprite anim");

    struct Sprite sp = createTestHumanoid();

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) move(&sp, DIRECTION_RIGHT);
        else if (IsKeyDown(KEY_LEFT)) move(&sp, DIRECTION_LEFT);
        else if (IsKeyDown(KEY_UP)) move(&sp, DIRECTION_UP);
        else if (IsKeyDown(KEY_DOWN)) move(&sp, DIRECTION_DOWN);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextureRec(sp.source, sp.frameRec, sp.position, WHITE);

        EndDrawing();
    }

    UnloadTexture(sp.source);

    CloseWindow();

    return 0;
}