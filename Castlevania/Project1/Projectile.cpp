#include "Projectile.h"
#include "GameManager.h"

bool Projectile::boolGroundCollision(vector<Rectangle> floorRec) {
	int len = floorRec.size();
	Rectangle predictedRec = hurtbox;
	predictedRec.y += velocity.y * deltaTime;
	int i = checkCollisionRecsArr(predictedRec, floorRec, len);
	if (i != -1) {
		queueDeletion();
		return true;
	}
	else isOnFloor = false;
	return false;
}

bool Projectile::enemyCollision(vector<damageRect>& dmgRect) {
	if (owner == ENEMY) return false;
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect)) {

			queueDeletion();
			return true;
		}
	}
	return false;
}

bool Projectile::playerCollision(Rectangle rec) {
	if (owner == PLAYER) return false;
		if (CheckCollisionRecs(hurtbox, rec)) {
			
			queueDeletion();
			return true;
		}
	return false;
}

void Projectile::earlyUpdate() {
	Entity::earlyUpdate();
	sprite->update(deltaTime);
	sprite->setFlipX(direction < 0);
	Vector2 temp = GetScreenToWorld2D(position, GameManager::getInstance().getActiveScene()->getCamera());
	Rectangle screen = { temp.x - 128, temp.y, 256 * 2, 224 * 2 };
	if ( !CheckCollisionPointRec(position, screen))
	{
		queueDeletion();
	}
}

Projectile::Projectile()
{
	sprite = new SpriteRenderer("resources/sprites/simon_sprites.png", SpriteRenderer::PROJECTILES);
	owner = PLAYER;
	direction = 0;
}

Projectile::~Projectile()
{
	GameManager::getInstance().getActiveScene()->removeProjectile(this);
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

//AXE

Axe::Axe(Vector2 pos, int dir, int own)
{
	sprite->setAnimation("axe");
	position = pos;
	direction = dir;
	hurtbox.width = sprite->getAnimationFromName("axe").frameWidth;
	hurtbox.height = sprite->getAnimationFromName("axe").frameHeight;
	size = Vector2{ hurtbox.width, hurtbox.height };
	owner = own;
	grav = 600;
	velocity.y = -350;
}

Axe::~Axe()
{
}

void Axe::update() {
	earlyUpdate();
	moveHLinear(100 * direction);
	moveV();
	lateUpdate();
}

//HORIZONTAL FIREBALL

Hfire::Hfire(Vector2 pos, int dir, int own)
{
	sprite->setAnimation("fireball");
	position = pos;
	direction = dir;
	hurtbox.width = sprite->getAnimationFromName("fireball").frameWidth;
	hurtbox.height = sprite->getAnimationFromName("fireball").frameHeight;
	size = Vector2{ hurtbox.width, hurtbox.height };
	owner = own;
}

Hfire::~Hfire()
{
}

void Hfire::update() {
	earlyUpdate();
	moveHLinear(100 * direction);
	lateUpdate();
}

//SEEKING FIREBALL

Sfire::Sfire(Vector2 pos, int dir, int own)
{
	sprite->setAnimation("fireball");
	position = pos;
	direction = dir;
	hurtbox.width = sprite->getAnimationFromName("fireball").frameWidth;
	hurtbox.height = sprite->getAnimationFromName("fireball").frameHeight;
	size = Vector2{ hurtbox.width, hurtbox.height };
	owner = own;
	//sine = (GameManager::getInstance().getActiveScene().getPlayer()->getPos().y - pos.y) / Vector2Sub(GameManager::getInstance().getActiveScene().getPlayer()->getPos(), pos);
	//cosine = (GameManager::getInstance().getActiveScene().getPlayer()->getPos().x - pos.x) / Vector2Sub(GameManager::getInstance().getActiveScene().getPlayer()->getPos(), pos);
}

Sfire::~Sfire()
{
}

void Sfire::update() {
	earlyUpdate();
	moveHLinear(100 * direction);
	lateUpdate();
}