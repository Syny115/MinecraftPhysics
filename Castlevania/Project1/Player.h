#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

struct PlayerState {
    int current, previous;

    void changeState(int future) {
        previous = current;
        current = future;
    }
};

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

class Player
{
public:
    Player();
    ~Player();
    Vector2 getPosition();
    Vector2 getVelocity();
    int getInputAxis();
    int checkCollisionPointRecArr(Vector2 point, Rectangle* recs, int len);
    int checkCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len);
    void groundCollision(Rectangle floorRec);
    void groundCollision(vector<Rectangle> floorRec);
    void ceilingCollision(vector<Rectangle> ceilingRec, int len);
    void wallCollision(vector<Rectangle> wallRec);
    void update();
    //void lateUpdate();
    //void earlyUpdate();

    void moveH();
    void moveV();
    int getNormalizedVelocity();
    void increaseHalfOfVelocity();
    void increaseHalfOfGravity();
    void updateColliderPosiotions();
    void drawPlayer();

private:
    enum playerStates
    {
        IDLE, WALK, JUMP, FALL, ATTACK, STUN, STAIRS, KNOCKBACK, DIE
    };

    float jumpForce = 250;
    float grav = 1100;
    float halfGrav = 0.5f * grav;
    float acc = 300;
    float dec = 200;
    float airDec = 50;
    Vector2 size = { 16, 32 };
    Vector2 position = { 50, 100 };
    Vector2 velocity = { 0, 0 };
    float maxSPD = 80;
    float minSPD = 0.5f;
    float maxFALL = 400;
    int normalizedVelocity = 0;
    bool isOnFloor = true;
    bool wasOnFloor = true;
    bool isOnCeiling = true;

    bool leftBlocked = false;
    bool rightBlocked = false;

    bool jumpAllowed = false;

    const char* imageName = "HOUSE_MD.png";
    Texture2D sprite;

    Rectangle groundCollider = { 0, 0, size.x, 4 };
    Rectangle topCollider = { 0, 0, size.x, 2 };
    Rectangle leftCollider = { 0, 0, 1, size.y };
    Rectangle rightCollider = { 0, 0, 1, size.y };

    PlayerState upperState = { 0, 0 };
    PlayerState lowerState = { 0, 0 };

    Timer attackTimer{1};
};