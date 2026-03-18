#pragma once
#include "raylib.h"
#include <vector>
#include "Scene.h"


class Game {
private:
	Scene* activeScene;
	int currentLevel = 0;
public:
	~Game();
	void startGame();
	void loadScene(Scene* newScene);
};