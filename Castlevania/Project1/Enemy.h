#pragma once
#include "Entity.h"
#include "SpriteRenderer.h"
#include "Classes.h"

using namespace std;
class Enemy : public Entity
{
public:
	Enemy() {}
	Enemy(Vector2 p);
	virtual ~Enemy();
	void hitCollision(vector<damageRect>& dmgRect);
	virtual void update() override;
	void draw() {
		if (!hitCooldown.isActive() || (int)(GetTime() / GetFrameTime()) % 8 > 0 || health == 0)
		{
			sprite->draw({ offsetX, offsetY });
		}
	}

protected:
	Timer hitCooldown{0.5};
	short damage;
	bool setup = false;
	SpriteRenderer* sprite = nullptr;
	int points;
	int direction = 1;
	bool offCamera;
};


// ZOMBIE
class Zombie : public Enemy
{
public:
	void update() override;
	Zombie(Vector2 pos);
	~Zombie();
};

// BAT
class Bat : public Enemy {
public:
	Bat(Vector2 pos);
	~Bat();
	void update() override;
	void groundCollision(vector<Rectangle> floorRec) override{}

private:
	enum class BatState { IDLE, CHASE };
	BatState state = BatState::IDLE;

	const float detectionRange = 100.0f;
	const float sinFrequency = 4.0f;
	Vector2 velocity = { 80.0f, 40.0f };
};

// MEDUSA
class Medusa : public Enemy
{
public:
	void update() override;
	Medusa(Vector2 pos);
	~Medusa();
	void groundCollision(vector<Rectangle> floorRec) override {}
private:
	const float sinFrequency = 2.0f;
	Vector2 velocity = { 50.0f, 40.0f };
};

// PANTHER
class Panther : public Enemy
{
public:
	void update() override;
	Panther(Vector2 pos);
	~Panther();
private:
	enum class PantherState { IDLE, JUMP, WALK };
	PantherState state = PantherState::IDLE;
	const float detectionRange = 60.0f;
	const float speed = 80.0f;
	bool wasOnFloor = true;

};

// MERMAN /work in progress
class Merman : public Enemy
{
public:
	void update() override;
	Merman(Vector2 pos);
	~Merman();
private:
	enum class MermanState { SHOT, WALK };
	MermanState state = MermanState::SHOT;
	const float speed = 40.0f;
};

// BAT BOSS

class BatBoss : public Enemy
{
public:
	void update() override;
	BatBoss(Vector2 pos);
	~BatBoss();
	void groundCollision(vector<Rectangle> floorRec) override {}

	Vector2 EvaluateSwoop(Vector2 start, Vector2 end, float t, float swoopHeight);
	void prepareRoost();
	
private:
	Timer seekingTimer{ 1.5 };
	enum BatBossState { IDLE, SEARCHING, ATTACKING, SPITTING, ROOSTING };
	BatBossState state = BatBossState::IDLE;

	// Spitting
	float spitTimer = 0;
	const float spitInterval = 1.5f;   // cada cuánto escupe
	const int spitCount = 3;           // cuántos proyectiles por ráfaga
	int spitsFired = 0;

	Vector2 startPos;
	Vector2 targetPos = { 0, 0 };
	Vector2 playerPos;

	int* globalHealth;
	float swoopTimer;
	float swoopDuration = 2;
	float swoopHeight = 50;
	float t;
	float dist;
};


