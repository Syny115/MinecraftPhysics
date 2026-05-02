#include "SceneManager.h"
#include "GameManager.h"

SceneManager::SceneManager() {
	static room entrance{ "resources/json/Level1_Scene1.json" };
	allRooms.push_back(&entrance);
	static room hall1{ "resources/json/Level1_Scene2.json" };
	allRooms.push_back(&hall1);
	static room hall1p5{ "resources/json/Level1_Scene2-5.json" };
	allRooms.push_back(&hall1p5);
	static room hall2{ "resources/json/Level1_Scene3.json" };
	allRooms.push_back(&hall2);
	static room hall3{ "resources/json/Level1_Scene4.json" };
	allRooms.push_back(&hall3);

	entrance.exit1 = &hall1;
	entrance.area = 0;
	hall1.exit0 = &entrance;
	hall1.exit1 = &hall2;
	hall1.exit2 = &hall1p5;
	hall1.area = 0;;
	hall1p5.exit0 = &hall1;
	hall1p5.area = 0;
	hall2.exit0 = &hall1;
	hall2.exit1 = &hall3;
	hall2.area = 0;
	hall3.exit0 = &hall2;
	hall3.area = 0;


	//----------------------------------------------------------------------------------//
	//	null <- | entrance	| <- | hall1	| <- | hall2	| <- | hall3	|			//
	//			|			| -> |			| -> |			| -> | hall3	| -> null	//
	//								V	^												//
	//							 | hall1p5  |											//
	//----------------------------------------------------------------------------------//

	head = &entrance;
	areas.push_back(head);
	saveRoom = head;
}

SceneManager::~SceneManager()
{
}

void SceneManager::requestSceneLoad(SceneType type) {
	switch (type) {
		case SceneType::PLAYABLE: if (currentRoom != nullptr) GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, GameManager::getInstance().getLastExit())); break;
		case SceneType::TITLE: GameManager::getInstance().getGamePointer()->setPendingScene(new TitleScene()); break;
		case SceneType::LORE: GameManager::getInstance().getGamePointer()->setPendingScene(new LoreScene()); break;
		case SceneType::LEVEL_SELECT: GameManager::getInstance().getGamePointer()->setPendingScene(new LevelScene()); break;
	default: GameManager::getInstance().getGamePointer()->setPendingScene(new TitleScene()); break;
	}
	
}

void SceneManager::requestRoomExit(int exit) {
	room* r = getExit(exit);
	int spawn;
		if (r != nullptr) {
			if (currentRoom == r->exit0) spawn = 0;
			else if (currentRoom == r->exit1) spawn = 1;
			else if (currentRoom == r->exit2) spawn = 2;
			else if (currentRoom == r->exit3) spawn = 3;
			currentRoom = r;
			GameManager::getInstance().setLastExit(spawn);
			GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, spawn));
		}
}

void SceneManager::requestFirstRoomInArea(int area) {
	if (areas[area] != nullptr) {
		currentRoom = areas[area];
		GameManager::getInstance().setArea(area);
		GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, 0));
	}
}

void SceneManager::requestSpecificRoom(int r) {
	if (allRooms[r] != nullptr) {
		currentRoom = allRooms[r];
		GameManager::getInstance().setArea(currentRoom->area);
		GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, 0));
	}
}

void SceneManager::requestSaveRoom() {
		currentRoom = saveRoom;
		GameManager::getInstance().setArea(currentRoom->area);
		GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, 4));
	
}

room* SceneManager::getExit(int exit) {
	if (exit == 1) return currentRoom->exit1;
	if (exit == 2) return currentRoom->exit2;
	if (exit == 3) return currentRoom->exit3;
	return currentRoom->exit0;
}

int SceneManager::getAllRoomsSize() { return allRooms.size(); }

void SceneManager::gameSaved() { 
	saveRoom = currentRoom;
}