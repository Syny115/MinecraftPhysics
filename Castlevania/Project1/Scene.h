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
using json = nlohmann::json;
using namespace std;

class Scene {
protected:
	Camera2D camera = { 0 };
	queue<Entity*> deletionQueue;
public:
	Scene();
	virtual void start();
	virtual void updateScene();
	virtual void updateCamera();
	virtual void drawScene();

	void deleteMe(Entity* e) {
		deletionQueue.push(e);
	}


	//So that the game doesnt go wonky when trying to access playable scene funcions
	virtual void setDebugMessage(string message, int count) { /* I | Ii | II | I- */ }
	virtual float getWorldHeight() { return 0; }
	virtual float getWorldWidth() { return 0; }
	virtual int getTileHeight() { return 0; }
	virtual int getTileWidth() { return 0; }
};





class PlayableScene : public Scene {
private:
	Player* player;
	
	vector<Rectangle> enemyRects;
	vector<Rectangle> lootRects;
	vector<Rectangle> destructableRects;
	vector<Rectangle> playerHitBoxes;

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
	vector<Rectangle> solidRects;
	PlayableScene(const char* path);
	~PlayableScene();
	
	void start() override;

	void updateScene() override;
	void updateCamera() override;
	void drawScene() override;
	void drawTiles();
	void parseTiles(const char* path);

	void setDebugMessage(string message, int count) override {
		if (count == 1) debug_text1 = message;
		if (count == 2) debug_text2 = message;
	}
	float getWorldHeight() override { return worldHeight; }
	float getWorldWidth() override { return worldWidth; }
	int getTileHeight() override { return tileHeight; }
	int getTileWidth() override { return tileWidth; }
};