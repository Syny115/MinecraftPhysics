#pragma once
#include "Entity.h"
#include "SpriteRenderer.h"

class SavePoint : public Entity
{
public:
	SavePoint(Vector2 pos);
	~SavePoint();
	void draw() {
		//DrawCircleV(position, 2, BLUE);
		sprite->draw({ offsetX, offsetY });
	}
	void update() override;
private:
	SpriteRenderer* sprite;
};


