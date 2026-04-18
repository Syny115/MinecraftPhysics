#include "UI.h"
#include "GameManager.h"
using namespace std;
#define FontSize 36
#define FontSpacing 6

#define LivesRecWidth 11
#define LivesRecHeight 20

#define CameraZoom 3.125

UI::UI() {
	int fileSize = 0;
	unsigned char* fileData = LoadFileData("resources/fonts/gamefont.ttf", &fileSize);
	font = { 0 };
	font.baseSize = 16;
	font.glyphCount = 77;
	font.glyphs = LoadFontData(fileData, fileSize, 16, 0, 77, FONT_DEFAULT);

	

}
UI::~UI() {
	UnloadFont(font);
}

void UI::updateUI() {
	if(isRendering){
		
		string text = "Score-00" + to_string(GameManager::getInstance().getScore());
		DrawTextEx(font, text.c_str(), Vector2{ 0,0 }, FontSize, FontSpacing, WHITE);
		
		// Player lives
		
		

		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 172 + i * (LivesRecWidth + 3), 54, LivesRecWidth, LivesRecHeight};
			if (GameManager::getInstance().getPlayerHealth() > i) {
				DrawRectangleRec(rec, RED);
			}
			else {
				DrawRectangleLinesEx(rec, 3, WHITE);
			}
		}

		// Boss lives

		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 172 + i * (LivesRecWidth + 3), 104, LivesRecWidth, LivesRecHeight };
			if (GameManager::getInstance().getBossHealth() > i) {
				DrawRectangleRec(rec, RED);
			}
			else {
				DrawRectangleLinesEx(rec, 3, WHITE);
			}
		}

		//Time left

		text = "TIME 0" + to_string((int)GameManager::getInstance().getActiveScene()->getTimeLeft());
		DrawTextEx(font, text.c_str(), Vector2{320,0}, FontSize, FontSpacing, WHITE);

		text = "STAGE 0" + to_string((int)GameManager::getInstance().getGamePointer()->getCurrentLevel() + 1);

		DrawTextEx(font, text.c_str(), Vector2{600,0}, FontSize, FontSpacing, WHITE);

		//Ammo and Lives left

		text = "-"; //+GameManager::getInstance().; get the current ammo

		DrawTextEx(font, text.c_str(), Vector2{580,16 * CameraZoom }, FontSize, FontSpacing, WHITE);

		text = "P-03"; // + blahblablah lives		

		DrawTextEx(font, text.c_str(), Vector2{ 552,32 * CameraZoom }, FontSize, FontSpacing, WHITE);
	}
	

}

void UI::renderUI() {

	isRendering = true;

	DrawRectangle(0, 0, 800, 150, BLACK);
	DrawRectangleLinesEx(Rectangle{ 400, 50, 100, 80 }, 5, RED);

	DrawTextEx(font, "PLAYER", Vector2{ 0,16 * CameraZoom }, FontSize, FontSpacing, WHITE);
	DrawTextEx(font, "ENEMY", Vector2{ 0,32 * CameraZoom }, FontSize, FontSpacing, WHITE);

	//Add the bigheart sprite here

}