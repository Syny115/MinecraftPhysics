#pragma once

class Timer
{
private:
    float lifeTime;
    float time = lifeTime;
public:
    float getTime() { return time; }
    float getLifeTime() { return lifeTime; }

    Timer(float _lifeTime) : lifeTime(_lifeTime), time(_lifeTime) {}

    void startTimer() {
        time = lifeTime;
    }

    void updateTimer() {
        time -= GetFrameTime();
    }

    bool isTriggerd() {
        return time <= 0;
    }
};