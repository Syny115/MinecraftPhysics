#include "Player.h"

Player::Player() {
    sprite = LoadTexture(imageName);
}
Player::~Player() {
    UnloadTexture(sprite);
}

Vector2 Player::GetPosition() { return position; }
Vector2 Player::GetVelocity() { return velocity; }

int Player::GetInpuAxis() { return IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT); }

int Player::CheckCollisionPointRecArr(Vector2 point, Rectangle* recs, int len) {
    for (int i = 0; i < len; i++) {
        if (CheckCollisionPointRec(point, recs[i])) return i;
    }
    return -1;
}

int Player::CheckCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len) {
    for (int i = 0; i < len; i++) {
        if (CheckCollisionRecs(rec1, recs2[i])) return i;
    }
    return -1;
}

void Player::GroundCollision(Rectangle floorRec) {
    wasOnFloor = isOnFloor;
    if (CheckCollisionRecs(groundCollider, floorRec)) {
        isOnFloor = true;
        position.y = floorRec.y - size.y;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else {
        isOnFloor = false;
    }
}

void Player::GroundCollision(vector<Rectangle> floorRec) {
    wasOnFloor = isOnFloor;
    int len = floorRec.size();
    Rectangle predictedRec = groundCollider;
    predictedRec.y += velocity.y;
    int i = CheckCollisionRecsArr(predictedRec, floorRec, len);
    if (i != -1) {
        isOnFloor = true;
        position.y = floorRec[i].y - size.y;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else {
        isOnFloor = false;
    }
}

void Player::CeilingCollision(vector<Rectangle> ceilingRec, int len) {
    int i = CheckCollisionRecsArr(topCollider, ceilingRec, len);
    if (i != -1 && velocity.y < 0) {
        velocity.y = 0;
    }
}

void Player::WallCollision(vector<Rectangle> wallRec) {
    int len = wallRec.size();
    Rectangle predictedRec = leftCollider;
    predictedRec.x += velocity.x;

    int i = CheckCollisionRecsArr(predictedRec, wallRec, len);
    if (i != -1) {
        leftBlocked = true;
        if (velocity.x < 0) position.x = wallRec[i].x + wallRec[i].width;
    }
    else {
        leftBlocked = false;
    }

    predictedRec = rightCollider;
    predictedRec.x += velocity.x;

    int j = CheckCollisionRecsArr(predictedRec, wallRec, len);
    if (j != -1) {
        rightBlocked = true;
        if (velocity.x > 0) position.x = wallRec[j].x - size.x;
    }
    else {
        rightBlocked = false;
    }
}

void Player::Update() {
    if (velocity.x > 0) {
        normalizedVelocity = 1;
    }
    else if (velocity.x < 0) {
        normalizedVelocity = -1;
    }
    else {
        normalizedVelocity = 0;
    }

    if (abs(velocity.x) > maxSPD) {
        velocity.x = maxSPD * normalizedVelocity;
    }
    else {
        velocity.x += GetInpuAxis() * GetFrameTime() * acc;
        float _dec = dec;
        if (!isOnFloor) {
            _dec = airDec;
        }
        velocity.x -= _dec * GetFrameTime() * normalizedVelocity;
    }


    
    if (!isOnFloor && velocity.y < maxFALL) {
        velocity.y += grav;
    }
    if (wasOnFloor) {
        if (IsKeyPressed(KEY_SPACE)) velocity.y -= jumpForce;
    }
    if (IsKeyDown(KEY_SPACE)) {
        grav = halfGrav;
    }
    else {
        grav = halfGrav*2;
    }


    if (abs(velocity.x) < 0.1) {
        velocity.x = 0;
    }

    if ((rightBlocked && velocity.x > 0) || (leftBlocked && velocity.x < 0)) {
        velocity.x = 0;
    }
    else {
        position.x += velocity.x;
    }
    position.y += velocity.y;

    position.x = Clamp(position.x, 0, 256 - size.x);
    position.y = Clamp(position.y, 0, 240);

    groundCollider.x = position.x;
    groundCollider.y = position.y + size.y- 1;

    topCollider.x = position.x;
    topCollider.y = position.y;

    leftCollider.x = position.x -1;
    leftCollider.y = position.y + 10;
    rightCollider.x = position.x + size.x;
    rightCollider.y = position.y + 10;

        leftCollider.height = size.y - 20;
        rightCollider.height = size.y - 20;
}

void Player::DrawPlayer() {
    Rectangle rec = { position.x, position.y, size.x, size.y };
    DrawRectangleRec(rec, DARKGREEN);
    DrawTextureV(sprite, position, WHITE);
    DrawRectangleRec(groundCollider, RED);
    DrawRectangleRec(topCollider, RED);
    DrawRectangleRec(leftCollider, RED);
    DrawRectangleRec(rightCollider, RED);
}

