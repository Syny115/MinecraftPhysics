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
	GameManager::getInstance().sceneTime = 0;
	this->path = path;
	parseTiles(path);
	timeLeft.startTimer();
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
	for (int i = projectiles.size() - 1; i >= 0 && !projectiles.empty(); i--) {
		delete projectiles[i];
	}
	projectiles.clear();

	for (Particle& p : particles) delete p.sprite;
	particles.clear();
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
	GameManager::getInstance().sceneTime += GetFrameTime();
	timeLeft.updateTimer();

	if (timeLeft.isTriggerd()) {
		GameManager::getInstance().getGamePointer()->requestSceneReload();
	}

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

	if (!pantherSpawners.empty())
	{
		for (int i = pantherSpawners.size() - 1; i >= 0; i--) {
			Vector2 p = GetWorldToScreen2D(pantherSpawners[i], camera);
			if (CheckCollisionPointRec(p, { 0, 0, screenWidth, screenHeight })) {
				enemies.push_back(new Panther(pantherSpawners[i]));
				pantherSpawners.erase(pantherSpawners.begin() + i);
			}
		}
	}

	if (!Vector2Equals(bossSpawner, {0, 0}))
	{
			Vector2 p = GetWorldToScreen2D(bossSpawner, camera);
			if (CheckCollisionPointRec(p, { 0, 0, screenWidth, screenHeight })) {
				enemies.push_back(new BatBoss(Vector2Add(bossSpawner, {0, 16})));
				bossSpawner = { 0, 0 };
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

	if (GameManager::getInstance().getBossStarted() && GameManager::getInstance().getPlayerHealth() > 0) {
		GameManager::getInstance().getGamePointer()->publicPlayMusicOffset(Game::BLACK_NIGHT, 1.6);
	}

	for (int i = (int)particles.size() - 1; i >= 0; i--) {
		particles[i].timer += GetFrameTime();
		particles[i].sprite->update(GetFrameTime());
		particles[i].position.y += particles[i].velocity.y * GetFrameTime();
		particles[i].position.x += particles[i].velocity.x * GetFrameTime();
		if (particles[i].timer >= particles[i].duration) {
			delete particles[i].sprite;
			particles.erase(particles.begin() + i);
		}
	}

	Scene::updateScene();
}

void PlayableScene::drawScene() {
	ClearBackground(/*{0xA0, 0xF0, 0xFF, 0xFF}*/BLACK);
	BeginMode2D(camera);

	

	drawTiles();
	if (GameManager::getInstance().debugMode) {
		for (int i = 0; i < solidRects.size(); i++) {
			DrawRectangleRec(solidRects[i], WHITE);
			DrawRectangle(solidRects[i].x + 1, solidRects[i].y + 1, solidRects[i].width - 2, solidRects[i].height - 2, GREEN);
		}
	}

	player->drawPlayer();

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

	if (GameManager::getInstance().debugMode)
	{
		if (!zombieSpawners.empty()) {
			for (int i = 0; i < zombieSpawners.size(); i++) DrawRectangleLinesEx(zombieSpawners[0], 1, BLUE);
		}
		if (!zombieSpawners.empty()) {
			for (int i = 0; i < zombieSpawners.size(); i++) DrawRectangleLinesEx(zombieSpawners[0], 1, BLUE);
		}
		if (!destructableRects.empty()) {
			for (int i = 0; i < destructableRects.size(); i++) DrawRectangleLinesEx(destructableRects[0], 1, ORANGE);
		}
		DrawRectangleLinesEx(nextArea, 3, GREEN);
		if (!enemyRects.empty()) {
			for (int i = 0; i < enemyRects.size(); i++) {
				DrawRectangleLinesEx(*enemyRects[i].rect, 2, RED);
			}
		}
		if (!playerHitBoxes.empty()) {
			for (int i = 0; i < playerHitBoxes.size(); i++) {
				DrawRectangleLinesEx(*playerHitBoxes[i].rect, 1, WHITE);
			}
		}
	}


	for (int i = 0; i < particles.size(); i++) {
		particles[i].sprite->draw(particles[i].position);
	}
	

	EndMode2D();
	
	ui.renderUI();
	ui.updateUI();
	
	if (GameManager::getInstance().debugMode)  {
		DrawText(debug_text1.c_str(), 0, 146, 50, WHITE);
		DrawText(debug_text2.c_str(), 0, 196, 50, WHITE);
	}


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
	if (p->getOwner() == Projectile::PLAYER) pushPlayerHitBoxes({ rec, p->getDamage() });
	if (p->getOwner() == Projectile::ENEMY) pushEnemyRects({ rec, p->getDamage() });
	if (p->getOwner() == Projectile::ENEMY) pushEnemyRects({ rec, p->getDamage() });
	projectiles.push_back(p); 
	 
}

void PlayableScene::spawnParticle(Vector2 position) {
	Particle p;
	p.position = { position.x - 2.5f, position.y - 10.0f };
	p.velocity = { 0, 0 };
	p.timer = 0;
	p.duration = 0.4f;
	p.sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::PARTICLES);
	p.sprite->setAnimation("death");
	particles.push_back(p);
}

void PlayableScene::spawnHitEffect(Vector2 position) {
	Particle p;
	p.position = { position.x + 0.0f, position.y + 0.0f };
	p.velocity = { 0, 0 };
	p.timer = 0;
	p.duration = 0.2f;
	p.sprite = new SpriteRenderer("resources/sprites/enemies_sprites.png", SpriteRenderer::HIT_EFFECT);
	p.sprite->setAnimation("hitEffect");
	p.sprite->setFlipX(GetRandomValue(0, 1) == 1);
	particles.push_back(p);
}

// Tile screen

TitleScene::TitleScene()
{
	camera.zoom = screenWidth / viewportWidth;
	background = LoadTexture("resources/sprites/Title_BG.png");
}

TitleScene::~TitleScene()
{
	UnloadTexture(background);
}

void TitleScene::updateScene() {
	if (IsKeyPressed(KEY_ENTER)) GameManager::getInstance().getGamePointer()->requestNextLevel();
}

void TitleScene::drawScene() {
	ClearBackground(BLACK);
	camera.offset = { 0 , 0};
	BeginMode2D(camera);
	DrawTexture(background, 0, 0, WHITE);
	EndMode2D();
}


// LORE screen

LoreScene::LoreScene()
{
	camera.zoom = screenWidth / viewportWidth;
}

LoreScene::~LoreScene()
{
}

void LoreScene::updateScene() {
	if (IsKeyPressed(KEY_ENTER)) GameManager::getInstance().getGamePointer()->requestNextLevel();
}

void LoreScene::drawScene() {
	ClearBackground(BLACK);
	camera.offset = { 0 , 0};
	BeginMode2D(camera);
	DrawText("This is Project 1 of Bachelor's ,", 16, 16, 16, WHITE);
	DrawText("degree in Video Game Design,", 16, 32, 16, WHITE);
	DrawText("and Development at CITM UPC,", 16, 48, 16, WHITE);
	DrawText("in which we must recreate", 16, 64, 16, WHITE);
	DrawText("Castlevania game with raylib.", 16, 64+16, 16, WHITE);
	DrawText("By Fernando Garcia,", 16, 64+48, 16, WHITE);
	DrawText("Adria Pavon", 16, 64+48+16, 16, WHITE);
	DrawText("Joao de Queiroz. Our bossman", 16, 64+48+16+16, 16, WHITE);
	DrawText("is Rodrigo de Pedro", 16, 64+48+32+16, 16, WHITE);
	DrawText("Press start, dingus!", 8, 64+48+32+16+32-8, 24, WHITE);
	EndMode2D();
}


