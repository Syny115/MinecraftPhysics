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

DestructableLoot::DestructableLoot(Vector2 p, bool candle) {

}

DestructableWall::DestructableWall(Vector2 p, bool c) {
	position = p;
	hurtbox.x = p.x;
	hurtbox.y = p.y;
	sprite = new SpriteRenderer("resources/sprites/CastlevaniaTileset.png", SpriteRenderer::BREAKABLES);
	sprite->setAnimation("ground");
	chicken = c;
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
	//delete sprite;
}

DestructableWall::~DestructableWall() {
	GameManager::getInstance().getActiveScene()->removeSolidRects(hurtbox);
	if (chicken) {
		//Create new loot CHICKEN!
	}
}

void DestructableObject::update() {
	sprite->update(GetFrameTime());
}

void DestructableObject::hitCollision(vector<damageRect>& dmgRect) {
	for (int i = 0; i < dmgRect.size(); i++) {
		if (CheckCollisionRecs(hurtbox, *dmgRect[i].rect)) {
			GameManager::getInstance().getActiveScene()->removeDestructables(this);
			GameManager::getInstance().getActiveScene()->deleteMe(this);
		}
	}
}

void DestructableObject::draw() {
	DrawRectangleV(position, size, GREEN);
	sprite->draw(position);
}