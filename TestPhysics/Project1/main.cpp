/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "libraries/raylib/include/raylib.h"
#include "raylib.h"
#include <iostream>
#include "raymath.h"
#include <math.h>

Sound soundArray[10];

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------



class Player {
    private:
        float jumpForce = 20;
        float grav = 1.5f;
        float acc = 20;
        float dec = 15;
        float airDec = 10;
        float size = 70;
        Rectangle rec;
        Vector2 position = {50, 60};
        Vector2 velocity = {0, 0};
        float maxSPD = 10;
        int normalizedVelocity = 0;

        bool isOnFloor = true;
        bool wasOnFloor = true;

        const char* imageName = "HOUSE_MD.png";
        Texture2D sprite;

        Vector2 groundCollider = { size * 0.5f , size };
    public:
        
        Player() {
            sprite = LoadTexture(imageName);
        }
        ~Player() {
            UnloadTexture(sprite);
        }

        Vector2 GetPosition() { return position; }
        Vector2 GetVelocity() { return velocity; }

        int GetInpuAxis() { return IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT); }

        void GroundCollision(Rectangle floorRec) {
            wasOnFloor = isOnFloor;
            if (CheckCollisionPointRec(groundCollider, floorRec)) {
                isOnFloor = true;
                position.y = floorRec.y-size;
                if (velocity.y > 0) {
                    velocity.y = 0;
                }
            }
            else {
                isOnFloor = false;
            }
        }

        void Update() {      
            if (velocity.x > 0) {
                normalizedVelocity = 1;
            }
            else if (velocity.x < 0) {
                normalizedVelocity = -1;
            }
            else {
                normalizedVelocity = 0;
            }

            if (abs(velocity.x) > maxSPD) {
                velocity.x = maxSPD*normalizedVelocity;
            }
            else {
                velocity.x += GetInpuAxis() * GetFrameTime()* acc;
                float _dec = dec;
                if (!isOnFloor){
                    _dec = airDec;
                }
                velocity.x -= _dec * GetFrameTime() * normalizedVelocity;
            }
            


            if (!isOnFloor) {
                velocity.y += grav;
            }
            if (wasOnFloor) {
                if (IsKeyPressed(KEY_SPACE)) velocity.y -= jumpForce;
            }



            if (abs(velocity.x) < 0.1) {
                velocity.x = 0;
            }
           

            position = Vector2Add(position, velocity);
            position.x = Clamp(position.x, 0, GetScreenWidth()-size);
            position.y = Clamp(position.y, 0, GetScreenHeight());
            groundCollider = Vector2Add(position, Vector2{ size * 0.5f, size });
        }

        void DrawPlayer() {
            rec = { position.x, position.y, size, size };
            DrawRectangleRec(rec, DARKGREEN);
            DrawTextureV(sprite, position, WHITE);
            DrawCircleV(groundCollider, 2, RED);
        }
};



int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();

    soundArray[0] = LoadSound("resources/raylib_audio_resources/sound.wav");

    Music music = LoadMusicStream("resources/raylib_audio_resources/country.mp3");

    music.looping = true;
    float pitch = 0.5f;

    PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    Rectangle floorRec = { 0, 350, 800, 100 };
    Player player;
    static double x= 120, y = 35;
    static double speed_x = 2, speed_y = 2;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here



        player.GroundCollision(floorRec);
        player.Update();
        if (IsKeyDown(KEY_RIGHT)) x += speed_x;
        if (IsKeyDown(KEY_LEFT)) x -= speed_x;
        if (IsKeyDown(KEY_DOWN)) floorRec.y++;
        if (IsKeyDown(KEY_UP)) floorRec.y--;

        if (y > screenHeight) y = screenHeight;
        if (y < 0) y = 0;
        if (x > screenWidth) x = screenWidth;
        if (x < 0) x = 0;

        if (IsKeyPressed(KEY_SPACE))
            PlaySound(soundArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Cuddy, I need moere vicodon!", 190, 200, 20, LIGHTGRAY);
        
        DrawRectangleRec(floorRec, BLUE);
        player.DrawPlayer();
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
// raylib example source code