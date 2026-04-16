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
	int currentLevel = 0;
	Sound soundArray[10];
	Music musicArray[10];

	int levelIndex = 0, sceneIndex = 0;

public:
	vector<Level> levels;
	Game() {
		levels.push_back({ {"resources/json/Level1_Scene1.json", "resources/json/Level1_Scene2.json", "resources/json/Level1_Scene3.json", "resources/json/Level1_Scene2.json"}});
		levels.push_back({ {"resources/json/Test1.json", "resources/json/test2.json", "resources/json/Test3.json"} });
	}
	~Game();
	void startGame();
	void loadScene(Scene* newScene); //DO NOT CALL!
	void requestSceneChange(Scene* newScene);
	void requestNextLevel();

	Scene* getActiveScene() { return activeScene; }	

	void publicPlaySound(int i) {
		PlaySound(soundArray[i]);
	}

	int getLevelIndex() { return levelIndex; }
	int getSceneIndex() { return sceneIndex; }
};