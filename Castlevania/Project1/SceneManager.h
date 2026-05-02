#pragma once
#include <vector>
#include "Classes.h"
using namespace std;

struct room {
	const char* path;
	room* exit0 = nullptr; //ALWAYS FOR THE WAY BACK, IS NULL ON FIRST ROOM
	room* exit1 = nullptr;
	room* exit2 = nullptr;
	room* exit3 = nullptr;
	int area;
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void requestSceneLoad(SceneType type);
	void requestRoomExit(int exit);
	void requestFirstRoomInArea(int area);
	void requestSpecificRoom(int r);
	void requestSaveRoom();
	room* getExit(int exit);
	int getAllRoomsSize();

	void gameSaved();

private:
	room* head;
	room* currentRoom;
	room* saveRoom;
	vector<room*> areas;
	vector<room*> allRooms;
};


