#pragma once
#include "raylib.h"
#include <string>

class UI {
private:
	Font font;
public:
	UI();
	~UI();
	void updateUI();
};