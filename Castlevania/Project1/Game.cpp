#include "Game.h"
#include "GameManager.h"


void Game::loadScene(Scene* newScene) {
    delete activeScene;
    activeScene = newScene;
    activeScene->start();
}

void Game::startGame() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const float screenWidth = 800;
    const float screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Castlevania");
    activeScene = new PlayableScene("resources/json/Test1.json");
    activeScene->start();
    
    


    //Audio
    InitAudioDevice();

    //Sound
    soundArray[0] = LoadSound("resources/audio/snd_whip.wav");

    //Music
    musicArray[0] = LoadMusicStream("resources/audio/VampireKiller.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   // activeScene.startScene();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //if (IsKeyPressed(KEY_SPACE)) publicPlaySound(0);      // Play WAV sound
        activeScene->updateScene();

        UpdateMusicStream(musicArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        activeScene->drawScene();
        

        // Debug


        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < 10; i++) {
        UnloadMusicStream(musicArray[i]);
        UnloadSound(soundArray[i]);
    }
    
    CloseAudioDevice();
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

Game::~Game() {
    delete activeScene;
}