#pragma once

class Timer
{
private:
    float lifeTime;
    float time = lifeTime;
    bool active = false;
public:
    float getTime() { return time; }
    float getLifeTime() { return lifeTime; }

    Timer(float _lifeTime) : lifeTime(_lifeTime) { time = _lifeTime; }

    void startTimer() {
        time = lifeTime;
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