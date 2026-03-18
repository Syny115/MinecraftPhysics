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

	solidRects.push_back({ 0, 192, 256, 32 });
	solidRects.push_back({ 0, 160, 32, 32 });
	solidRects.push_back({ 224, 128, 32, 64 });
	solidRects.push_back({ 224 - 64, 128 - 32, 64 + 32, 32 });
}

void Scene::updateCamera() {
	camera.target = player.getPosition();
	camera.target = { Clamp(camera.target.x, viewportWidth / 2.0f, worldWidth - viewportWidth / 2.0f), Clamp(camera.target.y, viewportHeight / 2.0f, worldHeight - viewportHeight / 2.0f) };
}

void Scene::updateScene() {
	player.groundCollision(solidRects);
	player.ceilingCollision(solidRects);
	player.wallCollision(solidRects);
	player.update();
	updateCamera();
}

void Scene::drawScene() {
	ClearBackground({ 0xA0, 0xF0, 0xFF, 0xFF });
	BeginMode2D(camera);
		DrawRectangleRec(solidRects[0], BLUE);
		DrawRectangleRec(solidRects[1], BLUE);
		DrawRectangleRec(solidRects[2], BLUE);
		DrawRectangleRec(solidRects[3], BLUE);
		player.drawPlayer();
	EndMode2D();
}