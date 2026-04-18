#pragma once
#include "raylib.h"
#include <vector>
#include "Scene.h"

struct Level
{
	vector<const char*> scenePath;
};

class Game {
private:
	Scene* activeScene = nullptr;
	Scene* pendingScene = nullptr;
	Sound soundArray[10];
	Music musicArray[10];
	int currentSong = -1;


	int levelIndex = 0, sceneIndex = 0;

public:

	enum MUSIC
	{
		VAMPIRE_KILLER, PLAYER_MISS
	};

	enum SOUND
	{
		WHIP, BREAK, CROSS, DAGGER, HEART, JAR, TREASURE, WEAPON_PICK, AXE, HURT
	};

	vector<Level> levels;
	Game() {
		levels.push_back({ {"resources/json/Level1_Scene1.json", "resources/json/Level1_Scene2.json", "resources/json/Level1_Scene3.json", "resources/json/Level1_Scene2.json"}});
		levels.push_back({ {"resources/json/Test1.json", "resources/json/test2.json", "resources/json/Test3.json"} });
	}
	~Game();
	void startGame();
	void loadScene(Scene* newScene); //DO NOT CALL!
	void requestSceneChange(Scene* newScene);
	void requestSceneReload();
	void requestNextLevel();

	Scene* getActiveScene() { return activeScene; }	

	void publicPlaySound(int i) {
			PlaySound(soundArray[i]);
	}

	void publicPauseMusic() {
		if (currentSong > -1) PauseMusicStream(musicArray[currentSong]);
	}

	void publicResumeMusic() {
		if (currentSong > -1)
		ResumeMusicStream(musicArray[currentSong]);

	}

	void publicPlayMusic(int i) {
		if (i == currentSong) return;
		if (currentSong > -1) StopMusicStream(musicArray[currentSong]);
		PlayMusicStream(musicArray[i]);
		currentSong = i;
		
			
	}

	void publicPlayLevelMusic() {
		switch (levelIndex) {
		case 0:
			publicPlayMusic(VAMPIRE_KILLER);
			break;
		default:
			publicPlayMusic(VAMPIRE_KILLER);
			break;
		}
	}

	int getLevelIndex() { return levelIndex; }
	int getSceneIndex() { return sceneIndex; }
};