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
        position.y = floorRec.y - size;
        if (velocity.y > 0) {
            velocity.y = 0;
        }
    }
    else {
        isOnFloor = false;
    }
}

void Player::GroundCollision(vector<Rectangle> floorRec, int len) {
    wasOnFloor = isOnFloor;
    int i = CheckCollisionRecsArr(groundCollider, floorRec, len);
    if (i != -1) {
        isOnFloor = true;
        position.y = floorRec[i].y - size;
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

void Player::WallCollision(vector<Rectangle> wallRec, int len) {
    int i = CheckCollisionRecsArr(leftCollider, wallRec, len);
    if (i != -1) {
        leftBlocked = true;
        position.x += GetFrameTime() * 80;
        if (velocity.x < 0) {
            velocity.x = 0;
        }
    }
    else {
        leftBlocked = false;
    }
    int j = CheckCollisionRecsArr(rightCollider, wallRec, len);
    if (j != -1) {
        rightBlocked = true;
        position.x -= GetFrameTime() * 80;
        if (velocity.x > 0) {
            velocity.x = 0;
        }
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
        grav = 1;
    }
    else {
        grav = 2;
    }


    if (abs(velocity.x) < 0.1) {
        velocity.x = 0;
    }


    position = Vector2Add(position, velocity);
    position.x = Clamp(position.x, 0, GetScreenWidth() - size);
    position.y = Clamp(position.y, 0, GetScreenHeight());
    groundCollider.x = position.x + 8 - GetInpuAxis() * 8;
    groundCollider.y = position.y + size - 1;
    topCollider.x = position.x + 8;
    topCollider.y = position.y + 4;
    leftCollider.x = position.x - 1;
    leftCollider.y = position.y + 10;
    rightCollider.x = position.x + size - 1;
    rightCollider.y = position.y + 10;

    if (velocity.y > 10) {
        leftCollider.height = size - 40;
        rightCollider.height = size - 40;
    }
    else {
        leftCollider.height = size - 20;
        rightCollider.height = size - 20;
    }
}

void Player::DrawPlayer() {
    rec = { position.x, position.y, size, size };
    DrawRectangleRec(rec, DARKGREEN);
    DrawTextureV(sprite, position, WHITE);
    //DrawRectangleRec(groundCollider, RED);
    //DrawRectangleRec(topCollider, RED);
    //DrawRectangleRec(leftCollider, RED);
    //DrawRectangleRec(rightCollider, RED);
}

