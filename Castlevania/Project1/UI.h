#pragma once
#include "raylib.h"
#include <string>
#include "SpriteRenderer.h"
class UI {
private:
	Font font;
	bool isRendering = false;
	float cz;
	SpriteRenderer* sr = nullptr;
	SpriteRenderer* sw = nullptr;
public:
	UI();
	~UI();
	void updateUI();
	void renderUI();
};