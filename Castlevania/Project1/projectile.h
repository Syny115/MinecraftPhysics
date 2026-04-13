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

	void groundCollision(vector<Rectangle> floorRec);
	bool enemyCollision(vector<damageRect>& dmgRect);
	bool playerCollision(Rectangle rec);

	SpriteRenderer* sprite = nullptr;

	void draw() {
		//DrawRectangleRec(hurtbox, WHITE);
		sprite->draw({ offsetX, offsetY });
		
	}

	void earlyUpdate() override;

	short getDamage() { return damage; }

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