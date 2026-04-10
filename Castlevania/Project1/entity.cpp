#include "GameManager.h"
#include "Entity.h"

int Entity::checkCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len) {
	for (int i = 0; i < len; i++) {
		if (CheckCollisionRecs(rec1, recs2[i])) return i;
	}
	return -1;
}

void Entity::groundCollision(vector<Rectangle> floorRec) {
	int len = floorRec.size();
	Rectangle predictedRec = hurtbox;
	predictedRec.y += velocity.y * GetFrameTime();
	int i = checkCollisionRecsArr(predictedRec, floorRec, len);
	if (i != -1) {
		isOnFloor = true;
		position.y = floorRec[i].y - size.y / 2;
		floorHeight = position.y;
		if (velocity.y > 0) velocity.y = 0;
	}
	else isOnFloor = false;
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

void Entity::earlyUpdate() {}

void Entity::lateUpdate() { updateColliderPosiotions(); }

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
