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
	sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::BAT_BOSS);
	size.x = sprite->getAnimationFromName("batBossIdle").frameWidth;
	size.y = sprite->getAnimationFromName("batBossIdle").frameHeight;
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	position = pos;
	health = 16;
	damage = 2;
	points = 5000;
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
		printf("%d\n", seekingTimer.getTime());
		seekingTimer.updateTimer();
		sprite->setAnimation("batBossSearching");
		playerPos = GameManager::getInstance().getActiveScene()->getPlayer()->getPos();

		dist = Vector2DistanceSqr(playerPos, position);

		if (seekingTimer.isTriggerd()) {
			if (dist > 8 * 16 * 8 * 16) {
				startPos = position;
				targetPos = { (float)GameManager::getInstance().getActiveScene()->getWorldWidth() - 16 - GetRandomValue(0, 256 - 32), (float)16 + GetRandomValue(0, 224 - 32) };
				swoopTimer = 0.0f;
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
		printf("attack\n");
		position = Vector2MoveTowards(position, playerPos, 120 * deltaTime);

		if (Vector2Equals(position, playerPos)) {
			startPos = position;
			targetPos = { (float)GameManager::getInstance().getActiveScene()->getWorldWidth() - 128, (float) 224/2 };
			swoopTimer = 0.0f;
			state = ROOSTING;
		}

		break;
	case SPITTING:
		break;
	case ROOSTING:
		printf("roost\n");
		swoopTimer += deltaTime;
		t = swoopTimer / swoopDuration;
		position = EvaluateSwoop(startPos, targetPos, t, swoopHeight);
		if (t >= 1.0f) {
			printf("aaaaaa\n");
			t = 1.0f;
			seekingTimer.startTimer();
			state = SEARCHING;
		}
		break;
	default:
		break;
	}
	//if (setup) {

	//	switch (state) {

	//	case BatBossState::IDLE:
	//	{
	//		
	//		

	//		

	//		break;

	//	}

	//	case BatBossState::SEARCHING: {
	//		
	//		
	//		// Volar hacia el jugador
	//		float dx = playerPos.x - position.x;
	//		float dy = playerPos.y - position.y;
	//		float dist = sqrtf(dx * dx + dy * dy);

	//		if (dist > 8.0f) {
	//			// Normalizar y mover
	//			position.x += (dx / dist) * speed * GetFrameTime();
	//			position.y += (dy / dist) * speed * GetFrameTime();
	//		}
	//		else {
	//			// Llegó al jugador — decidir si atacar o escupir
	//			float yDiff = playerPos.y - position.y;
	//			if (yDiff > spitYThreshold) {
	//				state = BatBossState::SPITTING;
	//				spitsFired = 0;
	//				spitTimer = 0;
	//			}
	//			else {
	//				state = BatBossState::ATTACKING;
	//				targetPos = playerPos; // guarda dónde estaba el jugador al atacar
	//			}
	//		}
	//		break;
	//	}

	//	case BatBossState::ATTACKING: {
	//		sprite->setAnimation("batBossAttack");

	//		// Embestida rápida hacia targetPos
	//		float dx = targetPos.x - position.x;
	//		float dy = targetPos.y - position.y;
	//		float dist = sqrtf(dx * dx + dy * dy);

	//		if (dist > 8.0f) {
	//			position.x += (dx / dist) * attackSpeed * GetFrameTime();
	//			position.y += (dy / dist) * attackSpeed * GetFrameTime();
	//		}
	//		else {
	//			// Ataque completado — retroceder a posición alejada del jugador
	//			// La posición de retreat es en dirección contraria al jugador
	//			float rdx = position.x - playerPos.x;
	//			float rdy = position.y - playerPos.y;
	//			float rdist = sqrtf(rdx * rdx + rdy * rdy);
	//			targetPos = {
	//				position.x + (rdx / rdist) * retreatDistance,
	//				position.y + (rdy / rdist) * retreatDistance
	//			};
	//			state = BatBossState::SEARCHING;
	//		}
	//		break;
	//	}

	//	case BatBossState::SPITTING: {
	//		sprite->setAnimation("batBossSpit");

	//		spitTimer += GetFrameTime();
	//		if (spitTimer >= spitInterval) {
	//			spitTimer = 0;
	//			spitsFired++;

	//			// TODO: instanciar proyectil apuntando al jugador
	//			// GameManager::getInstance().getActiveScene()->spawnProjectile(position, playerPos);

	//			if (spitsFired >= spitCount) {
	//				// Ráfaga completada — volver a SEARCHING
	//				state = BatBossState::SEARCHING;
	//				spitsFired = 0;
	//			}
	//		}
	//		break;
	//	}
	//	}
	//}
	
	Enemy::update();
	lateUpdate();
}

BatBoss::~BatBoss() {}

Vector2 BatBoss::EvaluateSwoop(Vector2 start, Vector2 end, float t, float swoopHeight) {
	// Linear interpolation (straight line point)
	Vector2 linearPoint = Vector2Lerp(start, end, t);

	// Direction from start to end
	Vector2 direction = Vector2Normalize(Vector2Subtract(end, start));

	// Perpendicular vector (rotate 90 degrees)
	Vector2 perpendicular = { -direction.y, direction.x };


	// Parabolic offset: peak at t = 0.5, zero at t = 0 and t = 1
	float offset = swoopHeight * (1.0f - std::pow(2.0f * t - 1.0f, 2.0f));

	// Apply the offset
	return {
		linearPoint.x + perpendicular.x * offset,
		linearPoint.y + perpendicular.y * offset
	};
}


