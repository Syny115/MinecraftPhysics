#include "GameManager.h"
#include "Entity.h"
using namespace std;



int Entity::checkSelfCollisionPointRecArr(Rectangle* recs, int len) {
	for (int i = 0; i < len; i++) {
		if (CheckCollisionPointRec(position, recs[i])) return i;
	}
	return -1;
}

void Entity::increaseHalfOfGravity() {
	if (!isOnFloor && velocity.y < maxFALL && GetTime()/GetFrameTime() > 15) {
		velocity.y += grav * GetFrameTime() * 0.5f;
	}
}

void Entity::moveV() {
	//If Space is down, gravity is halved (Variable Jump height)
	//grav = IsKeyDown(KEY_SPACE) ? (grav * 0.5f) : (grav * 0.5f) * 2;

	//Apply half of gravity
	increaseHalfOfGravity();

	//Apply Vertical Movement
	position.y += velocity.y * GetFrameTime();

	//Apply other half of gravity
	increaseHalfOfGravity();

	//Clamp position to World Size
	position.y = Clamp(position.y, 0, GameManager::getInstance().getActiveScene()->getWorldHeight());
}

void Entity::queueDeletion() { //Doesn't work just yet
	// Wait for Game Manager to be added for this:
	GameManager::getInstance().getActiveScene()->deleteMe(this);
}

void Entity::earlyUpdate() {
	// Things here are done right BEFORE the update code
}

void Entity::lateUpdate() {
	// Things here are done right AFTER the update code
}

void Entity::update() {
	// Things here are done right AFTER the update code
}

void Entity::moveHLinear(const int speed) {
	position.x += speed * GetFrameTime();
	offsetX = position.x - size.x / 2;
	updateColliderPosiotions();
}

void Entity::moveVLinear(const int speed) {
	position.y += speed * GetFrameTime();
	offsetY = position.y - size.y / 2;
	updateColliderPosiotions();
}

void Entity::updateColliderPosiotions() {
	hurtbox.x = position.x;
	hurtbox.y = position.y;
}