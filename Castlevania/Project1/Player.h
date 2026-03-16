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
    void GroundCollision(vector<Rectangle> floorRec, int len);
    void CeilingCollision(vector<Rectangle> ceilingRec, int len);
    void WallCollision(vector<Rectangle> wallRec, int len);
    void Update();
    void DrawPlayer();

private:
    float jumpForce = 20;
    float grav = 1.5f;
    float acc = 20;
    float dec = 15;
    float airDec = 10;
    float size = 70;
    Rectangle rec;
    Vector2 position = { 50, 60 };
    Vector2 velocity = { 0, 0 };
    float maxSPD = 10;
    float maxFALL = 20;
    int normalizedVelocity = 0;
    bool isOnFloor = true;
    bool wasOnFloor = true;

    bool leftBlocked = false;
    bool rightBlocked = false;

    const char* imageName = "HOUSE_MD.png";
    Texture2D sprite;

    Rectangle groundCollider = { 8, size - 1, size - 16, 4 };
    Rectangle topCollider = { 8, +4, size - 16, 2 };
    Rectangle leftCollider = { 0 + 2, 10, 4, size - 40 };
    Rectangle rightCollider = { size - 4, 10, 4, size - 40 };
};