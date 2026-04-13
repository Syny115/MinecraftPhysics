#include "Projectile.h"
#include "GameManager.h"

void Projectile::groundCollision(vector<Rectangle> floorRec) {
	int len = floorRec.size();
	Rectangle predictedRec = hurtbox;
	predictedRec.y += velocity.y * deltaTime;
	int i = checkCollisionRecsArr(predictedRec, floorRec, len);
	if (i != -1) {
		GameManager::getInstance().getActiveScene()->removeProjectile(this);
		queueDeletion();
	}
	else isOnFloor = false;
}

bool Projectile::enemyCollision(vector<damageRect>& dmgRect) {
	if (owner == ENEMY) return false;
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect)) {
			GameManager::getInstance().getActiveScene()->removeProjectile(this);
			queueDeletion();
			return true;
		}
	}
	return false;
}

bool Projectile::playerCollision(Rectangle rec) {
	if (owner == PLAYER) return false;
		if (CheckCollisionRecs(hurtbox, rec)) {
			GameManager::getInstance().getActiveScene()->removeProjectile(this);
			queueDeletion();
			return true;
		}
	return false;
}

void Projectile::earlyUpdate() {
	Entity::earlyUpdate();
	sprite->update(deltaTime);
	sprite->setFlipX(direction > 0);
}

Projectile::Projectile()
{
	sprite = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
	owner = PLAYER;
	direction = 0;
}

Projectile::~Projectile()
{
	delete sprite;
}


// DAGGER

Dagger::Dagger(Vector2 pos, int dir, int own)
{
	sprite->setAnimation("dagger");
	position = pos;
	direction = dir;
	hurtbox.width = sprite->getAnimationFromName("dagger").frameWidth;
	hurtbox.height = sprite->getAnimationFromName("dagger").frameHeight;
	size = Vector2{ hurtbox.width, hurtbox.height };
	owner = own;
}

Dagger::~Dagger()
{
}

void Dagger::update() {
	earlyUpdate();
	moveHLinear(200 * direction);
	lateUpdate();
}