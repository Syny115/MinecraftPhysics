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
	printf("AAA\nAAAAAAAAAAAAAAAAA\nAAAAAAAAAAAAAAAAA\nAAAAA\n\n\n\nAA\nAA");
	if (offCamera == false && !isBoss)
	{
		GameManager::getInstance().getGamePointer()->publicPlaySound(Game::BREAK);
		int rand = GetRandomValue(1, 256);
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
	Vector2 temp = GetWorldToScreen2D(position, GameManager::getInstance().getActiveScene()->getCamera());
	Rectangle screen = { -GetScreenWidth() / 2, -GetScreenHeight() / 2, GetScreenWidth() * 2, GetScreenHeight() * 2 };
	if (!CheckCollisionPointRec(temp, screen))
	{
		//TODO implement unloading for monsters that do not infinately despawm
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
	hitCooldown.updateTimer();
	if (hitCooldown.isActive()) return;
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect)) {
			health -= dmgRect[i].damage;
			hitCooldown.startTimer();
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
	moveV();
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

//BAT BOSS

BatBoss::BatBoss(Vector2 pos) {
	isBoss = true;
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::BAT_BOSS);
	size.x = sprite->getAnimationFromName("batBossIdle").frameWidth;
	size.y = sprite->getAnimationFromName("batBossIdle").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 16;
	damage = 2;
	points = 5000;
	globalHealth = GameManager::getInstance().getBossHealthPointer();
}

void BatBoss::update() {
	earlyUpdate();
	switch (state)
	{
	case IDLE:
		sprite->setAnimation("batBossIdle");

		if (GameManager::getInstance().getBossStarted()) {
			seekingTimer.startTimer();
			state = BatBossState::SEARCHING;
		}
		break;
	case SEARCHING:
		seekingTimer.updateTimer();
		sprite->setAnimation("batBossSearching");
		playerPos = GameManager::getInstance().getActiveScene()->getPlayer()->getPos();

		dist = Vector2DistanceSqr(playerPos, position);

		if (seekingTimer.isTriggerd()) {
			if (dist < 8 * 16 * 8 * 16) {
				prepareRoost();
				state = ROOSTING;
			}
			else {
				if (playerPos.y > position.y) {
					state = ATTACKING;
				}
				else state = SPITTING;
			}
		}
		break;
	case ATTACKING:
		position = Vector2MoveTowards(position, playerPos, 120 * deltaTime);

		if (Vector2Equals(position, playerPos)) {
			prepareRoost();
			state = ROOSTING;
		}

		break;
	case SPITTING:
		GameManager::getInstance().getActiveScene()->pushProjectile(new Sfire({position.x + size.x/2, position.y + size.y / 2 }, 1, Projectile::ENEMY));
		seekingTimer.startTimer();
		state = BatBossState::SEARCHING;
		break;
	case ROOSTING:
		swoopTimer += deltaTime;
		t = swoopTimer / swoopDuration;
		position = EvaluateSwoop(startPos, targetPos, t, swoopHeight);
		if (t >= 1.0f) {
			t = 1.0f;
			seekingTimer.startTimer();
			state = SEARCHING;
		}
		break;
	default:
		break;
	}

	*globalHealth = health;
	Enemy::update();
	lateUpdate();
}

BatBoss::~BatBoss() {
	GameManager::getInstance().getActiveScene()->pushLoot(new Loot({ GameManager::getInstance().getActiveScene()->getWorldWidth() - (128), 96 }, 4)); //ORB
}

Vector2 BatBoss::EvaluateSwoop(Vector2 start, Vector2 end, float t, float swoopHeight) {
	// Linear interpolation (straight line point)
	Vector2 linearPoint = Vector2Lerp(start, end, t);

	// Direction from start to end
	Vector2 direction = Vector2Normalize(Vector2Subtract(end, start));

	// Perpendicular vector (rotate 90 degrees)
	Vector2 perpendicular = { -direction.y, direction.x };


	// Parabolic offset: peak at t = 0.5, zero at t = 0 and t = 1
	float offset = swoopHeight * (1.0f - std::pow(2.0f * t - 1.0f, 2.0f));

	float w = GameManager::getInstance().getActiveScene()->getWorldWidth();

	// Apply the offset
	return Vector2Clamp( {
		linearPoint.x + perpendicular.x * offset,
		linearPoint.y + perpendicular.y * offset
		}, { w - (256), 31 }, {w, 224});
}

void BatBoss::prepareRoost() {
	startPos = position;
	targetPos.y = 96 + (float)GetRandomValue(0, 80);
	float middle = GameManager::getInstance().getActiveScene()->getWorldWidth()-(128);
	if (playerPos.x > middle) targetPos.x = middle - (float)GetRandomValue(0, 128-16);
	else targetPos.x = middle + (float)GetRandomValue(0, 128 - 16);
	swoopTimer = 0.0f;
}
