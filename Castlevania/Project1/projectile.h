#pragma once
#include "Entity.h"
#include "SpriteRenderer.h"
#include "Classes.h"


class Projectile : public Entity
{
public:
	enum OWNERS
	{
		PLAYER, ENEMY
	};

	Projectile();
	virtual ~Projectile();

	virtual bool boolGroundCollision(vector<Rectangle> floorRec);
	virtual bool enemyCollision(vector<damageRect>& dmgRect);
	virtual bool playerCollision(Rectangle rec);

	SpriteRenderer* sprite = nullptr;

	void draw() {
		//DrawRectangleRec(hurtbox, WHITE);
		sprite->draw({ offsetX, offsetY });
		
	}

	void earlyUpdate() override;

	short getDamage() { return damage; }
	

	int getOwner() { return owner; }

protected:
	int direction;
	int owner;
	short damage = 1;
};

//DAGGER

class Dagger : public Projectile {
public:
	Dagger(Vector2 pos, int dir, int own);
	~Dagger();

	void update() override;
private:
};

//AXE

class Axe : public Projectile {
public:
	Axe(Vector2 pos, int dir, int own);
	~Axe();

	void update() override;
	bool enemyCollision(vector<damageRect>& dmgRect) override { return false; }
private:
};

//HORIZONTAL FIREBALL

class Hfire : public Projectile {
public:
	Hfire(Vector2 pos, int dir, int own);
	~Hfire();

	void update() override;
private:
};

//SEEKING FIREBALL

class Sfire : public Projectile {
public:
	Sfire(Vector2 pos, int dir, int own);
	~Sfire();

	void update() override;
private:
	float sine, cosine;
};