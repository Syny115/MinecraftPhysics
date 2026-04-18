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
		sprite->draw({offsetX, offsetY});
	}

protected:
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
private:
	enum class BatBossState { IDLE, FLY };
	BatBossState state = BatBossState::IDLE;
	const float detectionRange = 150.0f;
	const float speed = 60.0f;
};



