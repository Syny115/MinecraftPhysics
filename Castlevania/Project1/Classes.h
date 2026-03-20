#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <string>
#include <queue>
using namespace std;
class GameManager {
private:
	// Scene scene; (Need Scene class)
	//Game* gamePointer; (Need game class)
	const float screenWidth = 800;
	const float screenHeight = 700;

	const float viewportWidth = 256;
	const float viewportHeight = 224;

	const int worldWidth = 256;
	const int worldHeight = 240;

public:
	/* Scene getActiveScene() {
	return game.Scene[currentLevel];
}*/
	float getScreenWidth() {
		return screenWidth;
	}
	float getScreenHeight() {
		return screenHeight;
	}
	float getViewportWidth() {
		return viewportWidth; 
	}
	float getViewportHeight() {
		return getViewportHeight();
	}

};



