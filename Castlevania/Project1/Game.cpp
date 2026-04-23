#include "Game.h"
#include "GameManager.h"


void Game::loadScene(Scene* newScene) {
    GameManager::getInstance().setBossStarted(false);
    delete activeScene;
    activeScene = newScene;
    activeScene->start();
}

void Game::requestSceneChange(Scene* newScene) {
    pendingScene = newScene;
}

void Game::requestSceneReload() {
    GameManager::getInstance().maximizeHealth();
    GameManager::getInstance().setWhipLevel(0);
    if (levels[levelIndex].scenePath[sceneIndex] == "Title") pendingScene =  new TitleScene();
    else pendingScene = new PlayableScene(activeScene->getPath());
}

void Game::requestNextLevel() {
    if (++sceneIndex >= levels[levelIndex].scenePath.size()) {
        sceneIndex = 0;
        if (++levelIndex >= levels.size()) {
            levelIndex = 0; //TODO, make this go to credits!
        }
    }
    if (levels[levelIndex].scenePath[sceneIndex] == "Title") requestSceneChange(new TitleScene());
    else requestSceneChange(new PlayableScene(levels[levelIndex].scenePath[sceneIndex]));
}

void Game::startGame() {
    //Audio
    InitAudioDevice();

    //Sound
    soundArray[AXE] = LoadSound("resources/audio/snd_axe.wav");
    soundArray[BREAK] = LoadSound("resources/audio/snd_break.wav");
    soundArray[CROSS] = LoadSound("resources/audio/snd_cross.wav");
    soundArray[DAGGER] = LoadSound("resources/audio/snd_dagger.wav");
    soundArray[HEART] = LoadSound("resources/audio/snd_heart.wav");
    soundArray[HURT] = LoadSound("resources/audio/snd_hurt.wav");
    soundArray[JAR] = LoadSound("resources/audio/snd_jar.wav");
    soundArray[TREASURE] = LoadSound("resources/audio/snd_treasure.wav");
    soundArray[WEAPON_PICK] = LoadSound("resources/audio/snd_weaponPick.wav");
    soundArray[WHIP] = LoadSound("resources/audio/snd_whip.wav");

    //Music
    musicArray[VAMPIRE_KILLER] = LoadMusicStream("resources/audio/mus_vampireKiller.wav");
    musicArray[VAMPIRE_KILLER].looping = true;
    musicArray[PLAYER_MISS] = LoadMusicStream("resources/audio/mus_playerMiss.wav");
    musicArray[PLAYER_MISS].looping = false;
    musicArray[BLACK_NIGHT] = LoadMusicStream("resources/audio/mus_blackNight.wav");
    musicArray[BLACK_NIGHT].looping = true;
    
    float pitch = 0.5f;

    // Initialization
    //--------------------------------------------------------------------------------------
    const float screenWidth = 800;
    const float screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Castlevania");
    activeScene = new TitleScene();
    activeScene->start();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   // activeScene.startScene();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (activeScene != nullptr)
        {
            // Update
            //----------------------------------------------------------------------------------
            activeScene->updateScene();
            //if (IsKeyPressed(KEY_F1)) GameManager::getInstance().setTimeScale(0.5f);
            //else if (IsKeyPressed(KEY_F2)) GameManager::getInstance().setTimeScale(1.0f);
            //else if (IsKeyPressed(KEY_F3)) GameManager::getInstance().setTimeScale(2.0f);
            //else if (IsKeyPressed(KEY_F4)) GameManager::getInstance().setTimeScale(4.0f);
            //else if (IsKeyPressed(KEY_F5)) requestNextLevel();
            //else if (IsKeyPressed(KEY_F7)) requestSceneReload();
            if (currentSong > -1) UpdateMusicStream(musicArray[currentSong]);
            //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        activeScene->drawScene();

            // Debug


            EndDrawing();
            //----------------------------------------------------------------------------------
        }


        if (pendingScene != nullptr) {
            loadScene(pendingScene);
            pendingScene = nullptr;
        }

    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < 10; i++) {
        UnloadMusicStream(musicArray[i]);
    }
    for (int i = 0; i < 10; i++) {
        UnloadSound(soundArray[i]);
    }
    
    CloseAudioDevice();
    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

Game::~Game() {
    delete activeScene;
}