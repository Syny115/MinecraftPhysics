#include "SceneManager.h"
#include "GameManager.h"

SceneManager::SceneManager() {
	static room entrance{ "resources/json/entrance.json" };
	static room hall1{ "resources/json/hall1.json" };
	static room hall2{ "resources/json/hall2.json" };
	static room cavern1{ "resources/json/cavern1.json" };
	static room cavern2{ "resources/json/cavern2.json" };
	static room patio{ "resources/json/patio.json" };
	static room mario{ "resources/json/mario.json" };
	static room redHall1{ "resources/json/redHall1.json" };
	static room redHall2{ "resources/json/redHall2.json" };
	static room tower1{ "resources/json/tower1.json" };
	static room boss{ "resources/json/boss.json" };

	allRooms.push_back(&entrance);
	allRooms.push_back(&hall1);
	allRooms.push_back(&hall2);
	allRooms.push_back(&cavern1);
	allRooms.push_back(&cavern2);
	allRooms.push_back(&patio);
	allRooms.push_back(&redHall1);
	allRooms.push_back(&redHall2);
	allRooms.push_back(&tower1);
	allRooms.push_back(&boss);
	allRooms.push_back(&mario);

	entrance.area = 0;
	hall1.area = 0;
	hall2.area = 0;
	patio.area = 1;
	cavern1.area = 2;
	cavern2.area = 2;
	redHall1.area = 3;
	redHall2.area = 3;
	tower1.area = 4;
	boss.area = 4;
	mario.area = 5;




	entrance.exit1 = &hall1;

	hall1.exit0 = &entrance;
	hall1.exit1 = &hall2;

	hall2.exit0 = &hall1;
	hall2.exit1 = &cavern1;
	hall2.exit2 = &redHall2;
	hall2.exit3 = &patio;

	cavern1.exit0 = &hall2; //ONE WAY
	cavern1.exit1 = &cavern2;
	cavern1.exit2 = &mario;

	cavern2.exit0 = &cavern1;
	cavern2.exit1 = &patio;

	patio.exit0 = &hall2;
	patio.exit2 = &redHall1;
	patio.exit1 = &cavern2;

	redHall2.exit0 = &hall2;
	redHall2.exit1 = &tower1;

	redHall1.exit0 = &patio;
	redHall1.exit1 = &tower1;

	tower1.exit0 = &redHall2;
	tower1.exit2 = &boss;
	tower1.exit1 = &redHall1;

	boss.exit0 = &tower1;

	//==========================================================//
	//                                 boss						//
	//                                  |						//
	//                           + -  tower1 - +				//
	//                           |             |				//
	//                         redHall2  redHall1				//
	//                             |       |					//
	// null - entrance - hall1 - hall2 - patio					//
	//                             |      |						//
	//                        cavern1 - cavern2					//
	//							   |							//
	//							 mario							//
	//==========================================================//	

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
		case SceneType::CREDITS: GameManager::getInstance().getGamePointer()->setPendingScene(new CreditsScene()); break;
		case SceneType::CUTSCENE: GameManager::getInstance().getGamePointer()->setPendingScene(new CutScene()); break;
	default: GameManager::getInstance().getGamePointer()->setPendingScene(new TitleScene()); break;
	}
	
}

void SceneManager::requestRoomExit(int exit) {
	room* r = getExit(exit);
	int spawn = 0;
		if (r != nullptr) {
			if (currentRoom == r->exit0) spawn = 0;
			else if (currentRoom == r->exit1) spawn = 1;
			else if (currentRoom == r->exit2) {
				spawn = 2;
			}
			else if (currentRoom == r->exit3) {
				spawn = 3;
			}
			currentRoom = r;
			GameManager::getInstance().setArea(currentRoom->area);
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
	int spawn = 0;
	if (saved) spawn = 4;
	GameManager::getInstance().Time.stopTimer();
	currentRoom = saveRoom;
	GameManager::getInstance().setArea(currentRoom->area);
	GameManager::getInstance().getGamePointer()->setPendingScene(new PlayableScene(currentRoom->path, spawn));
	
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
	saved = true;
}