#pragma once
#include "raylib.h"
#include <string>

class UI {
private:
	Font font;
	bool isRendering = false;
public:
	UI();
	~UI();
	void updateUI();
	void renderUI();
};