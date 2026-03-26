#pragma once
#include "Entity.h"
using namespace std;
// All the methods are in "Projectile.cpp" in request of the john guy. - Dr.D


class Projectile : public Entity {
private:
	
public:
	Projectile();
	Projectile(Vector2 s, Vector2 p, Vector2 v /*add other variables here*/) : Entity() {
		Entity(s, p, v);
		//Add other variables needed here
	}
	~Projectile();
	void constantMove(Vector2 velocity);
};