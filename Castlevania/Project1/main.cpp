#include "libraries/raylib/include/raylib.h"
#include "raylib.h"
#include "Player.h";
#include <stdio.h>
#include <math.h>
#include <vector>


Sound soundArray[10];
Music musicArray[10];

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Castlevania");

    vector<Rectangle> floorRec;
    Player player;

    floorRec.push_back({ 0, 350, 800, 100 });
    floorRec.push_back({ 80, 250, 50, 100 });
    floorRec.push_back({ 400, 250, 50, 100 });
    floorRec.push_back({ 450, 150, 50, 100 });

    //Audio
    InitAudioDevice();

    //Sound
    soundArray[0] = LoadSound("resources/raylib_audio_resources/sound.wav");

    //Music
    musicArray[0] = LoadMusicStream("resources/raylib_audio_resources/VampireKiller.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        player.GroundCollision(floorRec, floorRec.size());
        player.CeilingCollision(floorRec, floorRec.size());
        player.WallCollision(floorRec, floorRec.size());
        player.Update();

        UpdateMusicStream(musicArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });

        DrawRectangleRec(floorRec[0], BLUE);
        DrawRectangleRec(floorRec[1], BLUE);
        DrawRectangleRec(floorRec[2], BLUE);
        DrawRectangleRec(floorRec[3], BLUE);
        player.DrawPlayer();

        // Debug


        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseAudioDevice();

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
