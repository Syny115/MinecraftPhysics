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
	while (!deletionQueue.empty()) {
		delete deletionQueue.front();
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
	spriteAnimation = new SpriteRenderer("resources/sprites/CastlevaniaTileset.png", SpriteRenderer::BREAKABLES);
	enemyRects.push_back(damageRect{ new Rectangle{100, 180, 16, 16}, 2 });
}


PlayableScene::~PlayableScene() {
	delete player;
	UnloadTexture(tileAtlas);
	for (Loot* loot : lootitems) delete loot;
	lootitems.clear();
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
		player->stairCollision(stairs);
		player->update();

		for (int i = 0; i < destructables.size(); i++) {
			destructables[i]->update();
			destructables[i]->hitCollision(playerHitBoxes);
		}

		spriteAnimation->setAnimation("ground"); // o lo que corresponda
		spriteAnimation->setFlipX(false);      // true si el jugador va a la izquierda
		spriteAnimation->update(GetFrameTime());
	}


	if (!lootitems.empty()) {
		for (int i = (int)lootitems.size() - 1; i >= 0; i--) { //si se sacan con player collision se puede saltar algun update, por eso iteramos al reves
			if (!lootitems[i]->playerCollision(player->getHurtbox())) {
				lootitems[i]->groundCollision(solidRects);
				lootitems[i]->update();
			}
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
	DrawRectangleRec(*enemyRects[0].rect, RED);
	/*DrawRectangleRec(stairs[0].start, BLUE);
	DrawRectangleRec(stairs[0].end, BLUE);
	DrawRectangleRec(stairs[1].start, RED);
	DrawRectangleRec(stairs[1].end, RED);*/
	/*DrawRectangleRec(stairs[2].start, GREEN);
	DrawRectangleRec(stairs[2].end, GREEN);*/
	//DrawRectangleRec(stairs[0].start, BLUE);
	//DrawRectangleRec(stairs[0].end, BLUE);
	spriteAnimation->draw(Vector2{ 100, 50 });

	if (!lootitems.empty()) {
		for (int i = 0; i < lootitems.size(); i++) {
			lootitems[i]->Draw();
		}
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
	for (int i = 0; i < destructables.size(); i++) {
		destructables[i]->draw();
	}
}

void PlayableScene::removePlayerHitBoxes(Rectangle* hitBox) {
	for (int i = 0; i < playerHitBoxes.size(); i++) {
		if (playerHitBoxes[i].rect == hitBox) playerHitBoxes.erase(playerHitBoxes.begin() + i);
	}
}

void PlayableScene::removeDestructables(DestructableObject* d) {
	for (int i = 0; i < destructables.size(); i++) {
		if (destructables[i] == d) destructables.erase(destructables.begin() + i);
	}
}

void PlayableScene::removeSolidRects(Rectangle rect) {
	for (int i = 0; i < solidRects.size(); i++) {
		if (solidRects[i].x == rect.x && solidRects[i].y == rect.y && solidRects[i].width == rect.width && solidRects[i].height == rect.height) solidRects.erase(solidRects.begin() + i);
	}
}

void PlayableScene::removeLoot(Loot* l) {
	for (int i = 0; i < lootitems.size(); i++) {
		if (lootitems[i] == l) lootitems.erase(lootitems.begin() + i);
	}
}