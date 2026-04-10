#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Classes.h"
#include "Entity.h"
#include <vector>
#include <iostream>
#include <string>
#include "SpriteRenderer.h"

using namespace std;

class Player : public Entity
{
private:
    Timer startAttackTimer{ 0.25 };
    Timer attackTimer{ 0.25 };
    Timer stunTimer{ 0.5 };

    enum playerStates
    {
        IDLE, WALK, JUMP, FALL, ATTACK, STARTATTACK, CROUCH, STUN, STAIRS, KNOCKBACK, DIE
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
    float acc = 200;
    float dec = 350;
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
    
    float stunHeight = 100;
    float maxHeight = 256;
    float floorHeight = 256;

    float bottomAnimOffsetY;
    float bottomAnimOffsetX;
    float topAnimOffsetY;
    float topAnimOffsetX;
    short isOnStair; // 0 = no stair; 1 = stair up start; 2 = stair up end; -1 = stair down start; -2 = stair down end;
    short lockStair; // 0 = no stair; 1 = stair up start; 2 = stair up end; -1 = stair down start; -2 = stair down end;
    float stairPos;
    SpriteRenderer* topSprite = nullptr;
    SpriteRenderer* bottomSprite = nullptr;
    SpriteRenderer* whipSprite = nullptr;

    Rectangle hurtbox;

    Rectangle groundCollider;
    Rectangle topCollider;
    Rectangle leftCollider;
    Rectangle rightCollider;

    Rectangle whipCollider = {0, 0, 32, 16};

    PlayerState upperState{*this};
    PlayerState lowerState{*this};

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
    void stairCollision(vector<staircase>& stairs);
    void update();
    void lateUpdate() override;
    void earlyUpdate() override;

    void moveH(bool accelerate, bool decelerate);
    void moveV() override;
    int getNormalizedVelocity();
    void increaseHalfOfVelocity(bool accelerate, bool decelerate);
    void updateColliderPosiotions() override;
    void drawPlayer();
    void updateDirection();
    void updateAnimation();

    void betweenStates(int previous, int current, int future, PlayerState* state);
};