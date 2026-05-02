#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Scene.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;


class GameManager {
public:
	static GameManager& getInstance()
	{
		static GameManager instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	float sceneTime = 0;
	bool debugMode = false;
private:
	GameManager() {
		inventory["dagger"] = false;
		inventory["axe"] = false;
		inventory["holywater"] = false;
		inventory["cross"] = false;
		inventory["stopwatch"] = false;
		inventory["dash"] = false;
	}


	Game* gamePointer = nullptr;
	const float screenWidth = 800;
	const float screenHeight = 700;

	const float viewportWidth = 256;
	const float viewportHeight = 224;

	bool bossStarted = false;;

	enum weapons
	{
		EMPTY, DAGGER, AXE, HOLYWATER, CROSS, STOPWATCH
	};

	int playerHealth = 16;
	int enemyHealth = 0;
	int score = 0;
	int whipLevel = 0;
	int subWeapon = EMPTY;
	int projectileUpgrade = 0; // 0 is un-upgraded, 1 is two projectiles and so on
	int projectileCount = 0; // actual amount of projectiles
	int ammo = 0;
	int area = 0;

	int lastExit = 0;

	float timeScale = 1.0f;


	unordered_map<string, bool> inventory;

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

	void changeSubWeapon(bool dir) { 
		int mult = dir ? 1 : -1;
		const char* weaponKeys[] = { "", "dagger", "axe", "holywater", "cross", "stopwatch" };
		int total = 6;
		int next = subWeapon;

		for (int i = 1; i < total; i++) {
			int candidate = (subWeapon + mult * i + total) % total;
			if (candidate == EMPTY) {
				next = EMPTY; 
				break;
			}
			if (inventory[weaponKeys[candidate]]) {
				next = candidate;
				break;
			}
		}

		subWeapon = next;
	}

	void setSubWeapon(int sw) { subWeapon = sw; }

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

	int getLastExit() { return lastExit; }
	void setLastExit(int i) { lastExit = i; }

	void setBossStarted(bool b) { bossStarted = b; }
	bool getBossStarted() { return bossStarted; }

	int* getBossHealthPointer() { return &enemyHealth; }

	void setArea(int i) { area = i; }
	int getArea() { return area; }

	void setInvetory(string key, bool value) { inventory.at(key) = value; if (value) gamePointer->publicPlaySound(Game::WEAPON_PICK);}
	bool getInvetory(string key) { return inventory.at(key); }

	friend class Player;
};



