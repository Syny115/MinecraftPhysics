#include "Scene.h"
#include "GameManager.h"


//REPLACE LATER WHEN I HAVE GAME MANAGER
const float screenWidth = 800;
const float screenHeight = 700;

const float viewportWidth = 256;
const float viewportHeight = 224;


Scene::Scene() {
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = screenWidth / viewportWidth;
	GameManager::getInstance().getGamePointer()->publicPlayLevelMusic();
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

PlayableScene::PlayableScene(const char* path) : Scene() {
	this->path = path;
	parseTiles(path);
}

void PlayableScene::start() {
	while (checkpoints[0].x == 0 && checkpoints[0].y == 0  && GameManager::getInstance().getCheck() > 0) GameManager::getInstance().addCheck(-1);
	player = new Player(checkpoints[GameManager::getInstance().getCheck()]);
	GameManager::getInstance().getGamePointer()->publicPlayLevelMusic();
}


PlayableScene::~PlayableScene() {
	delete player;
	UnloadTexture(tileAtlas);
	for (Loot* loot : lootitems) if (loot != nullptr && loot != (Loot*)0xFFFFFFFFFFFFFFFF) delete loot;
	lootitems.clear();
	for (int i = enemies.size()-1; i >= 0 && !enemies.empty(); i--) {
		delete enemies[i];
	}
	enemies.clear();
	for (DestructableObject* dest : destructables) delete dest;
	destructables.clear();
	for (Projectile* proj : projectiles) if (proj != nullptr && proj != (Projectile*)0xFFFFFFFFFFFFFFFF) delete proj;
	projectiles.clear();
}



void PlayableScene::updateCamera() {
	if (GetWorldToScreen2D(bossStart, camera).x > 0 || bossStart.x < 0) 
	{
		camera.target = player->getPosition();
		camera.target = { Clamp(camera.target.x, viewportWidth / 2.0f, worldWidth - viewportWidth / 2.0f), Clamp(camera.target.y, viewportHeight / 2.0f, worldHeight - viewportHeight / 2.0f) };
	}
	else {
		GameManager::getInstance().setBossStarted(true);
	}
}

void PlayableScene::updateScene() {
	printf("\tBefore Update: %d\n", enemies.size());
	if (player != nullptr)
	{
		player->groundCollision(solidRects);
		player->ceilingCollision(solidRects);
		player->wallCollision(solidRects);
		player->stairCollision(stairs);
		player->enemyCollision(enemyRects);
		player->update();

		if (CheckCollisionPointRec(player->getPosition(), nextArea)) GameManager::getInstance().getGamePointer()->requestNextLevel();
	}

	if (!destructables.empty()) {
		for (int i = 0; i < destructables.size(); i++) {
			destructables[i]->update();
			destructables[i]->hitCollision(playerHitBoxes);
		}
	}

	if (!lootitems.empty()) {
		for (int i = (int)lootitems.size() - 1; i >= 0; i--) { //si se sacan con player collision se puede saltar algun update, por eso iteramos al reves
			if (!lootitems[i]->playerCollision(player->getHurtbox())) {
				lootitems[i]->groundCollision(solidRects);
				lootitems[i]->update();
			}
		}
	}

	spawnCoolDown.updateTimer();

	if (!zombieSpawners.empty()) 
	{
		for (int i = 0; i < zombieSpawners.size();i ++)
		if (CheckCollisionPointRec(player->getPosition(), zombieSpawners[i])
			&& enemies.size() < 10 && !spawnCoolDown.isActive()) {
			spawnCoolDown.startTimer();
			enemies.push_back(new Zombie(GetScreenToWorld2D(Vector2{ screenWidth+100, screenHeight }, camera)));
		}
	}
	if (!medusaSpawners.empty())
	{
		for (int i = 0; i < medusaSpawners.size(); i++)
		{
			if (CheckCollisionPointRec(player->getPosition(), medusaSpawners[i])
				&& enemies.size() < 10 && !spawnCoolDown.isActive()) {
				spawnCoolDown.startTimer();
				Vector2 p = GetScreenToWorld2D(Vector2{ screenWidth + 100, 0 }, camera);
				p.y = player->getPos().y;
				enemies.push_back(new Medusa(p));
			}
		}
	}

	if (!batSpawners.empty())
	{
		for (int i = batSpawners.size()-1; i >= 0; i--) {
			Vector2 p = GetWorldToScreen2D(batSpawners[i], camera);
			if (CheckCollisionPointRec(p, { 0, 0, screenWidth, screenHeight })) {
				enemies.push_back(new Bat(batSpawners[i]));
				batSpawners.erase(batSpawners.begin() + i);
			}
		}
	}

	if (!enemies.empty()) {
		for (int i = (int)enemies.size() - 1; i >= 0; i--) {
			enemies[i]->hitCollision(playerHitBoxes);
			enemies[i]->groundCollision(solidRects);
			enemies[i]->update();
			
		}
	}

	if (!projectiles.empty()) {
		for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
			if (!projectiles[i]->boolGroundCollision(solidRects) && 
				!projectiles[i]->enemyCollision(enemyRects) && 
				!projectiles[i]->playerCollision(player->getHurtbox())) {
				projectiles[i]->update();
			}

		}
	}
	Scene::updateScene();
	printf("\After Update: %d\n", enemies.size());

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
	//if (!zombieSpawners.empty()) DrawRectangleLinesEx(zombieSpawners[0], 1, BLUE);
	//if (!medusaSpawners.empty()) DrawRectangleLinesEx(medusaSpawners[0], 1, BLUE);
	
	/*DrawRectangleRec(stairs[1].start, RED);
	DrawRectangleRec(stairs[1].end, RED);*/
	/*DrawRectangleRec(stairs[2].start, GREEN);
	DrawRectangleRec(stairs[2].end, GREEN);*/
	//DrawRectangleRec(stairs[0].start, BLUE);
	//DrawRectangleRec(stairs[0].end, BLUE);

	if (!lootitems.empty()) {
		for (int i = 0; i < lootitems.size(); i++) {
			lootitems[i]->Draw();
		}
	}
	if (!enemies.empty()) {
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->draw();
		}
	}
	if (!projectiles.empty()) {
		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i]->draw();
		}
	}
	//DrawRectangleLinesEx(nextArea, 3, GREEN);
	//if (!lootRects.empty()) {
	//	for (int i = 0; i < lootRects.size(); i++) {
	//		
	//		
	//		//DrawRectangleRec();
	//	}
	//}
	//if (!enemyRects.empty()) {
	//	for (int i = 0; i < enemyRects.size(); i++) {
	//		DrawRectangleLinesEx(*enemyRects[i].rect, 2, RED);
	//		//DrawRectangleRec();
	//	}
	//}
	//if (!playerHitBoxes.empty()) {
	//	for (int i = 0; i < playerHitBoxes.size(); i++) {
	//		DrawRectangleLinesEx(*playerHitBoxes[i].rect, 1, BLUE);
	//		//DrawRectangleRec();
	//	}
	//}
	
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
		if (playerHitBoxes[i].rect == hitBox) {playerHitBoxes.erase(playerHitBoxes.begin() + i);
		TraceLog(LOG_INFO, "Remove: rect=%p  INDEX: %d", hitBox, i);
	}
		else TraceLog(LOG_INFO, "Not Removed: rect=%p  INDEX: -1", hitBox);
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

void PlayableScene::removeEnemy(Enemy* e) {
	removeEnemyRects(e->getHurtboxPtr());
	
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] == e) enemies.erase(enemies.begin() + i);
	}
	
}

void PlayableScene::removeEnemyRects(Rectangle* hitbox) {
	for (int i = 0; i < enemyRects.size(); i++) {
		if (enemyRects[i].rect == hitbox) enemyRects.erase(enemyRects.begin() + i);
	}
}

void PlayableScene::removeProjectile(Projectile* p) {
	if (p->getOwner() == Projectile::PLAYER) removePlayerHitBoxes(p->getHurtboxPtr());
	if (p->getOwner() == Projectile::ENEMY) removeEnemyRects(p->getHurtboxPtr());
	for (int i = 0; i < projectiles.size(); i++) {
		if (projectiles[i] == p) projectiles.erase(projectiles.begin() + i);
		GameManager::getInstance().addProjectileCount(-1);
	}
}

void PlayableScene::pushProjectile(Projectile* p) { 
	Rectangle* rec = p->getHurtboxPtr();
	projectiles.push_back(p); 
	pushPlayerHitBoxes({ rec, p->getDamage() }); 
}