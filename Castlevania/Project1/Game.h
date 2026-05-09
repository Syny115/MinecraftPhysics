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
	Sound soundArray[11];
	Music musicArray[10];
	int currentSong = -1;
	bool musFirstGo;
	float loopPoint;

	int levelIndex = 0, sceneIndex = 0;
public:
	enum MUSIC
	{
		VAMPIRE_KILLER, PLAYER_MISS, BLACK_NIGHT, STAGE_CLEAR, STALKER, WICKED_CHILD, OUT_OF_TIME, WALKING_ON_THE_EDGE, MARIO
	};

	enum SOUND
	{
		WHIP, BREAK, CROSS, DAGGER, HEART, JAR, TREASURE, WEAPON_PICK, AXE, HURT, RAISE_HEALTH
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
		if (i < 0 || i >= 11) {
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
		loopPoint = 0.0f;
		currentSong = i;	
	}


	void publicPlayMusicOffset(int i, float time) {
		if (i != currentSong) {
			musFirstGo = true;
			publicPlayMusic(i);
			loopPoint = time;
		}	
	}

	void updateMusic() {
		if (currentSong > -1) {
			UpdateMusicStream(musicArray[currentSong]);

			if (loopPoint == 0.0f) return;

			float played = GetMusicTimePlayed(musicArray[currentSong]);
			float length = GetMusicTimeLength(musicArray[currentSong]);
			printf("%f / %f\nmus first go: %d\n", played, length, musFirstGo);
			if (!musFirstGo && played >= length - 2 * GetFrameTime()) {
				SeekMusicStream(musicArray[currentSong], loopPoint);
				return;
			}

			if (musFirstGo && played >= loopPoint + GetFrameTime()) {
				musFirstGo = false;
			}
		}
	}

	void publicPlayLevelMusic();

	int getLevelIndex() { return levelIndex; }
	int getSceneIndex() { return sceneIndex; }

};