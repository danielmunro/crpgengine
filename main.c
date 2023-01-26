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
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - sprite anim");

    struct Sprite sp;

    sp.source = LoadTexture("resources/scarfy.png");

    Vector2 position = { 350.0f, 280.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)sp.source.width/6, (float)sp.source.height };

    SetTargetFPS(60);

    sp.position = position;
    sp.frameRec = frameRec;
    sp.currentFrame = 0;
    sp.framesCounter = 0;
    sp.framesSpeed = 8;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        incrementFrameCounter(&sp);

        // Control frames speed
        if (IsKeyPressed(KEY_RIGHT)) sp.framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT)) sp.framesSpeed--;

        if (sp.framesSpeed > MAX_FRAME_SPEED) sp.framesSpeed = MAX_FRAME_SPEED;
        else if (sp.framesSpeed < MIN_FRAME_SPEED) sp.framesSpeed = MIN_FRAME_SPEED;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(sp.source, 15, 40, WHITE);
        DrawRectangleLines(15, 40, sp.source.width, sp.source.height, LIME);
        DrawRectangleLines(15 + (int)sp.frameRec.x, 40 + (int)sp.frameRec.y, (int)sp.frameRec.width, (int)sp.frameRec.height, RED);

        DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
        DrawText(TextFormat("%02i FPS", sp.framesSpeed), 575, 210, 10, DARKGRAY);
        DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);

        for (int i = 0; i < MAX_FRAME_SPEED; i++)
        {
            if (i < sp.framesSpeed) DrawRectangle(250 + 21*i, 205, 20, 20, RED);
            DrawRectangleLines(250 + 21*i, 205, 20, 20, MAROON);
        }

        DrawTextureRec(sp.source, sp.frameRec, sp.position, WHITE);  // Draw part of the texture

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(sp.source);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}