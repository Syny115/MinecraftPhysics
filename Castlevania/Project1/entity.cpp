#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "entity.h"
using namespace std;



int Entity::checkSelfCollisionPointRecArr(Rectangle* recs, int len) {
	for (int i = 0; i < len; i++) {
		if (CheckCollisionPointRec(position, recs[i])) return i;
	}
	return -1;
}
void Entity::moveV() {
	//If Space is down, gravity is halved (Variable Jump height)
	grav = IsKeyDown(KEY_SPACE) ? (grav * 0.5f) : (grav * 0.5f) * 2;

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
void Entity::queueDeletion() { //Doesn't work just yet
	// Wait for Game Manager to be added for this:
	//getActiveScene().deleteQueue.push(this);
}
void Entity::earlyUpdate() {
	// Things here are done right BEFORE the update code
}
void Entity::lateUpdate() {
	// Things here are done right AFTER the update code
}