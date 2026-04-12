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
		sprite->draw(position);
	}

protected:
	short damage;
	bool setup = false;
	SpriteRenderer* sprite = nullptr;
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

class Bat : public Enemy
{
public:
	void update() override;
	Bat(Vector2 pos);
	~Bat();
};

// MEDUSA

class Medusa : public Enemy
{
public:
	void update() override;
	Medusa(Vector2 pos);
	~Medusa();
};

