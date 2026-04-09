#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Classes.h"
#include "Entity.h"

class Loot : public Entity {
private:
	Texture texture;
	Timer despawntime;
	int lootType;
public:
	Loot() {}
	Loot(Vector2 s, Vector2 p, Vector2 v, Texture tex, Timer t);
	~Loot();
	void fall();
	void update() override;
	void Draw() {
		DrawTexture(texture, position.x - size.x / 2, position.y - size.y / 2, WHITE);
	}
	void Collision() {

	}
	/*
	* Loot Collision (comes from entity), send pointer to hurtbox (entity) to array of rectangles (Loot rectangles) in scene
	* Inside player, make function: LootTouchedBelmont (Send loot to deconstructor and change loottype variable to act in Gamemanager)
	* Add gravity 
	* Add function TouchedGround (Same as player but adapted for just one hurtbox)
	*/
};


