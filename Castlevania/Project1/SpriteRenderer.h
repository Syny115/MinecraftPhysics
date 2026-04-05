#pragma once
#include "raylib.h"
#include <string>
#include <vector>
using namespace std;

class SpriteRenderer {
private:
    Image rawSprite;
    Texture2D sprite;

    struct Animation {
        string name;
        int totalFrames;
        float frameWidth;
        float frameHeight;
        float originX;   // X en el spritesheet donde empieza esta animación
        float originY;   // Y en el spritesheet donde empieza esta animación
        float fps;       // frames por segundo de esta animación
        float offset; // por si alguna animacion tiene un relieve mayor o menor
    };

    vector<Animation> animationLibrary;

    // Estado interno
    string currentAnimName;
    int currentFrame;
    float frameTimer;
    bool flipX;          // para mirar a la izquierda

    int findAnimation(const string& name) const;

public:
    enum Type { PLAYER_TOP, PLAYER_BOTTOM, ZOMBIE, BAT, BOSS, LOOT, WHIP };

    SpriteRenderer() : currentFrame(0), frameTimer(0), flipX(false) {}
    SpriteRenderer(const char* path, int spriteType);
    ~SpriteRenderer();

    void setAnimation(const string& animationName); // cambia animación sin resetear si es la misma
    void setFlipX(bool flip) { flipX = flip; }
    void update(float deltaTime);                   // avanza el timer
    void draw(Vector2 position);                    // dibuja el frame actual
    string getAnimation() { return currentAnimName; }
};