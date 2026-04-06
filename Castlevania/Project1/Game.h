#pragma once
#include "raylib.h"
#include <vector>
#include "Scene.h"


class Game {
private:
	Scene* activeScene = nullptr;
	int currentLevel = 0;
	Sound soundArray[10];
	Music musicArray[10];
public:
	~Game();
	void startGame();
	void loadScene(Scene* newScene);

	Scene* getActiveScene() { return activeScene; }	

	void publicPlaySound(int i) {
		PlaySound(soundArray[i]);
	}
};