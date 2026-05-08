#include "Game.h"
#include "GameManager.h"


void Game::loadScene(Scene* newScene) {
    publicPauseMusic();
    GameManager::getInstance().setBossStarted(false);
    delete activeScene;
    activeScene = newScene;
    activeScene->start();
    publicResumeMusic();
}

void Game::publicPlayLevelMusic() {
    printf("%d, is the area\n", GameManager::getInstance().getArea());
    if (activeScene == nullptr) return;
    if (activeScene->type != SceneType::PLAYABLE) return;
    switch (GameManager::getInstance().getArea()) {
    case 0:
        publicPlayMusic(VAMPIRE_KILLER);
        break;
    case 1:
        publicPlayMusicOffset(WALKING_ON_THE_EDGE, 6.390);
        break;
    case 2:
        publicPlayMusic(OUT_OF_TIME);
        break;
    case 3:
        publicPlayMusicOffset(STALKER, 12.885);
        break;
    case 4:
        publicPlayMusicOffset(WICKED_CHILD, 22.60);
        break;
    case 5:
        publicPlayMusic(MARIO);
        break;
    default:
        //publicPlayMusic(VAMPIRE_KILLER);
        break;
    }
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
    musicArray[STALKER] = LoadMusicStream("resources/audio/mus_stalker.wav");
    musicArray[STALKER].looping = true;
    musicArray[WICKED_CHILD] = LoadMusicStream("resources/audio/mus_wickedChild.wav");
    musicArray[WICKED_CHILD].looping = true;
    musicArray[OUT_OF_TIME] = LoadMusicStream("resources/audio/mus_outOfTime.wav");
    musicArray[OUT_OF_TIME].looping = true;
    musicArray[WALKING_ON_THE_EDGE] = LoadMusicStream("resources/audio/mus_walkingOnTheEdge.wav");
    musicArray[WALKING_ON_THE_EDGE].looping = true;
    musicArray[STAGE_CLEAR] = LoadMusicStream("resources/audio/mus_stageClear.wav");
    musicArray[STAGE_CLEAR].looping = true;
    musicArray[MARIO] = LoadMusicStream("resources/audio/mus_mario.wav");
    musicArray[MARIO].looping = true;

    
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
    HideCursor();
   // activeScene.startScene();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (activeScene != nullptr)
        {
            // Update
            //----------------------------------------------------------------------------------
            activeScene->updateScene();
            if (GameManager::getInstance().debugMode) {
                if (IsKeyPressed(KEY_F1)) GameManager::getInstance().setTimeScale(0.5f);
                else if (IsKeyPressed(KEY_F2)) GameManager::getInstance().setTimeScale(1.0f);
                else if (IsKeyPressed(KEY_F3)) GameManager::getInstance().setTimeScale(2.0f);
                else if (IsKeyPressed(KEY_F4)) GameManager::getInstance().setTimeScale(4.0f);
                else if (IsKeyPressed(KEY_F5)) sceneMan.requestSceneLoad(SceneType::PLAYABLE);
                else if (IsKeyPressed(KEY_F7)) sceneMan.requestRoomExit(1);
                else if (IsKeyPressed(KEY_F8)) GameManager::getInstance().setBossStarted(true);
            }
            updateMusic();
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