#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>

using namespace std;

class Entity {
protected:
	bool isLoaded = false;
	bool isOnFloor;
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	float grav = 1250;
	float maxFALL = 400;
	float worldHeight, worldWidth;
	int health;


	float floorHeight = 256;

	float offsetX, offsetY;

	Rectangle hurtbox;
	


public:
	Entity() {}
	Entity(Vector2 s, Vector2 p, Vector2 v) { size = s; position = p; velocity = v; }
	~Entity() {} // No delete this;
	Vector2 getSize() {
		return size;
	}
	Vector2 getPos() {
		return position;
	}
	Vector2 getVelocity() {
		return velocity;
	}
	virtual int checkCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len);
	virtual void groundCollision(vector<Rectangle> floorRec);
	void load() {isLoaded = true;}
	void unload() {isLoaded = false;}
	void queueDeletion();
	void increaseHalfOfGravity();
	virtual void earlyUpdate();
	virtual void lateUpdate();
	virtual void update();
	virtual void moveV(); // moved to entity.cpp
	void moveHLinear(const int speed);
	void moveVLinear(const int speed);
	virtual void updateColliderPosiotions();
	Rectangle getHurtbox() { return hurtbox; };
};