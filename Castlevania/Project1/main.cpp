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
    const float screenWidth = 800;
    const float screenHeight = 700;

    const float viewportWidth = 256;
    const float viewportHeight = 224;

    InitWindow(screenWidth, screenHeight, "Castlevania");

    Camera2D camera = { 0 };
    vector<Rectangle> floorRec;
    Player player;

    camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = screenWidth/ viewportWidth;

    floorRec.push_back({ 0, 192, 256, 32 });
    floorRec.push_back({ 0, 160, 32, 32 });
    floorRec.push_back({ 224, 128, 32, 64 });
    floorRec.push_back({ 224-64, 128-32, 64+32, 32 });

    //Audio
    InitAudioDevice();

    //Sound
    soundArray[0] = LoadSound("resources/raylib_audio_resources/sound.wav");

    //Music
    //musicArray[0] = LoadMusicStream("resources/raylib_audio_resources/VampireKiller.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        camera.target = Vector2{ viewportWidth / 2, viewportHeight / 2 };//player.GetPosition().x + 20.0f, player.GetPosition().y + 20.0f };
        player.GroundCollision(floorRec);
        player.CeilingCollision(floorRec, floorRec.size());
        player.WallCollision(floorRec);
        player.Update();

        UpdateMusicStream(musicArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });
        BeginMode2D(camera);
            DrawRectangleRec(floorRec[0], BLUE);
            DrawRectangleRec(floorRec[1], BLUE);
            DrawRectangleRec(floorRec[2], BLUE);
            DrawRectangleRec(floorRec[3], BLUE);
            player.DrawPlayer();
        EndMode2D();

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
