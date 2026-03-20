#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Player.h";
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include "libraries/json.hpp"
using json = nlohmann::json;
using namespace std;

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

	float worldWidth;
	float worldHeight;
	int tileHeight;
	int tileWidth;

	vector<vector<int>> tileMat;
	Texture2D tileAtlas = LoadTexture("resources/sprites/CastlevaniaTileset.png");



	//vector<Projectile> projectiles;
public:
	vector<Rectangle> solidRects;
	PlayableScene(const char* path);
	~PlayableScene();
	//void startScene();

	void updateScene() override;
	void updateCamera() override;
	void drawScene() override;
	void drawTiles();
	void parseTiles(const char* path);
};