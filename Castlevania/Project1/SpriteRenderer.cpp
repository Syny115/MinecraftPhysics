#include "SpriteRenderer.h"


void SpriteRenderer::playAnimation(string animationName, int currentFrame, Vector2 position) {
	int animIndex = 0;
	while (animationLibrary[animIndex].name != animationName) { animIndex++; }
	animation currentAnim = animationLibrary[animIndex];

	Rectangle source = { currentAnim.framePos.x + currentAnim.hOffset, currentAnim.framePos.y + currentAnim.vOffset, currentAnim.frameWidth, currentAnim.frameHeigth };

	
	DrawTextureRec(sprite, source, position, WHITE);
}

SpriteRenderer::SpriteRenderer(const char* path, int spriteType) {
	sprite = LoadTexture(path);
	animation walkAnim;
	switch (spriteType) {
	case PLAYER:
		walkAnim.name = "walk";
		walkAnim.totalFrames = 4;
		walkAnim.framePos.x = 0;
		walkAnim.framePos.y = 0;
		walkAnim.hOffset = 1;
		walkAnim.vOffset = 1;
		walkAnim.frameHeigth = 32;
		walkAnim.frameWidth = 16;

		animationLibrary.push_back(walkAnim);
		break;
	case ZOMBIE:
		break;
	case BAT:
		break;
	case BOSS:
		break;
	default:
		break;
	}
}

SpriteRenderer::~SpriteRenderer() {
	UnloadTexture(sprite);
}


