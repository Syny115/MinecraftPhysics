#include "UI.h"
#include "GameManager.h"
using namespace std;
#define FontSize 36
#define FontSpacing 6

#define LivesRecWidth 11
#define LivesRecHeight 20

#define CameraZoom cz

UI::UI() {
	int fileSize = 0;
	cz = GameManager::getInstance().getActiveScene()->getCamera().zoom;
	unsigned char* fileData = LoadFileData("resources/fonts/gamefont.ttf", &fileSize);
	font = { 0 };
	font.baseSize = 16;
	font.glyphCount = 77;
	font.glyphs = LoadFontData(fileData, fileSize, 16, 0, 250, FONT_DEFAULT);
	
	
	sr = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
	sw = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
}
UI::~UI() {
	UnloadFont(font);
	delete sr;
	delete sw;
}

void UI::updateUI() {
	if (isRendering) {
		int buffer = GameManager::getInstance().getScore();
		string text;
		if (buffer < 10) {
			text = "Score-00000" + to_string(buffer);
		}
		else if (buffer < 100) {
			text = "Score-0000" + to_string(buffer);
		}
		else if (buffer < 1000) {
			text = "Score-000" + to_string(buffer);
		}
		else if (buffer < 10000) {
			text = "Score-00" + to_string(buffer);
		}
		else if (buffer < 100000) {
			text = "Score-0" + to_string(buffer);
		}
		else {
			text = "Score-" + to_string(buffer);
		}

		DrawTextEx(font, text.c_str(), Vector2{ 0,0 }, FontSize, FontSpacing, WHITE);

		// Player lives

		buffer = GameManager::getInstance().getPlayerHealth();

		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 172 + i * (LivesRecWidth + 3), 54, LivesRecWidth, LivesRecHeight };
			if (buffer > i) {
				DrawRectangleRec(rec, RED);
			}
			else {
				DrawRectangleLinesEx(rec, 3, WHITE);
			}
		}

		// Boss lives
		buffer = GameManager::getInstance().getBossHealth();
		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 172 + i * (LivesRecWidth + 3), 104, LivesRecWidth, LivesRecHeight };
			if (buffer > i) {
				DrawRectangleRec(rec, CLITERAL(Color){ 255, 152, 116, 255 });
			}
			else {
				DrawRectangleLinesEx(rec, 3, WHITE);
			}
		}

		//Time left
		buffer = (int)GameManager::getInstance().getActiveScene()->getTimeLeft();
		if (buffer < 10) {
			text = "TIME 000" + to_string(buffer);
		}
		else if ((int)GameManager::getInstance().getActiveScene()->getTimeLeft() < 100) {
			text = "TIME 00" + to_string(buffer);
		}
		else {
			text = "TIME 0" + to_string(buffer);
		}

		DrawTextEx(font, text.c_str(), Vector2{ 320,0 }, FontSize, FontSpacing, WHITE);

		text = "STAGE 0" + to_string((int)GameManager::getInstance().getGamePointer()->getLevelIndex());

		DrawTextEx(font, text.c_str(), Vector2{ 600,0 }, FontSize, FontSpacing, WHITE);

		//Ammo and Lives left
		buffer = GameManager::getInstance().getAmmo();
		if (buffer < 10) text = "-0" + to_string(buffer);
		else text = "-" + to_string(buffer);

		DrawTextEx(font, text.c_str(), Vector2{ 580,16 * CameraZoom }, FontSize, FontSpacing, WHITE);

		text = "P-03"; //When lives are added do the get

		DrawTextEx(font, text.c_str(), Vector2{ 552,32 * CameraZoom }, FontSize, FontSpacing, WHITE);
		buffer = GameManager::getInstance().getSubWeapon();
		if(buffer != 0){
			if (buffer == 1) sw->setAnimation("dagger");
			else if (buffer == 2) sw->setAnimation("axe");
			else if (buffer == 2) sw->setAnimation("holywater");
			else if (buffer == 2) sw->setAnimation("cross");
			else if (buffer == 2) sw->setAnimation("stopwatch");
			sw->draw({ 420, 60 }, 4.125);
		}
	}
	

}

void UI::renderUI() {
	isRendering = true;
	DrawRectangle(0, 0, 800, 150, BLACK);
	DrawRectangleLinesEx(Rectangle{ 400, 50, 100, 80 }, 5, RED);

	DrawTextEx(font, "PLAYER", Vector2{ 0,16 * CameraZoom }, FontSize, FontSpacing, WHITE);
	DrawTextEx(font, "ENEMY", Vector2{ 0,32 * CameraZoom }, FontSize, FontSpacing, WHITE);

	//Add the bigheart sprite here
	
	sr->setAnimation("bigheart");
	sr->draw({ 530,35 }, CameraZoom);
}