#include "UI.h"
#include "GameManager.h"
using namespace std;
#define FontSize 3
#define FontSpacing 6

#define LivesRecWidth 3
#define LivesRecHeight 6

#define CameraZoom cz

UI::UI() {
	int fileSize = 0;
	unsigned char* fileData = LoadFileData("resources/fonts/gamefont.ttf", &fileSize);
	font = { 0 };
	font.baseSize = 16;
	font.glyphCount = 77;
	font.glyphs = LoadFontData(fileData, fileSize, 16, 0, 250, FONT_DEFAULT);
	
	
	sw = new SpriteRenderer("resources/sprites/misc_sprites.png", SpriteRenderer::LOOT);
	fontTexture = LoadTexture("resources/sprites/fonts_sprites.png");

	int h = 0;
	for (int i = 0; i+'a' <= 'z'; i++) {
		if (i % (fontTexture.width / 8) == 0 && i != 0) h++;
		fontBody[i + 'a'] = { i * 8.0f, h * 8.0f, 8.0f, 8.0f };
		fontTitle[i + 'a'] = { i * 8.0f, h * 16.0f + 28, 8.0f, 12.0f };
	}
	for (int i = 0; i <= 9; i++) {
		fontBody[i + '0'] = { i * 8.0f, 16, 8, 8 };
	}
	fontBody['.'] = { 80, 8, 8, 8 };
	fontTitle['.'] = { 80, 44, 8, 12 };
	fontBody[','] = { 88, 8, 8, 8 };
	fontTitle[','] = { 88, 44, 8, 12 };
	fontBody['!'] = { 96, 8, 8, 8 };
	fontTitle['!'] = { 96, 44, 8, 12 };
	fontBody['?'] = { 104, 8, 8, 8 };
	fontTitle['?'] = { 104, 44, 8, 12 };
	fontBody['+'] = { 112, 8, 8, 8 };
	fontBody['*'] = { 120, 8, 8, 8 };
	fontBody[':'] = { 80, 16, 8, 8 };
	fontBody[';'] = { 88, 16, 8, 8 };
	fontBody['('] = { 96, 16, 8, 8 };
	fontBody[')'] = { 104, 16, 8, 8 };
	fontBody['-'] = { 112, 16, 8, 8 };
	fontTitle['-'] = { 112, 44, 8, 12 };
	fontBody['/'] = { 120, 16, 8, 8 };
	fontTitle['/'] = { 120, 44, 8, 12 };


	
}
UI::~UI() {
	UnloadFont(font);
	delete sw;
	UnloadTexture(fontTexture);
}

void UI::initUI() {
	cz = GameManager::getInstance().getActiveScene()->getCamera().zoom;
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

		drawTextFromTexture(text, 0, { 1 * cz, 7 * cz }, Vector2Zero(), 1, -1, WHITE);

		// Player lives

		buffer = GameManager::getInstance().getPlayerHealth();

		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 56*cz + i * (LivesRecWidth + 1)*cz, 18 * cz, LivesRecWidth*cz, LivesRecHeight*cz };
			if (buffer > i) {
				DrawRectangleRec(rec, RED);
			}
			else {
				DrawRectangleLinesEx(rec, 1*cz, WHITE);
			}
		}

		// Boss lives
		buffer = GameManager::getInstance().getBossHealth();
		for (int i = 0; i < 16; i++) {
			Rectangle rec = { 56*cz + i * (LivesRecWidth + 1) * cz, 26* cz, LivesRecWidth* cz, LivesRecHeight * cz };
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

		drawTextFromTexture(text, 0, { 106 * cz, 7 * cz }, Vector2Zero(), 1, -1, WHITE);

		text = "AREA 0" + to_string((int)GameManager::getInstance().getArea());

		drawTextFromTexture(text, 0, { 185 * cz, 7 * cz }, Vector2Zero(), 1, -1, WHITE);


		//Ammo and Lives left
		buffer = GameManager::getInstance().getAmmo();
		if (buffer < 10) text = " -0" + to_string(buffer);
		else text = " -" + to_string(buffer);

		drawTextFromTexture("*", 0, { 168 * cz, 16 * cz }, Vector2Zero(), 1, -1, RED);
		drawTextFromTexture(text, 0, { 168 * cz, 16 * cz }, Vector2Zero(), 1, -1, WHITE);

		text = "/-03"; //When lives are added do the get

		drawTextFromTexture(text, 0, { 168 * cz, 24 * cz }, Vector2Zero(), 1, -1, WHITE);

		buffer = GameManager::getInstance().getSubWeapon();
		if(buffer != 0){
			if (buffer == 1) sw->setAnimation("dagger");
			else if (buffer == 2) sw->setAnimation("axe");
			else if (buffer == 2) sw->setAnimation("holywater");
			else if (buffer == 2) sw->setAnimation("cross");
			else if (buffer == 2) sw->setAnimation("stopwatch");
			sw->draw({ 134*cz, 18*cz }, 4.125);
		}
	}
	

}

void UI::renderUI() {
	isRendering = true;
	DrawRectangle(0, 0, 256*cz, 48*cz, BLACK);
	DrawRectangleLinesEx(Rectangle{ 128 * cz, 16 * cz, 32 * cz, 22 * cz }, 1*cz, RED);
	
	drawTextFromTexture("PLAYER", 0, { 1 * cz, 15 * cz }, Vector2Zero(), 1, 50 * cz, WHITE);
	drawTextFromTexture("Enemy", 0, { 1 * cz, 23 * cz }, Vector2Zero(), 1, 50 * cz, WHITE);

	//Add the bigheart sprite here
	

}


void UI::drawTextFromTexture(string message, int font, Vector2 position, Vector2 offset, int scale, int maxWidth, Color c) {
	scale *= cz;
	unordered_map<char, Rectangle>* f;
	if (font == 0) {
		f = &fontBody;
	}
	else {
		f = &fontTitle;
	}
	Rectangle sourceRect;
	Rectangle destRect = (*f)['a'];
	float w = destRect.width, h = destRect.height;

	int x = 0, y = 0;
	for (int i = 0; i < message.size(); i++) {
		sourceRect = (*f)[tolower(message.c_str()[i])];
		if (sourceRect.height == 0 && sourceRect.width == 0) sourceRect = fontBody[tolower(message.c_str()[i])];

		destRect = { position.x + x, position.y + y, sourceRect.width * scale, sourceRect.height * scale };
		DrawTexturePro(fontTexture, sourceRect, destRect, { 0, 0 }, 0, c);
		x += w * scale + offset.x * cz;
		if (x > maxWidth && maxWidth > 0) {
			y += h * scale + offset.y * cz;
			x = 0;
		}
	}

}
