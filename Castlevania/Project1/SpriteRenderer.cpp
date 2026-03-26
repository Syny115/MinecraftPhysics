#include "SpriteRenderer.h"

int SpriteRenderer::findAnimation(const string& name) const {
    for (int i = 0; i < (int)animationLibrary.size(); i++)
        if (animationLibrary[i].name == name) return i;
    return 0;
}

void SpriteRenderer::setAnimation(const string& animationName) {
    if (currentAnimName == animationName) return;
    currentAnimName = animationName;
    currentFrame = 0;
    frameTimer = 0;
}

void SpriteRenderer::update(float deltaTime) {
    if (animationLibrary.empty()) return;
    Animation& anim = animationLibrary[findAnimation(currentAnimName)];
    frameTimer += deltaTime;
    if (frameTimer >= 1.0f / anim.fps) {
        frameTimer = 0;
        currentFrame = (currentFrame + 1) % anim.totalFrames;
    }
}

void SpriteRenderer::draw(Vector2 position) {
    if (animationLibrary.empty()) return;
    Animation& anim = animationLibrary[findAnimation(currentAnimName)];

    // Los frames NO son contiguos en todos los casos,
    // por eso guardamos los offsets de cada frame individualmente
    float fx = anim.originX + currentFrame * anim.frameWidth;
    float fy = anim.originY;

    Rectangle source = {
        fx,
        fy,
        flipX ? -anim.frameWidth : anim.frameWidth,
        anim.frameHeight
    };
    DrawTextureRec(sprite, source, position, WHITE);
}

SpriteRenderer::SpriteRenderer(const char* path, int spriteType)
    : currentFrame(0), frameTimer(0), flipX(false)
{
    sprite = LoadTexture(path);

    // Todos los sprites de Simon son 16x32
    // Fila superior: y=16, altura=32
    // Fila inferior: y=48, altura=32 (pero los sprites usan y=48 con h=32 -> hasta y=80)

    switch (spriteType) {
    case PLAYER: {
        auto add = [&](string name, int frames, float ox, float oy, float fps) {
            Animation a;
            a.name = name;
            a.totalFrames = frames;
            a.originX = ox;
            a.originY = oy;
            a.frameWidth = 16;
            a.frameHeight = 32;
            a.fps = fps;
            animationLibrary.push_back(a);
            };

        // FILA Y=16 — animaciones principales
        add("idle", 1, 0, 16, 1);   // x=0,  parado
        add("walk", 4, 16, 16, 8);   // x=16..64, caminar (4 frames contiguos)
        add("attack", 2, 192, 16, 10);  // x=192..208, látigo extendido
        add("hurt", 2, 224, 16, 6);   // x=224..240, recibir daño
        add("dead", 1, 272, 16, 1);   // x=272, muerto

        // FILA Y=48 — estados especiales
        add("crouch", 1, 32, 48, 1);   // x=32, agachado
        add("jump", 1, 80, 48, 1);   // x=80, salto
        add("land", 1, 128, 48, 1);   // x=128, aterrizaje
        add("stairs", 4, 160, 48, 6);   // x=160..224, subir escaleras

        currentAnimName = "idle";
        break;
    }
    case ZOMBIE:
        // TODO
        break;
    case BAT:
        // TODO
        break;
    default:
        break;
    }
}

SpriteRenderer::~SpriteRenderer() {
    UnloadTexture(sprite);
}