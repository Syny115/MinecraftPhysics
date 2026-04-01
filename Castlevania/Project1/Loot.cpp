#include "Loot.h"
#include "GameManager.h"

Loot::Loot(Vector2 s, Vector2 p, Vector2 v, Texture tex, Timer t) {
	texture = tex;
	despawntime = t;
	rect.width = tex.width;
	rect.height = tex.height;
	size = s;
	position = p;
	velocity = v;
	despawntime.startTimer();
}
Loot::~Loot() {

}
void Loot::update () {	
	despawntime.updateTimer();
	if (despawntime.isTriggerd()) {
		//add to the fuckign despawn queue
		GameManager::getInstance().getActiveScene()->removeLoot(this);
		GameManager::getInstance().getActiveScene()->deleteMe(this);
		
	}
	Entity::update();
	Entity::moveV(); // "falling"
	// add other stuff here for the update
}
