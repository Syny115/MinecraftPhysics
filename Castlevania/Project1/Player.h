#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Classes.h"
#include "Entity.h"
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

class Player : public Entity
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
    void ceilingCollision(vector<Rectangle> ceilingRec);
    void wallCollision(vector<Rectangle> wallRec);
    void update();
    //void lateUpdate();
    //void earlyUpdate();

    void moveH();
    void moveV() override;
    int getNormalizedVelocity();
    void increaseHalfOfVelocity();
    void updateColliderPosiotions();
    void drawPlayer();

private:
    enum playerStates
    {
        IDLE, WALK, JUMP, FALL, ATTACK, STUN, STAIRS, KNOCKBACK, DIE
    };

    float jumpForce = 240;
    float halfGrav = 0.5f * grav;
    float acc = 300;
    float dec = 200;
    float airDec = 50;
    float maxSPD = 58.85;
    float minSPD = 5;
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

    Rectangle groundCollider;
    Rectangle topCollider;
    Rectangle leftCollider;
    Rectangle rightCollider;

    PlayerState upperState = { 0, 0 };
    PlayerState lowerState = { 0, 0 };

    Timer attackTimer{1};
};