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

void Scene::start() {

}

void Scene::updateCamera() {
	camera.target = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
}

void Scene::drawScene() {
	ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });
}

void Scene::updateScene() {
	if (!deletionQueue.empty()) {
		delete deletionQueue.back();
		deletionQueue.pop();
	}
	updateCamera();
}

//----------- PLAYABLE SCENE -------------//
Texture test;
Timer timertest(10);

PlayableScene::PlayableScene(const char* path) {
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = screenWidth / viewportWidth;
	parseTiles(path);

	// Creating test loot 
	test = LoadTexture("resources/sprites/heart.png");
	lootitems.push_back(new Loot(Vector2{16,16},Vector2{100,100},Vector2{0,20}, test, timertest));
}

void PlayableScene::start() {
	player = new Player;
}


PlayableScene::~PlayableScene() {
	delete player;
	UnloadTexture(tileAtlas);
	UnloadTexture(test);
	for (int i = 0; lootitems.empty(); i++) {
		delete lootitems[i];
	}
}

void PlayableScene::updateCamera() {
	camera.target = player->getPosition();
	camera.target = { Clamp(camera.target.x, viewportWidth / 2.0f, worldWidth - viewportWidth / 2.0f), Clamp(camera.target.y, viewportHeight / 2.0f, worldHeight - viewportHeight / 2.0f) };
}

void PlayableScene::updateScene() {
	if (player != nullptr)
	{
		player->groundCollision(solidRects);
		player->ceilingCollision(solidRects);
		player->wallCollision(solidRects);
		player->update();
	}
	if (!lootitems.empty()) {
		for (int i = 0; i < lootitems.size(); i++) {
			if (CheckCollisionRecs(player->getHurtbox(), lootitems[i]->getHurtbox())) {
				// Add pickup code here
				lootitems[i]->queueDeletion();
			}
			lootitems[i]->groundCollision(solidRects);
			lootitems[i]->update();
		}
	}
	
		
	Scene::updateScene();
}

void PlayableScene::drawScene() {
	ClearBackground(/*{0xA0, 0xF0, 0xFF, 0xFF}*/BLACK);
	BeginMode2D(camera);
		drawTiles();
	/*	for (int i = 0; i < solidRects.size(); i++) {
			DrawRectangleRec(solidRects[i], WHITE);
			DrawRectangle(solidRects[i].x + 1, solidRects[i].y + 1, solidRects[i].width - 2, solidRects[i].height - 2, GREEN);
		}*/
		
		player->drawPlayer();
		if (!lootitems.empty()) {
			lootitems[0]->Draw();
		}
	EndMode2D();
	DrawText(debug_text1.c_str(), 0, 0, 50, WHITE);
	DrawText(debug_text2.c_str(), 0, 50, 50, WHITE);
	
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

void PlayableScene::removePlayerHitBoxes(Rectangle* hitBox) {
	for (int i = 0; i < playerHitBoxes.size(); i++) {
		if (playerHitBoxes[i] == hitBox) playerHitBoxes.erase(playerHitBoxes.begin() + i);
	}
}

void PlayableScene::removeLoot(Loot* l) {
	for (int i = 0; i < lootitems.size(); i++) {
		if (lootitems[i] == l) lootitems.erase(lootitems.begin() + i);
	}
}