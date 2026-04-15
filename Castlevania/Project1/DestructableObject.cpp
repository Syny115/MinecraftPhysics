#include "DestructableObject.h"
#include "GameManager.h"
DestructableObject::DestructableObject()
{
	size.x = 16;//GameManager::getInstance().getActiveScene()->getTileWidth();
	size.y = 16;//GameManager::getInstance().getActiveScene()->getTileHeight();
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	sprite = new SpriteRenderer("resources/sprites/CastlevaniaTileset.png", SpriteRenderer::BREAKABLES);
}

DestructableObject::DestructableObject(Vector2 p)
{
	position = p;
	size.x = 16;//GameManager::getInstance().getActiveScene()->getTileWidth();
	size.y = 16;//GameManager::getInstance().getActiveScene()->getTileHeight();
	hurtbox.width = size.x;
	hurtbox.height = size.y;
	hurtbox.x = p.x;
	hurtbox.y = p.y;
	
}

DestructableLoot::DestructableLoot(Vector2 p, bool c, int l) {
	
	sprite = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::BREAKABLES);
	if (c) sprite->setAnimation("candle");
	else {
		sprite->setAnimation("torch");
		size.x = sprite->getAnimationFromName("torch").frameWidth;
		size.y = sprite->getAnimationFromName("torch").frameHeight;
		hurtbox.width = size.x;
		hurtbox.height = size.y;
	}
	loot = l;

	position = { p.x+ size.x /2, p.y + size.y /2};
	hurtbox.x = p.x;
	hurtbox.y = p.y;
}

DestructableWall::DestructableWall(Vector2 p, int l) {
	sprite = new SpriteRenderer("resources/sprites/CastlevaniaTileset.png", SpriteRenderer::BREAKABLES);
	sprite->setAnimation("ground");
	loot = l;
	position = { p.x + size.x / 2, p.y + size.y / 2 };
	hurtbox.x = p.x;
	hurtbox.y = p.y;
}

void DestructableWall::update() {
	DestructableObject::update();
	if (!setup) GameManager::getInstance().getActiveScene()->pushSolidRects(hurtbox);
	setup = true;
}

DestructableObject::~DestructableObject() {
	delete sprite;
}

DestructableLoot::~DestructableLoot() {
	if (loot == 1) { // axe
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 6));
	}
	else if (loot == 2) { // dagger
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 5));
	}
	else if (loot == 3) { // Holy water
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 7));
	}
	else if (loot == 4) { // stopwatch
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 9));
	}
	else if (loot == 5) { // boomerang
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 8));
	}
	else if (loot == 6) { // jar
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 13));
	}
	else if (loot == 7) { // cross
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 14));
	}
	else { // random
		int rand = GetRandomValue(1, 100);
		if (GameManager::getInstance().getWhipLevel() == 0 && rand <= 75 || GameManager::getInstance().getWhipLevel() == 1 && rand <= 50) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 0));
		else {
			rand = GetRandomValue(1, 100);
			if (rand <= 40) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 1)); //small heart
			else if (rand <= 60) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 2)); //big heart
			else if (rand <= 90) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 3)); //Money bag
			else if (rand <= 96) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 14)); //Cross
			else if (rand <= 99) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 13)); //jar
			else if (rand <= 100) GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 15)); //1-up
		}
	}
}

DestructableWall::~DestructableWall() {
	GameManager::getInstance().getActiveScene()->removeSolidRects(hurtbox);
	if (loot == 1) {
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 12));
	}
	else if (loot == 2 && GameManager::getInstance().getProjectileUpgrade() < 2) {
		GameManager::getInstance().getActiveScene()->pushLoot(new Loot(position, 10 + GameManager::getInstance().getProjectileUpgrade()));
	}
}

void DestructableObject::update() {
	earlyUpdate();
	sprite->update(deltaTime);
	lateUpdate();
}

void DestructableObject::hitCollision(vector<damageRect>& dmgRect) {
	float s = GameManager::getInstance().getWhipLevel() < 2 ? 32 : 48;
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect) && dmgRect[i].rect->width == s) {
			GameManager::getInstance().getActiveScene()->removeDestructables(this);
			queueDeletion();
		}
	}
}

void DestructableObject::draw() {
	//DrawRectangleV(position, size, GREEN);
	//DrawRectangleRec(hurtbox, GREEN);
	sprite->draw({offsetX, offsetY});
}