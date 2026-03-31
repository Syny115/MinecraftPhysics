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
    float fx = anim.originX + currentFrame * anim.frameWidth + currentFrame;
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

    // Todos los sprites de Simon son 16x32 pero el frame en si es 20x41 (dentro del relieve lila)
    // Fila superior: y=7, altura=41
    // Fila inferior: y=49, altura=41 (pero los sprites usan y=48 con h=41 -> hasta y=89)

    switch (spriteType) {
    case PLAYER: { // hay que distinguir entre player TOP y player BOTTOM (así no tenemos que hacer sprites agachadas i atacando)
        //fila uno de top seria y=7 a y=29 (23 pixels) y de bottom seria y=30 a y = 47 (18 pixels)
        //Animaciones para TOP: idle, walk, attack, attack start, stun/hurt (hurt y stun tienen la misma aparencia), stair idle, stair walk (da igual la direccion)
        //Animaciones para BOTTOM: idle, walk, attack, crouch, dead, stair Idle Up, stair walk Up, stair idle down, stair walk down, hurt (reuses stair idle down)
        auto add = [&](string name, int frames, float ox, float oy, float fps) {
            Animation a;
            a.name = name;
            a.totalFrames = frames;
            a.originX = ox;
            a.originY = oy;
            a.frameWidth = 20;
            a.frameHeight = 41;
            a.fps = fps;
            animationLibrary.push_back(a);
            };

        // FILA Y=16 — animaciones principales
        add("idle", 1, 1, 7, 1);   // x=0,  parado
        add("walk", 3, 22, 7, 9);   // x=22..64, caminar (3 frames contiguos)
        //below are not corrected
        add("attack", 2, 85, 7, 10);  // x=192..208, látigo extendido
        //Deberia haber un start attack y un attack. asi podemos tocar los timers TODO: hacer un set fps para que los timers siempre estan sincronizados
        add("hurt", 2, 224, 16, 6);   // x=224..240, recibir daño
        add("dead", 1, 272, 16, 1);   // x=272, muerto

        // FILA Y=48 — estados especiales
        add("crouch", 1, 22, 49, 1);   // x=32, agachado
        add("jump", 1, 22, 49, 1);   // x=80, salto
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