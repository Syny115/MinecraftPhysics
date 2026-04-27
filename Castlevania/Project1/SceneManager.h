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
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void requestSceneLoad(SceneType type);
	void requestRoomExit(int exit);
	void requestFirstRoomInArea(int area);
	room* getExit(int exit);

private:
	room* head;
	room* currentRoom;
	vector<room*> areas;
};


