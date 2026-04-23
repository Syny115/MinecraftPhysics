#pragma once
#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "UI.h"

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
	bool musFirstGo;

	int levelIndex = 0, sceneIndex = 0;
public:

	enum MUSIC
	{
		VAMPIRE_KILLER, PLAYER_MISS, BLACK_NIGHT
	};

	enum SOUND
	{
		WHIP, BREAK, CROSS, DAGGER, HEART, JAR, TREASURE, WEAPON_PICK, AXE, HURT
	};

	vector<Level> levels;
	Game() {
		levels.push_back({ {"Title", "Lore"}});
		levels.push_back({ {"resources/json/Level1_Scene1.json", "resources/json/Level1_Scene2.json", "resources/json/Level1_Scene3.json", "resources/json/Level1_Scene4.json"}});
	}
	~Game();
	void startGame();
	void loadScene(Scene* newScene); //DO NOT CALL!
	void requestSceneChange(Scene* newScene);
	void requestSceneReload();
	void requestNextLevel();

	bool sceneRequested() {
		if (pendingScene != nullptr) return true;
		return false;
	}

	Scene* getActiveScene() { return activeScene; }	

	void publicPlaySound(int i) {
		if (i < 0 || i >= 10) {
			TraceLog(LOG_ERROR, "publicPlaySound: invalid index %d", i);
			return;
		}
			PlaySound(soundArray[i]);
	}

	void publicPauseMusic() {
		if (currentSong > -1) PauseMusicStream(musicArray[currentSong]);
	}

	void publicResumeMusic() {
		if (currentSong > -1) ResumeMusicStream(musicArray[currentSong]);

	}

	void publicPlayMusic(int i) {
		if (i == currentSong) return;
		else if (i < 0) {
			StopMusicStream(musicArray[currentSong]);
			return;
		}
		if (currentSong > -1) StopMusicStream(musicArray[currentSong]);
		PlayMusicStream(musicArray[i]);
		currentSong = i;	
	}

	void publicPlayMusicOffset(int i, float time) {
		printf("%f / %f\n", GetMusicTimePlayed(musicArray[i]), GetMusicTimeLength(musicArray[i]));
		if (i == currentSong) {
			if (GetMusicTimePlayed(musicArray[i]) < time && !musFirstGo) SeekMusicStream(musicArray[i], time); //1.6 for blackNight
			if (GetMusicTimePlayed(musicArray[i]) >= GetMusicTimeLength(musicArray[i]) - GetFrameTime())
			{
				musFirstGo = false;
			}
			return;
		}
		musFirstGo = true;
		publicPlayMusic(i);
	}

	void publicPlayLevelMusic() {
		switch (levelIndex) {
		case 0:
			publicPlayMusic(-1);
			break;
		default:
			publicPlayMusic(VAMPIRE_KILLER);
			break;
		}
	}

	int getLevelIndex() { return levelIndex; }
	int getSceneIndex() { return sceneIndex; }

};