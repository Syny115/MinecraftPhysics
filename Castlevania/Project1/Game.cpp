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
    activeScene = new PlayableScene("resources/json/Test3.json");
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
        if (IsKeyPressed(KEY_F1)) GameManager::getInstance().setTimeScale(0.5f);
        else if (IsKeyPressed(KEY_F2)) GameManager::getInstance().setTimeScale(1.0f);
        else if (IsKeyPressed(KEY_F3)) GameManager::getInstance().setTimeScale(2.0f);
        else if (IsKeyPressed(KEY_F4)) GameManager::getInstance().setTimeScale(4.0f);
        UpdateMusicStream(musicArray[0]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        activeScene->drawScene();
        gui.renderUI();
        gui.updateUI();

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