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
	Timer despawntime;
	int lootType;
	
public:
	Loot() {}
	Loot(Vector2 p, int lt);
	~Loot();
	void fall();
	void update() override;
	void Draw() {
		sprite->draw(position);
	}
};

