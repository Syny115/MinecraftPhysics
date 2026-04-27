#pragma once
#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "UI.h"
#include "SceneManager.h"

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

	
	~Game();
	

	
	void startGame();
	void loadScene(Scene* newScene); //DO NOT CALL!
	

	bool sceneRequested() {
		if (pendingScene != nullptr) return true;
		return false;
	}

	void setPendingScene(Scene* newScene) { pendingScene = newScene; }


	SceneManager sceneMan;

	Scene* getActiveScene() { return activeScene; }	

	void publicPlaySound(int i) {
		if (!IsAudioDeviceReady()) return;
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

	void publicPlayLevelMusic();

	int getLevelIndex() { return levelIndex; }
	int getSceneIndex() { return sceneIndex; }

};