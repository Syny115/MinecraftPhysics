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
	points = 1;
	offCamera = false;
}

Enemy::~Enemy()
{
	if (offCamera == false)
	{
		int rand = GetRandomValue(1, 100);
		if (rand <= 40) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 1)); //small heart
		else if (rand <= 60) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 2)); //big heart
		else if (rand <= 90) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 3)); //Money bag
		else if (rand <= 96) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 14)); //Cross
		else if (rand <= 99) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 13)); //jar
		else if (rand <= 100) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 15)); //1-up
		//GameManager::getInstance().addScore(points); //points DESCOMENTAR CUANDO HAGAMOS MERGE CON ADRI 
	}
	else
	{

	}

	delete sprite;
}

void Enemy::update() {
	sprite->update(deltaTime);
	if (!setup) {
		GameManager::getInstance().getActiveScene()->pushEnemyRects(damageRect{ &hurtbox, damage });
		if (GameManager::getInstance().getActiveScene()->getPlayer()->getPos().x - position.x > 0)
		{
			direction = 1;
		}
		else
		{
			direction = -1;
		}
	}
	setup = true;

	sprite->setFlipX(direction);

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
	points = 100;
	
}

void Zombie::update() {
	earlyUpdate();

	moveHLinear(60 * direction);

	if (GetWorldToScreen2D(position, GameManager::getInstance().getActiveScene()->getCamera()).x < -128 || GetWorldToScreen2D(position, GameManager::getInstance().getActiveScene()->getCamera()).x > GetScreenWidth() + 128)
	{
		health = 0; 
		offCamera = true;
	}

	Enemy::update();
	lateUpdate();
}

Zombie::~Zombie() {}

