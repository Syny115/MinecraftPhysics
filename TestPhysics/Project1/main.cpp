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
        float maxFALL =  20;
        int normalizedVelocity = 0;

        bool isOnFloor = true;
        bool wasOnFloor = true;

        bool leftBlocked = false;
        bool rightBlocked = false;

        const char* imageName = "HOUSE_MD.png";
        Texture2D sprite;

        Rectangle groundCollider = { 8, size-1, size-16, 4};
        Rectangle topCollider = { 8, +4, size-16, 2};
        Rectangle leftCollider = { 0+2, 10, 4, size-40};
        Rectangle rightCollider = { size-4, 10, 4, size-40};
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

        int CheckCollisionPointRecArr(Vector2 point, Rectangle* recs, int len) {
            for (int i = 0; i < len; i++) {
                if (CheckCollisionPointRec(point, recs[i])) return i;
            }
            return -1;
        }

        int CheckCollisionRecsArr(Rectangle rec1, Rectangle* recs2, int len) {
            for (int i = 0; i < len; i++) {
                    if (CheckCollisionRecs(rec1, recs2[i])) return i;
            }
            return -1;
        }

        void GroundCollision(Rectangle floorRec) {
            wasOnFloor = isOnFloor;
            if (CheckCollisionRecs(groundCollider, floorRec)) {
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

        void GroundCollision(Rectangle* floorRec, int len) {
            wasOnFloor = isOnFloor;
            int i = CheckCollisionRecsArr(groundCollider, floorRec, len);
            if (i != -1) {
                isOnFloor = true;
                position.y = floorRec[i].y - size;
                if (velocity.y > 0) {
                    velocity.y = 0;
                }
            }
            else {
                isOnFloor = false;
            }
        }

        void CeilingCollision(Rectangle* ceilingRec, int len) {
            int i = CheckCollisionRecsArr(topCollider, ceilingRec, len);
            if (i != -1 && velocity.y < 0) {
                velocity.y = 0;
            }
        }

        void WallCollision(Rectangle* wallRec, int len) {
            int i = CheckCollisionRecsArr(leftCollider, wallRec, len);
            if (i != -1) {
                leftBlocked = true;
                position.x += GetFrameTime()*80;
                if (velocity.x < 0) {
                    velocity.x = 0;
                }
            }
            else {
                leftBlocked = false;
            }
            int j = CheckCollisionRecsArr(rightCollider, wallRec, len);
            if (j != -1) {
                rightBlocked = true;
                position.x -= GetFrameTime()*80;
                if (velocity.x > 0) {
                    velocity.x = 0;
                }
            }
            else {
                rightBlocked = false;
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
            


            if (!isOnFloor && velocity.y < maxFALL) {
                velocity.y += grav;
            }
            if (wasOnFloor) {
                if (IsKeyPressed(KEY_SPACE)) velocity.y -= jumpForce;
            }
            if (IsKeyDown(KEY_SPACE)) {
                grav = 1;
            }
            else {
                grav = 2;
            }


            if (abs(velocity.x) < 0.1) {
                velocity.x = 0;
            }
           

            position = Vector2Add(position, velocity);
            position.x = Clamp(position.x, 0, GetScreenWidth()-size);
            position.y = Clamp(position.y, 0, GetScreenHeight());
            groundCollider.x = position.x+8 - GetInpuAxis() * 8;
            groundCollider.y = position.y+ size -1;
            topCollider.x = position.x+8;
            topCollider.y = position.y+4;
            leftCollider.x = position.x-1;
            leftCollider.y = position.y+10;
            rightCollider.x = position.x+size-1;
            rightCollider.y = position.y+10;

            if (velocity.y > 10) {
                leftCollider.height = size - 40;
                rightCollider.height = size - 40;
            }
            else {
                leftCollider.height = size - 20;
                rightCollider.height = size - 20;
            }
        }

        void DrawPlayer() {
            rec = { position.x, position.y, size, size };
            DrawRectangleRec(rec, DARKGREEN);
            DrawTextureV(sprite, position, WHITE);
            //DrawRectangleRec(groundCollider, RED);
            //DrawRectangleRec(topCollider, RED);
            //DrawRectangleRec(leftCollider, RED);
            //DrawRectangleRec(rightCollider, RED);
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
    Rectangle floorRec[4];
    Player player;

    floorRec[0] = { 0, 350, 800, 100 };
    floorRec[1] = { 80, 250, 50, 100 };
    floorRec[2] = { 400, 250, 50, 100 };
    floorRec[3] = { 450, 150, 50, 100 };


    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here



        player.GroundCollision(floorRec, sizeof(floorRec) / sizeof(floorRec[0]));
        player.CeilingCollision(floorRec, sizeof(floorRec) / sizeof(floorRec[0]));
        player.WallCollision(floorRec, sizeof(floorRec) / sizeof(floorRec[0]));
        player.Update();

        if (IsKeyDown(KEY_DOWN)) floorRec[0].y++;
        if (IsKeyDown(KEY_UP)) floorRec[0].y--;

        if (IsKeyPressed(KEY_SPACE))
            PlaySound(soundArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GREEN);

        DrawText("Cuddy, I need moere vicodon!", 190, 200, 20, LIGHTGRAY);
        
        DrawRectangleRec(floorRec[0], BLUE);
        DrawRectangleRec(floorRec[1], BLUE);
        DrawRectangleRec(floorRec[2], BLUE);
        DrawRectangleRec(floorRec[3], BLUE);
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