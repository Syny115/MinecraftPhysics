#include "Savepoint.h"

SavePoint::SavePoint(Vector2 pos)
{
	position = pos;
	sprite = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::SAVE);
	size.x = 32;
	size.y = 32;
}

SavePoint::~SavePoint()
{
	delete sprite;
}

void SavePoint::update() {
	Entity::earlyUpdate();
	sprite->update(deltaTime);
	Entity::lateUpdate();
}