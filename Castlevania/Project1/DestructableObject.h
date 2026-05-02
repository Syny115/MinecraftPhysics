#pragma once
#include "Entity.h"
#include "SpriteRenderer.h"
#include "Classes.h"
class DestructableObject :  public Entity
{
public:
	DestructableObject();
	DestructableObject(Vector2 p);
	virtual ~DestructableObject();
	void hitCollision(vector<damageRect>& dmgRect);
	void draw();
	void update() override;

protected:
	SpriteRenderer* sprite = nullptr;
};

class DestructableWall : public DestructableObject
{
public:
	DestructableWall(Vector2 p, int l);
	~DestructableWall();
	void update() override;
private:
	int loot;
	bool setup;
};

class DestructableLoot : public DestructableObject
{
public:
	DestructableLoot(Vector2 p, int t, int l);
	~DestructableLoot();
private:
	int loot;
	bool candle;
};