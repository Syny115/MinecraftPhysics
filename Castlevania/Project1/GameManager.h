#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Scene.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <string>
#include <queue>
using namespace std;


class GameManager {
public:
	static GameManager& getInstance()
	{
		static GameManager instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

private:
	GameManager() = default;


	Game* gamePointer = nullptr;
	const float screenWidth = 800;
	const float screenHeight = 700;

	const float viewportWidth = 256;
	const float viewportHeight = 224;

	enum weapons
	{
		EMPTY, DAGGER, AXE, HOLYWATER, CROSS, STOPWATCH
	};

	int playerHealth = 12;
	int enemyHealth;
	int score = 0;
	int whipLevel = 0;
	int subWeapon = EMPTY;
	int projectileUpgrade = 0; // 0 is un-upgraded, 1 is two projectiles and so on
	int projectileCount = 0; // actual amount of projectiles
	int ammo = 0;

	int cp = 0;

	float timeScale = 1.0f;

public:

	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	Scene* getActiveScene() {
		return gamePointer->getActiveScene();
	}

	void setGamePointer(Game* g) {
		gamePointer = g;
	}

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
		return viewportHeight;
	}

	Game* getGamePointer() {
		return gamePointer;
	}

	int getWhipLevel() { return whipLevel; }

	int getHealth() { return playerHealth; }

	void addHealth(int h) { playerHealth += h; }

	void maximizeHealth() { playerHealth = 12; }

	void addWhipLevel(int wl) { if (whipLevel < 2) whipLevel+=wl; }

	void changeSubWeapon(int sw) { subWeapon = sw; }

	void addScore(int sc) { score+=sc; }

	void resetScore() { score = 0; }

	void setProjectileUpgrade(int projcount) { projectileUpgrade = projcount; }

	void resetProjectiles() { projectileCount = 0; }

	int getProjectileUpgrade() { return projectileUpgrade; }

	int getProjectileCount() { return projectileCount; }
	void addProjectileCount(int i) { projectileCount += i; }

	void addAmmo(int a) { ammo += a; }

	void setTimeScale(float f) { timeScale = f; }
	float getTimeScale() { return timeScale; }

	int getCheck() { return cp; }
	void addCheck(int i) { cp += i; }

	friend class Player;
};



