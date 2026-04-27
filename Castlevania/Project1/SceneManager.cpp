#include "SceneManager.h"
#include "GameManager.h"

SceneManager::SceneManager() {
	static room entrance{ "resources/json/Level1_Scene1.json" };
	static room hall1{ "resources/json/Level1_Scene2.json" };
	static room hall2{ "resources/json/Level1_Scene3.json" };
	static room hall3{ "resources/json/Level1_Scene4.json" };

	entrance.exit1 = &hall1;
	hall1.exit0 = &entrance;
	hall1.exit1 = &hall2;
	hall2.exit0 = &hall1;
	hall2.exit1 = &hall3;
	hall3.exit0 = &hall2;


	//----------------------------------------------------------------------------------//
	//	null <- | entrance	| <- | hall1	| <- | hall2	| <- | hall3	|			//
	//			|			| -> |			| -> |			| -> | hall3	| -> null	//
	//----------------------------------------------------------------------------------//

	head = &entrance;
	areas.push_back(head);
}

SceneManager::~SceneManager()
{
}

void SceneManager::requestSceneLoad(SceneType type) {
	switch (type) {
		case SceneType::PLAYABLE: if (currentRoom != nullptr) GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path)); break;
		case SceneType::TITLE: GameManager::getInstance().getGamePointer()->setPendingScene(new TitleScene()); break;
		case SceneType::LORE: GameManager::getInstance().getGamePointer()->setPendingScene(new LoreScene()); break;
	default: GameManager::getInstance().getGamePointer()->setPendingScene(new TitleScene()); break;
	}
	
}

void SceneManager::requestRoomExit(int exit) {
	room* r = getExit(exit);
		if (r != nullptr) {
			currentRoom = r;
			GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path));
		}
}

void SceneManager::requestFirstRoomInArea(int area) {
	if (areas[area] != nullptr) {
		currentRoom = areas[area];
		GameManager::getInstance().setArea(area);
		GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path));
	}
}

room* SceneManager::getExit(int exit) {
	if (exit == 1) return currentRoom->exit1;
	if (exit == 2) return currentRoom->exit2;
	if (exit == 3) return currentRoom->exit3;
	return currentRoom->exit0;
}