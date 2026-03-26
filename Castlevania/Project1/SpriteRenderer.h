#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
using namespace std;

class SpriteRenderer
{
private: 
	Texture2D sprite;
	struct animation {
		string name;
		int totalFrames;
		float frameWidth;
		float frameHeigth;
		Vector2 framePos;
		float hOffset;
		float vOffset;
	};

	vector<animation> animationLibrary;
	

public:

	void playAnimation(string animationName, int currentFrame, Vector2 position);

	enum type
	{
		PLAYER, ZOMBIE, BAT, BOSS, LOOT
	};
	SpriteRenderer() {}
	SpriteRenderer(const char* path, int spriteType);

	~SpriteRenderer();

};

