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
	GameManager::getInstance().getActiveScene()->removeEnemy(this);
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

	if (GetWorldToScreen2D(position, GameManager::getInstance().getActiveScene()->getCamera()).x < -128 || GetWorldToScreen2D(position, GameManager::getInstance().getActiveScene()->getCamera()).x > GetScreenWidth() + 128)
	{
		health = 0;
		offCamera = true;
	}

	setup = true;

	sprite->setFlipX(direction > 0);

	if (health <= 0) {
		
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

// ZOMBIE //por hacer: que cambie de dirección al llegar a un muro
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

	Enemy::update();
	lateUpdate();
}

Zombie::~Zombie() {}

// BAT
Bat::Bat(Vector2 pos)
{
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::BAT);
	size.x = sprite->getAnimationFromName("batIdle").frameWidth;
	size.y = sprite->getAnimationFromName("batIdle").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
	points = 200;

}

void Bat::update() {
	earlyUpdate();

	if (setup) { // setup se vuelve true después del primer Enemy::update()
		if (state == BatState::IDLE) {
			Vector2 playerPos = GameManager::getInstance().getActiveScene()->getPlayer()->getPos();
			float distX = playerPos.x - position.x;
			float distance = sqrtf(distX * distX + (playerPos.y - position.y) * (playerPos.y - position.y));

			if (distance <= detectionRange) {
				state = BatState::CHASE;
				sprite->setAnimation("batFly");
			}
		}
		else if (state == BatState::CHASE) {
			moveHLinear(velocity.x * direction);
			moveVLinear(velocity.y * sin(GetTime() * sinFrequency));
		}
	}

	Enemy::update();
	lateUpdate();
}

Bat::~Bat() {}

// PANTHER

Panther::Panther(Vector2 pos)
{
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::PANTHER);
	size.x = sprite->getAnimationFromName("pantherIdle").frameWidth;
	size.y = sprite->getAnimationFromName("pantherIdle").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
	points = 300;
}

void Panther::update() {
	earlyUpdate();

	if (setup) {
		if (state == PantherState::IDLE) {
			sprite->setAnimation("pantherIdle"); // fuerza la animación correcta

			Vector2 playerPos = GameManager::getInstance().getActiveScene()->getPlayer()->getPos();
			float distX = playerPos.x - position.x;
			float distance = sqrtf(distX * distX + (playerPos.y - position.y) * (playerPos.y - position.y));

			if (distance <= detectionRange) {
				state = PantherState::JUMP;
				velocity.y = -400.0f;
				wasOnFloor = false;
			}
		}
		else if (state == PantherState::JUMP) {
			moveHLinear(speed * direction);
			sprite->setAnimation("pantherJump");

			if (isOnFloor && !wasOnFloor) {
				state = PantherState::WALK;
			}
			wasOnFloor = isOnFloor; // solo se actualiza en JUMP
		}
		else if (state == PantherState::WALK) {
			moveHLinear(speed * direction);
			sprite->setAnimation("pantherWalk");
		}
	}

	moveV();
	Enemy::update();
	lateUpdate();
}

Panther::~Panther() {}

//MEDUSA //por hacer: retocar parametros 

Medusa::Medusa(Vector2 pos)
{
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::MEDUSA);
	size.x = sprite->getAnimationFromName("medusaFly").frameWidth;
	size.y = sprite->getAnimationFromName("medusaFly").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
	points = 400;
}

void Medusa::update() {
	earlyUpdate();

	if (setup) {
		moveHLinear(velocity.x * direction);
		moveVLinear(velocity.y * sin(GetTime() * sinFrequency));
	}

	Enemy::update();
	lateUpdate();
}

Medusa::~Medusa() {}

//MERMAN 

Merman::Merman(Vector2 pos)
{
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::MERMAN);
	size.x = sprite->getAnimationFromName("mermanShot").frameWidth;
	size.y = sprite->getAnimationFromName("mermanShot").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 1;
	damage = 1;
	points = 400;
}

void Merman::update() {
	earlyUpdate();

	if (setup) {
		moveHLinear(velocity.x * direction);
	}

	Enemy::update();
	lateUpdate();
}

Merman::~Merman() {}


