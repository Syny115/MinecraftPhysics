#include "UI.h"
#include "GameManager.h"
using namespace std;

UI::UI() {
	int fileSize = 0;
	unsigned char* fileData = LoadFileData("resources/fonts/gamefont.ttf", &fileSize);
	font = { 0 };
	font.baseSize = 16;
	font.glyphCount = 95;
	font.glyphs = LoadFontData(fileData, fileSize, 16, 0, 95, FONT_DEFAULT);
}
UI::~UI() {
	UnloadFont(font);
}

void UI::updateUI() {
	string text = "Score: " + to_string(GameManager::getInstance().getScore());
	DrawTextEx(font, text.c_str(), Vector2{ 300,0 }, 50, 10, RED);
}