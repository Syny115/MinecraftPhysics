#include "Loot.h"
#include "GameManager.h"

static string lootnames[] = { "whip", "smallheart", "bigheart", "moneybag", "orb", "dagger", "axe", "holywater", "boomerang", "stopwatch", "twoupgrade", 
"threeupgrade", "chicken", "jar", "invincibilitycross", "1up", "crown", "chest"};

Loot::Loot(Vector2 p, int lt) {
	// Loot should only be constructed with position and loottype
	lootType = lt;
	sprite = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
	sprite->setAnimation(lootnames[lt]);
	hurtbox.width = sprite->getAnimationFromName(lootnames[lt]).frameWidth;
	hurtbox.height = sprite->getAnimationFromName(lootnames[lt]).frameHeight;
	size = Vector2{hurtbox.width * 2, hurtbox.height * 2}; // Has to be double the sprite's width/height or else collisions won't be accurate
	position = p;
	velocity = Vector2{ 0,20 }; //v.x = 0, v.y = 20
	despawntime.startTimer();
	grav = 0;
}
Loot::~Loot() {
	//delete sprite;
}

bool Loot::playerCollision(Rectangle rec) {
	if (CheckCollisionRecs(rec, hurtbox)) {
		// Add pickup code here

		GameManager::getInstance().getActiveScene()->removeLoot(this);
		queueDeletion();
	}
	return CheckCollisionRecs(rec, hurtbox);
}

void Loot::update () {	
	despawntime.updateTimer();
	GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(despawntime.getTime()), 1);
	
	
	Loot::earlyUpdate();
	if (!isOnFloor) { moveV(); }

	sprite->update(GetFrameTime());

	Loot::lateUpdate();
	if (despawntime.isTriggerd()) {
		GameManager::getInstance().getActiveScene()->removeLoot(this);
		queueDeletion();
	}
}
