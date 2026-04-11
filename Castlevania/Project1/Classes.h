#pragma once
#include "raylib.h"
struct damageRect
{
    Rectangle* rect;
    unsigned short damage;
};

struct staircase
{
    Rectangle start;
    Rectangle end;
    bool up;
};

class Timer
{
private:
    float lifeTime;
    float time = lifeTime;
    bool active = false;
public:
    float getTime() { return time; }
    float getLifeTime() { return lifeTime; }
    Timer() { lifeTime = 10; time = 10; };
    Timer(float _lifeTime) : lifeTime(_lifeTime) { time = _lifeTime; }

    void startTimer() {
        time = lifeTime;
        active = true;
    }

    void startTimer(int t) {
        time = t;
        lifeTime = t;
        active = true;
    }

    void updateTimer() {
        if (!active) return;
        time -= GetFrameTime();
        if (time <= 0) active = false;
    }

    bool isTriggerd() {
        return time <= 0;
    }

    bool isActive() {
        return active;
    }
};