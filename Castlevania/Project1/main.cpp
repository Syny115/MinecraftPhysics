#include "libraries/raylib/include/raylib.h"
#include "raylib.h"
#include <stdio.h>
#include <math.h>


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

    //Audio
    InitAudioDevice();

    //Sound
    soundArray[0] = LoadSound("resources/raylib_audio_resources/sound.wav");

    //Music
    musicArray[0] = LoadMusicStream("resources/raylib_audio_resources/country.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------


        UpdateMusicStream(musicArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

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
