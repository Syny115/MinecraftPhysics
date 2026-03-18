#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Player.h";
#include <iostream>
#include <queue>


class Scene {
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
	Scene();
	//void startScene();

	void updateScene();
	void updateCamera();
	void readTilemap();
	void drawScene();
	void drawTiles(const char* path);
};