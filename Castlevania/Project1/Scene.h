#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Player.h";
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include "libraries/json.hpp"
#include "SpriteRenderer.h"
#include "Classes.h"
#include "DestructableObject.h"
#include "Enemy.h"
#include "Loot.h"
#include "Projectile.h"
#include "UI.h"
#include "Savepoint.h"

using json = nlohmann::json;
using namespace std;

class Scene {
protected:
	Camera2D camera = { 0 };
	queue<Entity*> deletionQueue;
	UI ui;
	bool setup = false;
public:
	SceneType type;
	Scene();
	virtual ~Scene() {}
	virtual void start();
	virtual void updateScene();
	virtual void updateCamera();
	virtual void drawScene();
	Camera2D getCamera() { return camera; }

	void deleteMe(Entity* e) {
		deletionQueue.push(e);
	}


	//So that the game doesnt go wonky when trying to access playable scene funcions
	virtual void setDebugMessage(string message, int count) { /* I | Ii | II | I- */ }
	virtual float getWorldHeight() { return 0; }
	virtual float getWorldWidth() { return 0; }
	virtual int getTileHeight() { return 0; }
	virtual int getTileWidth() { return 0; }
	virtual const char* getPath() { return ""; }

	virtual void pushPlayerHitBoxes(damageRect hitBox) { /* I | Ii | II | I- */ }
	virtual void removePlayerHitBoxes(Rectangle* hitBox) { /*I AM AT A LOSS*/ }
	virtual void pushLoot(Loot* l) {}
	virtual void removeLoot(Loot* l) {}
	virtual void removeDestructables(DestructableObject* d) {}
	virtual void pushSolidRects(Rectangle rect) {}
	virtual void removeSolidRects(Rectangle rect) {}
	virtual void pushEnemyRects(damageRect hitBox) {}
	virtual void removeEnemy(Enemy* e) {}
	virtual void removeEnemyRects(Rectangle* hitbox) {}
	virtual void removeAllEnemies() {}

	virtual void pushProjectile(Projectile* p) {}
	virtual void removeProjectile(Projectile* p) {}
	virtual Player* getPlayer() { return nullptr; }

	virtual float getTimeLeft() { return 0; }
	virtual void resetTimeLeft() {}

	virtual void spawnParticle(Vector2 position) {}
	virtual void spawnHitEffect(Vector2 position) {}

	virtual Vector2 getBossStart() { return { 0, 0 }; }


};





class PlayableScene : public Scene {
private:
	const char* path;
	Player* player;
	SavePoint* savePoint;
	vector<Rectangle> solidRects;
	vector<damageRect> enemyRects;
	vector<Rectangle> lootRects;
	vector<Rectangle> destructableRects;
	vector<damageRect> playerHitBoxes;
	vector<Loot*> lootitems;

	Vector2 spawnponits[5];
	Vector2 spawnpoint;
	Rectangle exits[4];
	Vector2 bossStart = {-1, 0};
	vector<staircase> stairs;
	vector<DestructableObject*> destructables;
	vector<Enemy*> enemies;

	vector<Rectangle> zombieSpawners;
	vector<Rectangle> medusaSpawners;
	vector<Rectangle> mermanSpawners;
	vector<Vector2> batSpawners;
	vector<Vector2> shieldSpawners;
	vector<Vector2> spearSpawners;
	vector<Vector2> pantherSpawners;
	Vector2 bossSpawner;

	vector<Projectile*> projectiles;

	Timer spawnCoolDown{ 3 };

	float worldWidth;
	float worldHeight;
	int tileHeight;
	int tileWidth;

	vector<vector<int>> tileMat;
	Texture2D tileAtlas = LoadTexture("resources/sprites/CastlevaniaTileset.png");

	int enemyCap = 10;

	string debug_text1;
	string debug_text2;

	
	Timer *timeLeft;
	//particles for enemies death
	struct Particle {
		Vector2 position;
		Vector2 velocity;
		float timer;
		float duration;
		int frame;
		float frameTimer;
		SpriteRenderer* sprite;
	};
	vector<Particle> particles;

	void debugEnemySpawn();
	int debugEnemySelected = 0;
	SpriteRenderer* debugTile;
	Vector2 mousePos;

public:
	PlayableScene(const char* path, int spawn);
	~PlayableScene();

	void start() override;

	void updateScene() override;
	void updateCamera() override;
	void drawScene() override;
	void drawTiles();
	void parseTiles(const char* path);
	void parseStairs(vector<vector<int>> mat);

	void setDebugMessage(string message, int count) override {
		if (count == 1) debug_text1 = message;
		if (count == 2) debug_text2 = message;
	}
	float getWorldHeight() override { return worldHeight; }
	float getWorldWidth() override { return worldWidth; }
	int getTileHeight() override { return tileHeight; }
	int getTileWidth() override { return tileWidth; }

	const char* getPath() override { return path; }

	void pushPlayerHitBoxes(damageRect hitBox) override { playerHitBoxes.push_back(hitBox); }
	void removePlayerHitBoxes(Rectangle* hitBox) override;
	void removeDestructables(DestructableObject* d) override;
	void pushSolidRects(Rectangle hitBox) override { solidRects.push_back(hitBox); }
	void removeSolidRects(Rectangle hitBox) override;
	void pushLoot(Loot* l) override { lootitems.push_back(l); }
	void removeLoot(Loot* l) override;
	void pushEnemyRects(damageRect hitBox) override { enemyRects.push_back(hitBox); }
	void removeEnemy(Enemy* e) override;
	void removeEnemyRects(Rectangle* hitbox) override;
	void removeAllEnemies() override;
	void pushProjectile(Projectile* p) override;
	void removeProjectile(Projectile* p) override;
	Player* getPlayer() override { return player; }
	float getTimeLeft() override { return timeLeft->getTime(); }
	void resetTimeLeft() override { timeLeft->startTimer(timeLeft->getLifeTime()); }
	void spawnParticle(Vector2 position) override;
	void spawnHitEffect(Vector2 position) override;

	Vector2 getBossStart() { return bossStart; }
};

// TILE SCREEN

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();
	void updateScene() override;
	void drawScene() override;
private:
	Texture2D background;
};

// LEVEL SCREEN

class LevelScene : public Scene
{
public:
	LevelScene();
	~LevelScene();
	void updateScene() override;
	void drawScene() override;
private:
	Texture2D background;
	Vector2 triangleCenter;
	int level = 1;
	bool selection;
};

// LORE SCREEN

class LoreScene : public Scene
{
public:
	LoreScene();
	~LoreScene();
	void updateScene() override;
	void drawScene() override;
private:
};

// CREDITS SCREEN

class CreditsScene : public Scene
{
public:
	CreditsScene();
	~CreditsScene();
	void updateScene() override;
	void drawScene() override;
private:
	SpriteRenderer* castle = nullptr;
	SpriteRenderer* credits = nullptr;
	Texture2D background;

	string creditAnims[20] = {"cred_0-a", "cred_0-b", "cred_1-a", "cred_1-b", "cred_2-a", "cred_2-b", 
							"cred_3-a", "cred_3-b", "cred_4-a", "cred_4-b", "cred_5-a", "cred_5-b", 
							"cred_6-a", "cred_6-b", "cred_7-a", "cred_7-b", "cred_8-a", "cred_8-b", "cred_9-a", "cred_9-b" };
	int curCred = 0;

	Timer credTimer{ 1 };

};

// CUTSCENE SCREEN

class CutScene : public Scene
{
public:
	CutScene();
	~CutScene();
	void updateScene() override;
	void drawScene() override;
private:
	SpriteRenderer* simon = nullptr;
	SpriteRenderer* cloud1 = nullptr;
	SpriteRenderer* cloud2 = nullptr;
	SpriteRenderer* bat = nullptr;
	Texture2D background;

	

	

};

