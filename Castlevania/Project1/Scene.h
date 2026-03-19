#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Player.h";
#include <iostream>
#include <queue>
#include <vector>


class Scene {
private:
	Camera2D camera = { 0 };
	//queue<Entity> deletionQueue;
public:
	Scene();
	virtual void updateScene();
	virtual void updateCamera();
	virtual void drawScene();
};





class PlayableScene : public Scene {
private:
	Camera2D camera = { 0 };
		

	Player player;
	
	vector<Rectangle> enemyRects;
	vector<Rectangle> LootRects;
	vector<Rectangle> DestructableRects;

	//queue<Entity> deletionQueue;

	float worldWidth = 256*2;
	float worldHeight = 224;



	//vector<Projectile> projectiles;
public:
	vector<Rectangle> solidRects;
	PlayableScene();
	//void startScene();

	void updateScene() override;
	void updateCamera() override;
	void readTilemap();
	void drawScene() override;
	void drawTiles(const char* path);
};