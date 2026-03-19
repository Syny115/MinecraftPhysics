#pragma once
#include "raylib.h"

class SpriteRenderer {
private:
	const char* path;
	Vector2 spriteLocation;
	Vector2 spriteSize;
	int currentFrame;
public:
	void drawSprite();
	
	void setPath(const char* p);
	void setSpriteLocation();

};

