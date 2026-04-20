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

	bool bossStarted;

	enum weapons
	{
		EMPTY, DAGGER, AXE, HOLYWATER, CROSS, STOPWATCH
	};

	int playerHealth = 16;
	int enemyHealth;
	int score = 0;
	int whipLevel = 0;
	int subWeapon = EMPTY;
	int projectileUpgrade = 0; // 0 is un-upgraded, 1 is two projectiles and so on
	int projectileCount = 0; // actual amount of projectiles
	int ammo = 0;

	int cp = 0;

	float timeScale = 1.0f;

	//Lives but not yet

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

	int getBossHealth() { return enemyHealth; }

	int getPlayerHealth() { return playerHealth; }

	int getScore() { return score; }

	int getWhipLevel() { return whipLevel; }
	void setWhipLevel(int n) { whipLevel = n; }

	int getHealth() { return playerHealth; }

	void addHealth(int h) { playerHealth += h; }

	void maximizeHealth() { playerHealth = 16; }

	void addWhipLevel(int wl) { if (whipLevel < 2) whipLevel+=wl; gamePointer->publicPlaySound(Game::WEAPON_PICK);}

	void changeSubWeapon(int sw) { subWeapon = sw; gamePointer->publicPlaySound(Game::WEAPON_PICK);}

	int getSubWeapon() { return subWeapon; }

	void addScore(int sc) { score+=sc; }

	void resetScore() { score = 0; }

	void setProjectileUpgrade(int projcount) { projectileUpgrade = projcount; }

	void resetProjectiles() { projectileCount = 0; }

	int getProjectileUpgrade() { return projectileUpgrade; }

	int getProjectileCount() { return projectileCount; }
	void addProjectileCount(int i) { projectileCount += i; }

	void addAmmo(int a) { ammo += a; gamePointer->publicPlaySound(Game::HEART); }

	int getAmmo() { return ammo; }

	void setTimeScale(float f) { timeScale = f; }
	float getTimeScale() { return timeScale; }

	int getCheck() { return cp; }
	void addCheck(int i) { cp += i; }

	void setBossStarted(bool b) { bossStarted = b; }
	bool getBossStarted() { return bossStarted; }

	int* getBossHealthPointer() { return &enemyHealth; }

	friend class Player;
};



