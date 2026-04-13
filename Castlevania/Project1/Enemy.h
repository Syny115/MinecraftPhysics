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
	int direction;
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
	const float speedH = 80.0f;
	const float speedV = 40.0;
	const float sinFrequency = 4.0f;
};

// MEDUSA

class Medusa : public Enemy
{
public:
	void update() override;
	Medusa(Vector2 pos);
	~Medusa();
};

class Panther : public Enemy
{
	public:
	void update() override;
	Panther(Vector2 pos);
	~Panther();
};

