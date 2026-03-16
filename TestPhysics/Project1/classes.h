#pragma once
#ifndef CLASSES_H
#define CLASSES_H
#include "libraries/raylib/include/raylib.h"
#include "raymath.h"

class SpriteRenderer {
private:
	Texture2D tex;

public:
	SpriteRenderer(Texture2D t) { tex = t; }
	Texture2D getTexture() { return tex; }

};

class Entity {
protected:
	int health;
	Vector2 position;
	Rectangle Collider;
	Vector2 velocity;
	// No more variables mabye
public:

	int getHealth() { return health; }

};

class Player : public Entity {

};

class Enemy : public Entity {

};

struct Item {

};

class Loot : public Entity {
private:
	Item lootitem;
public:
	Loot(Item i) { lootitem = i; } // Constructor that assigns the item to the loot entity

};








#endif//CLASSES_H