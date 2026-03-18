#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

void earlyUpdate() {
	// Things here are done right BEFORE the update code
}
void lateUpdate() {
	// Things here are done right AFTER the update code
}


class GameManager {
private:
	// Scene scene; (Need Scene class)
	//Game* gamePointer; (Need game class)
	const float screenWidth = 800;
	const float screenHeight = 700;

	const float viewportWidth = 256;
	const float viewportHeight = 224;

	const int worldWidth = 256;
	const int worldHeight = 240;
	// Hey joao how the fuck does world size work i add this here just in case - Dr. D

public:
	/* Scene getActiveScene() {
	return game.Scene[currentLevel];
}*/
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
		return getViewportHeight();
	}

};

class Entity{
protected:
	bool isLoaded = false;
	bool isOnFloor;
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	float grav;
	float maxFALL = 400;
public:
	Entity() {}
	Entity(Vector2 s, Vector2 p, Vector2 v) { size = s; position = p; velocity = v; }
	~Entity() {
		delete this; // Must be created by new. No new[], no stack, just "new Entity"
	} // Deconstructor code here
	Vector2 getSize() {
		return size;
	}
	Vector2 getPos() {
		return position;
	}
	Vector2 getVelocity() {
		return velocity;
	}
	int checkSelfCollisionPointRecArr(Rectangle* recs, int len) {
		for (int i = 0; i < len; i++) {
			if (CheckCollisionPointRec(position, recs[i])) return i;
		}
		return -1;
	}

	void moveV() {
		//If Space is down, gravity is halved (Variable Jump height)
		grav = IsKeyDown(KEY_SPACE) ? (grav*0.5f) : (grav * 0.5f) * 2;

		//Apply half of gravity
		if (!isOnFloor && velocity.y < maxFALL) {
			velocity.y += grav * GetFrameTime() * 0.5f;
		}

		//Apply Vertical Movement
		position.y += velocity.y * GetFrameTime();

		//Apply other half of gravity
		if (!isOnFloor && velocity.y < maxFALL) {
			velocity.y += grav * GetFrameTime() * 0.5f;
		}

		//Clamp position to World Size TODO: Add world size to Game Manager
		position.y = Clamp(position.y, 0, 240);
	}
	void load() {
		isLoaded = true;
	}
	void unload() {
		isLoaded = false;
	}
	void queueDeletion(){ //Doesn't work just yet
		// Wait for Game Manager to be added for this:
		//getActiveScene().deleteQueue.push(this);
	}
};

class Enemy : public Entity{
protected:
	bool wasOnFloor;
	bool isOnCeiling;
	
	bool leftBlocked;
	bool rightBlocked;


};

