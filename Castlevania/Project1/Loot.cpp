#include "Loot.h"
#include "GameManager.h"

static string lootnames[] = { "whip", "smallheart", "bigheart", "moneybag", "orb", "dagger", "axe", "holywater", "boomerang", "stopwatch", "twoupgrade", 
"threeupgrade", "chicken", "jar", "rosary", "1up", "crown", "chest"};

Loot::Loot(Vector2 p, int lt) {
	// Loot should only be constructed with position and loottype
	lootType = lt;
	sprite = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
	sprite->setAnimation(lootnames[lt]);
	hurtbox.width = sprite->getAnimationFromName(lootnames[lt]).frameWidth;
	hurtbox.height = sprite->getAnimationFromName(lootnames[lt]).frameHeight;
	size = Vector2{hurtbox.width, hurtbox.height};
	position = p;
	velocity = Vector2{ 10,20 }; //v.x = 0, v.y = 20
	despawntime.startTimer();
	if (lt != 1) {grav = 200; velocity.x = 0; }
	else grav = 0;
}
Loot::~Loot() {
	//delete sprite;
}

bool Loot::playerCollision(Rectangle rec) {
	if (CheckCollisionRecs(rec, hurtbox)) {
		// Add pickup code here
		if (lootType == 5 || lootType == 6 || lootType == 7 || lootType == 8 || lootType == 9) {
			GameManager::getInstance().changeSubWeapon(lootType);
		}
		else if (lootType == 0) GameManager::getInstance().addWhipLevel(1);
		else if (lootType == 1) GameManager::getInstance().addAmmo(1);
		else if (lootType == 2) GameManager::getInstance().addAmmo(5);
		else if (lootType == 3) GameManager::getInstance().addScore(1000);
		else if (lootType == 10) GameManager::getInstance().setProjectileCount(1);
		else if (lootType == 11) GameManager::getInstance().setProjectileCount(2);
		else if (lootType == 12) {
			if (GameManager::getInstance().getHealth() > 7) {
				GameManager::getInstance().maximizeHealth();
			}
			else {
				GameManager::getInstance().addHealth(4);
			}
		}
		else if (lootType == 13) {
			//This is teh invisibility jar.
			// Should create a Timer that while active makes the player invulnerable
			// And also makes him glow with that epileptic sprite change animation
		}
		else if (lootType == 14) {
			// Should queue deletion on every enemy on screen 
		}
		else if (lootType == 15) {
			// Gives... one life? I can't find this one up item anywhere

		}
		else if (lootType >= 16 && lootType <= 17) GameManager::getInstance().addScore(2000);
		else if (lootType == 4){ 
			GameManager::getInstance().maximizeHealth();
			// Deletes all enemies from scene, so copy loottype == 14 code (Or call it)
			// Play cutscene
			// Add time left to score
			// End level
	}

		GameManager::getInstance().getActiveScene()->removeLoot(this);
		queueDeletion();
	}
	return CheckCollisionRecs(rec, hurtbox);
}

void Loot::update () {	
	earlyUpdate();
	despawntime.updateTimer(deltaTime);
	
	
	
	if (!isOnFloor) { 
		moveV();
		if (this->lootType == 1) {
			moveH(); // Needs help but not rn
		}
	}

	sprite->update(deltaTime);

	lateUpdate();
	if (despawntime.isTriggerd()) {
		GameManager::getInstance().getActiveScene()->removeLoot(this);
		queueDeletion();
	}
}
