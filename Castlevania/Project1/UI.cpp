#include "UI.h"

using namespace std;

void UI::updateUI() {
	char* point;
	//Substitute point with GameManager::etc to get the score points
	DrawTextPro(font, point, Vector2{ 100,0 }, Vector2{ 0,0 },0,20,5,WHITE);
	//GameManager::getInstance().getActiveScene().getScore();
}

UI::~UI(){
	UnloadFont(font);

}

