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
    float fx = anim.originX + currentFrame * anim.frameWidth + currentFrame * anim.offset;
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
    rawSprite = LoadImage(path);
    Color target = { 119, 51, 153, 255 };
    ImageColorReplace(&rawSprite, target, BLANK);

    sprite = LoadTextureFromImage(rawSprite);
    UnloadImage(rawSprite);

    // Todos los sprites de Simon son 16x32 pero el frame en si es 20x41 (dentro del relieve lila)
    // Fila superior: y=7, altura=41
    // Fila inferior: y=49, altura=41 (pero los sprites usan y=48 con h=41 -> hasta y=89)

    auto add = [&](string name, int frames, float ox, float oy, float w, float h, float off, float fps) {
        Animation a;
        a.name = name;
        a.totalFrames = frames;
        a.originX = ox;
        a.originY = oy;
        a.frameWidth = w;
        a.frameHeight = h;
        a.fps = fps;
        a.offset = off;
        animationLibrary.push_back(a);
        };

    switch (spriteType) {
    case PLAYER_TOP:
        //fila uno de top seria y=7 a y=30 (24 pixels) y de bottom seria y=31 a y = 46 (16 pixels)

        add("idle", 1, 1, 7, 20, 24, 1, 1);   // x=0,  parado
        add("walk", 3, 22, 7, 20, 24, 1, 9);   // x=22..64, caminar (3 frames contiguos)
        add("startAttack", 2, 85, 7, 41, 23, 1, 7);
        add("attack", 1, 169, 7, 41, 24, 1, 1);
        //TODO: hacer un set fps para que los timers siempre estan sincronizados
        add("hurt", 1, 211, 7, 20, 24, 1, 1);
        add("stairsIdle", 1, 320, 7, 20, 24, 1, 1);
        add("stairsWalk", 2, 341, 7, 20, 24, 1, 10);
        add("dead", 1, 0, 0, 0, 0, 1, 0);

        currentAnimName = "idle";
        break;
    case PLAYER_BOTTOM:
        //fila uno de top seria y=7 a y=30 (24 pixels) y de bottom seria y=31 a y = 46 (16 pixels)


        add("idle", 1, 1, 31, 20, 16, 1, 1);   // x=0,  parado
        add("walk", 3, 22, 31, 20, 16, 1, 9);   // x=22..64, caminar (3 frames contiguos)
        add("attack", 1, 85, 31, 41, 16, 1, 1);
        //TODO: hacer un set fps para que los timers siempre estan sincronizados
        add("dead", 1, 211, 70, 41, 19, 1, 1);   // x=211
        add("crouch", 1, 22, 79, 20, 8, 1, 1);   // x=22, agachado // Jump uses this also
        add("stairsIdleUp", 1, 320, 31, 20, 16, 1, 1);
        add("stairsWalkUp", 2, 341, 31, 20, 16, 1, 10);
        add("stairsIdleDown", 1, 320, 73, 20, 16, 1, 1); // Hurt also uses this
        add("stairsWalkDown", 2, 341, 73, 20, 16, 1, 10);

        currentAnimName = "idle";
        break;
    case WHIP:
        add("hidden", 3, 0, 0, 0, 0, 1, 1);
        add("shortStart", 2, 1, 91, 20, 26, 1, 7);
        add("longStart", 2, 43, 91, 20, 26, 1, 7);
        add("lv1Attack", 1, 85, 91, 62, 20, 1, 1);
        add("lv2Attack", 1, 85, 112, 62, 20, 1, 1);
        add("lv3Attack", 1, 85, 133, 62, 20, 1, 1);

        currentAnimName = "hidden";
        break;
    case ZOMBIE:
        add("walk", 2, 0, 0, 16, 32, 0, 1);
        currentAnimName = "walk";
        break;
    case BAT:
        add("batFly", 3, 32, 32, 16, 16, 0, 6);
        add("batIdle", 1, 80, 32, 16, 16, 0, 1);
        currentAnimName = "batIdle";
        break;
    case MEDUSA:
        add("medusaFly", 1, 0, 0, 0 ,0, 0, 1);
        break;
    case PANTHER:
        add("pantherWalk", 2, 0, 0, 16, 16, 0, 1);
		add("panterIdle", 1, 32, 0, 16, 16, 0, 1);
        currentAnimName = "pantherIdle";
		break;
    case LOOT:

        add("whip", 1, 1, 1, 16, 16, 1, 1); //Whip upgrade loot item
        add("smallheart", 1, 18, 1, 8, 8, 1, 1);
        add("bigheart", 1, 27, 1, 16, 16, 1, 1);
        add("moneybag", 3, 44, 1, 16, 16, 1, 12);
        add("orb", 2, 95, 1, 16, 16, 1, 8);
        add("dagger", 1, 1, 18, 16, 16, 1, 1);
        add("axe", 1, 18, 18, 16, 16, 1, 1);
        add("holywater", 1, 35, 18, 16, 16, 1, 1);
        add("boomerang", 3, 52, 18, 16, 16, 1, 12);
        add("stopwatch", 1, 103, 18, 16, 16, 1, 1);
        add("twoupgrade", 1, 120, 18, 16, 16, 1, 1);
        add("threeupgrade", 1, 137, 18, 16, 16, 1, 1);
        add("chicken", 1, 1, 35, 16, 16, 1, 1);
        add("jar", 1, 18, 35, 16, 16, 1, 1);
        add("invincibilitycross", 1, 35, 35, 16, 16, 1, 1);
        add("1up", 1, 52, 35, 16, 16, 1, 1); //Probably delete this ngl im not sure about it being a loot item
        add("crown", 4, 69, 35, 16, 16, 1, 12);
        add("chest", 4, 137, 35, 16, 16, 1, 12);
        currentAnimName = "whip";
    case BREAKABLES:
        add("ground", 1, 32, 96, 16, 16, 1, 1);
        add("candle", 2, 157, 1, 8, 16, 1, 10); //needs offset!!
        add("torch", 2, 175, 1, 16, 32, 1, 10); //needs offset!!
        currentAnimName = "torch";

        break;
    default:
        break;
    }
}


SpriteRenderer::~SpriteRenderer() {
    UnloadTexture(sprite);
}