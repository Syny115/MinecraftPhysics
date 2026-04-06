#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
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

	int checkSelfCollisionPointRecArr(Rectangle* recs, int len); //moved to entity.cpp

	void load() {isLoaded = true;}
	void unload() {isLoaded = false;}
	void queueDeletion();
	void increaseHalfOfGravity();
	virtual void earlyUpdate();
	virtual void lateUpdate();
	virtual void update();
	virtual void moveV(); // moved to entity.cpp
};