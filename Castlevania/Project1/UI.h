#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include "SpriteRenderer.h"
class UI {
private:
	Font font;
	bool isRendering = false;
	float cz;
	SpriteRenderer* sw = nullptr;

	unordered_map<char, Rectangle> fontBody;
	unordered_map<char, Rectangle> fontTitle;
	Texture2D fontTexture;

public:
	UI();
	~UI();
	void initUI();
	void updateUI();
	void renderUI();
	void drawTextFromTexture(string message, int font, Vector2 position, Vector2 offset, int scale, int maxWidth, Color c);
};