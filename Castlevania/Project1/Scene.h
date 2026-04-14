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

using json = nlohmann::json;
using namespace std;

class Scene {
protected:
	Camera2D camera = { 0 };
	queue<Entity*> deletionQueue;
public:

	Scene();
	//virtual ~Scene();
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

	virtual void pushPlayerHitBoxes(damageRect hitBox) { /* I | Ii | II | I- */ }
	virtual void removePlayerHitBoxes(Rectangle* hitBox) { /*I AM AT A LOSS*/ }
	virtual void pushLoot(Loot* l) {}
	virtual void removeLoot(Loot* l) {}
	virtual void removeDestructables(DestructableObject* d) {}
	virtual void pushSolidRects(Rectangle rect) {}
	virtual void removeSolidRects(Rectangle rect) {}
	virtual void pushEnemyRects(damageRect hitBox) {}
	virtual void removeEnemy(Enemy* e) {}

	virtual void pushProjectile(Projectile* p) {}
	virtual void removeProjectile(Projectile* p) {}
	virtual Player* getPlayer() { return nullptr; }
};





class PlayableScene : public Scene {
private:
	SpriteRenderer* spriteAnimation = nullptr;
	Player* player;
	vector<Rectangle> solidRects;
	vector<damageRect> enemyRects;
	vector<Rectangle> lootRects;
	vector<Rectangle> destructableRects;
	vector<damageRect> playerHitBoxes;
	vector<Loot*> lootitems;

	vector<Vector2> checkpoints;
	vector<staircase> stairs;
	vector<DestructableObject*> destructables;
	vector<Enemy*> enemies;

	vector<Vector2> zombieSpawners;
	vector<Vector2> medusaSpawners;
	vector<Vector2> batSpawners;
	Vector2 bossSpawner;

	vector<Projectile*> projectiles;

	Timer spawnCoolDown{ 3 };

	float worldWidth;
	float worldHeight;
	int tileHeight;
	int tileWidth;

	vector<vector<int>> tileMat;
	Texture2D tileAtlas = LoadTexture("resources/sprites/CastlevaniaTileset.png");

	//vector<Entity*> entities;

	string debug_text1;
	string debug_text2;
public:

	PlayableScene(const char* path);
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

	void pushPlayerHitBoxes(damageRect hitBox) override { playerHitBoxes.push_back(hitBox); 
	TraceLog(LOG_INFO, "ADD: rect=%p  INDEX: %d", hitBox.rect, playerHitBoxes.size()-1);
	}
	void removePlayerHitBoxes(Rectangle* hitBox) override;
	void removeDestructables(DestructableObject* d) override;
	void pushSolidRects(Rectangle hitBox) override { solidRects.push_back(hitBox); }
	void removeSolidRects(Rectangle hitBox) override;
	void pushLoot(Loot* l) override { lootitems.push_back(l); }
	void removeLoot(Loot* l) override;
	void pushEnemyRects(damageRect hitBox) override { enemyRects.push_back(hitBox); }
	void removeEnemy(Enemy* e) override;
	virtual void pushProjectile(Projectile* p) override;
	virtual void removeProjectile(Projectile* p) override;
	Player* getPlayer() override { return player; }

};