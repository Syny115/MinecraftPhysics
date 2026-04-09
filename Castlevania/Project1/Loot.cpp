#include "Loot.h"
#include "GameManager.h"

Loot::Loot(Vector2 s, Vector2 p, Vector2 v, Texture tex, Timer t) {
	texture = tex;
	despawntime = t;
	hurtbox.width = tex.width;
	hurtbox.height = tex.height;
	size = s;
	position = p;
	velocity = v;
	despawntime.startTimer();
	grav = 0;
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
	Loot::earlyUpdate();
	if (!isOnFloor) { moveV(); }
	Loot::lateUpdate();
}
