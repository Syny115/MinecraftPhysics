#pragma once
#include "GameManager.h"
#include "raylib.h"
#include "raymath.h"
class UI : public GameManager {
private:
	Font font;
public:
	UI();
	~UI();
	virtual void updateUI();

};