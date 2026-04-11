#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Classes.h"
#include "Entity.h"
#include "SpriteRenderer.h"
class Loot : public Entity {
private:
	SpriteRenderer* sprite = nullptr;
	Timer despawntime{10.0f}; //add time here instead
	int lootType;
	
public:
	Loot() {}
	Loot(Vector2 p, int lt);
	~Loot();
	//void fall(); USE moveV instead! override it if you need to!
	bool playerCollision(Rectangle rec);
	void update() override;
	void Draw() {
		sprite->draw(position);
		//DrawRectangleRec(hurtbox, WHITE);
	}
};

