#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>
using namespace std;

class Player
{
public:
	Player();
	~Player();
    Vector2 GetPosition();
    Vector2 GetVelocity();
    int GetInpuAxis();
    int CheckCollisionPointRecArr(Vector2 point, Rectangle* recs, int len);
    int CheckCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len);
    void GroundCollision(Rectangle floorRec);
    void GroundCollision(vector<Rectangle> floorRec);
    void CeilingCollision(vector<Rectangle> ceilingRec, int len);
    void WallCollision(vector<Rectangle> wallRec);
    void Update();
    void DrawPlayer();

private:
    float jumpForce =           5;
    float grav =                0.5f;
    float halfGrav =            0.5f * grav;
    float acc =                 10;
    float dec =                 5;
    float airDec =              3;
    Vector2 size =              { 16, 32 };
    Vector2 position =          { 50, 60 };
    Vector2 velocity =          { 0, 0 };
    float maxSPD =              1.2;
    float maxFALL =             20;
    int normalizedVelocity =    0;
    bool isOnFloor =            true;
    bool wasOnFloor =           true;

    bool leftBlocked =          false;
    bool rightBlocked =         false;

    const char* imageName = "HOUSE_MD.png";
    Texture2D sprite;

    Rectangle groundCollider =  { 0, 0, size.x, 4 };
    Rectangle topCollider =     { 0, 0, size.x, 2 };
    Rectangle leftCollider =    { 0, 0, 1, size.y };
    Rectangle rightCollider =   { 0, 0, 1, size.y };
};