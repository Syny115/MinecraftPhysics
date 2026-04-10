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
	DestructableWall(Vector2 p, bool c);
	~DestructableWall();
	void update() override;
private:
	bool chicken;
	bool setup;
};

class DestructableLoot : public DestructableObject
{
public:
	DestructableLoot(Vector2 p, bool c);
	~DestructableLoot();
private:
	bool candle;
};