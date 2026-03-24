#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Classes.h"
#include "Entity.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class Player : public Entity
{
private:
    Timer startAttackTimer{ 0.25 };
    Timer attackTimer{ 0.25 };

    enum playerStates
    {
        IDLE, WALK, JUMP, FALL, ATTACK, STARTATTACK, STUN, STAIRS, KNOCKBACK, DIE
    };

    struct PlayerState {
        Player& owner;
        int current = 0; int previous = 0;
        PlayerState(Player& p) : owner(p) {}
        void changeState(int future) {
            owner.betweenStates(previous, current, future, this);
            previous = current;
            current = future;
        }
    };

    int* health;
    int* whipLevel;
    int* subWeapon;
    int* score;

    float jumpForce = 230;
    float halfGrav = 0.5f * grav;
    float acc = 300;
    float dec = 200;
    float airDec = 50;
    float maxSPD = 58.85;
    float minSPD = 5;
    float maxFALL = 400;
    int normalizedVelocity = 0;
    int direction = 1;
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

    Rectangle whipCollider;

    PlayerState upperState{*this};
    PlayerState lowerState{*this};

    Sound soundArray[10];

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

    void moveH(bool accelerate);
    void moveV() override;
    int getNormalizedVelocity();
    void increaseHalfOfVelocity(bool accelerate);
    void updateColliderPosiotions();
    void drawPlayer();
    void updateDirection();

    void betweenStates(int previous, int current, int future, PlayerState* state);
};