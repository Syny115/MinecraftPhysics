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

PlayableScene::PlayableScene(const char* path) {
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = screenWidth / viewportWidth;
	parseTiles(path);
	
}

void PlayableScene::start() {
	player = new Player;
	spriteAnimation = SpriteRenderer("resources/sprites/enemies_sprites.png", 2);

}


PlayableScene::~PlayableScene() {
	delete player;
	UnloadTexture(tileAtlas);
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

		//spriteAnimation.setAnimation("walk"); // o lo que corresponda
		spriteAnimation.setFlipX(false);      // true si el jugador va a la izquierda
		spriteAnimation.update(GetFrameTime());
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
	spriteAnimation.draw(Vector2{ 100, 50 });
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
