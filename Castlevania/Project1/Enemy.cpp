#include "Enemy.h"
#include "GameManager.h"

Enemy::Enemy(Vector2 pos)
{
	size.x = 16;
	size.y = 32;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
}

Enemy::~Enemy()
{
	int rand = GetRandomValue(1, 100);
	if (rand <= 40) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 1)); //small heart
	else if (rand <= 60) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 2)); //big heart
	else if (rand <= 90) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 3)); //Money bag
	else if (rand <= 96) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 14)); //Cross
	else if (rand <= 99) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 13)); //jar
	else if (rand <= 100) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 15)); //1-up

	delete sprite;
}

void Enemy::update() {
	sprite->update(deltaTime);
	if (!setup) GameManager::getInstance().getActiveScene()->pushEnemyRects(damageRect{ &hurtbox, damage });
	setup = true;
	if (health <= 0) {
		GameManager::getInstance().getActiveScene()->removeEnemy(this);
		queueDeletion();
	}

}

void Enemy::hitCollision(vector<damageRect>& dmgRect) {
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect)) {
			health -= dmgRect[i].damage;
		}
	}
}

// ZOMBIE
Zombie::Zombie(Vector2 pos)
{
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::ZOMBIE);
	size.x = sprite->getAnimationFromName("walk").frameWidth;
	size.y = sprite->getAnimationFromName("walk").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
}

void Zombie::update() {
	earlyUpdate();
	// ADD BEHAVIOUR HERE!




	// ADD BEHAVIOUR HERE!
	Enemy::update();
	lateUpdate();
}

Zombie::~Zombie() {}

