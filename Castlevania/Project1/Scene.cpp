#include "Scene.h"

//REPLACE LATER WHEN I HAVE GAME MANAGER
const float screenWidth = 800;
const float screenHeight = 700;

const float viewportWidth = 256;
const float viewportHeight = 224;


Scene::Scene() {
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = screenWidth / viewportWidth;
}

void Scene::updateCamera() {
	camera.target = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
}

void Scene::drawScene() {
	ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });
}

void Scene::updateScene() {
	updateCamera();
}

//----------- PLAYABLE SCENE -------------//

PlayableScene::PlayableScene(const char* path) {
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = screenWidth / viewportWidth;
	parseTiles(path);
}

PlayableScene::~PlayableScene() {
	UnloadTexture(tileAtlas);
}

void PlayableScene::updateCamera() {
	camera.target = player.getPosition();
	camera.target = { Clamp(camera.target.x, viewportWidth / 2.0f, worldWidth - viewportWidth / 2.0f), Clamp(camera.target.y, viewportHeight / 2.0f, worldHeight - viewportHeight / 2.0f) };
}

void PlayableScene::updateScene() {
	player.groundCollision(solidRects);
	player.ceilingCollision(solidRects);
	player.wallCollision(solidRects);
	player.update();
	updateCamera();
}

void PlayableScene::drawScene() {
	ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });
	BeginMode2D(camera);
		/*DrawRectangleRec(solidRects[0], BLUE);
		DrawRectangleRec(solidRects[1], BLUE);
		DrawRectangleRec(solidRects[2], BLUE);
		DrawRectangleRec(solidRects[3], BLUE);*/
		drawTiles();
		player.drawPlayer();
	EndMode2D();
}

void PlayableScene::drawTiles() {
	Rectangle rect;
	int row = worldHeight / tileHeight;
	int col = worldWidth / tileWidth;

	float _x, _y;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (tileMat[i][j] != 0) {
				_x = (tileMat[i][j] - 1) % (tileAtlas.width / tileWidth);
				_y = (tileMat[i][j] - 1) / (tileAtlas.height / tileHeight);
				rect = { _x * tileWidth, _y * tileHeight, (float)tileWidth, (float)tileHeight };
				DrawTextureRec(tileAtlas, rect, Vector2{ (float)j * tileWidth,(float)i * tileHeight }, WHITE);
			}
		}
	}
}

void PlayableScene::parseTiles(const char* path) {
	ifstream data(path);
	json doc = json::parse(data);
	int row = doc["height"];
	int col = doc["width"];

	vector<vector<int>> collisionMat(row, vector<int>(col));
	tileMat = collisionMat;
	tileHeight = doc["tileheight"];
	tileWidth = doc["tilewidth"];
	worldHeight = row * tileHeight;
	worldWidth = col * tileWidth;


	int background = 0; int foreground = 0;
	int len = doc["layers"].size();
	for (int i = 0; i < len; i++) {
		if (doc["layers"][i]["name"] == "BackGround") background = i;
		if (doc["layers"][i]["name"] == "ForeGround") foreground = i;
	}
	len = row * col;
	int bData, fData;
	for (int j = 0; j < len; j++) {
		bData = doc["layers"][background]["data"][j];
		fData = doc["layers"][foreground]["data"][j];
		tileMat[j / col][j % col] = bData;
		if (fData != 0) {
			collisionMat[j / col][j % col] = 1;
			tileMat[j / col][j % col] = fData;
		}
		else collisionMat[j / col][j % col] = 0;
		
	}
	data.close();
}