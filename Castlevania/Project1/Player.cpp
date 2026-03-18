#include "Player.h"
#include "Classes.h"

Player::Player() {
    sprite = LoadTexture(imageName);
}
Player::~Player() {
    UnloadTexture(sprite);
}

Vector2 Player::getPosition() { return position; }
Vector2 Player::getVelocity() { return velocity; }

int Player::getInputAxis() { return IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT); }

int Player::getNormalizedVelocity() {
    if (velocity.x > 0) {
        return 1;
    }
    if (velocity.x < 0) {
        return -1;
    }
    return 0;
}

void Player::updateColliderPosiotions() {
    groundCollider.x = position.x;
    groundCollider.y = position.y + size.y - 1;

    topCollider.x = position.x;
    topCollider.y = position.y;

    leftCollider.x = position.x - 1;
    leftCollider.y = position.y + 10;
    rightCollider.x = position.x + size.x;
    rightCollider.y = position.y + 10;

    leftCollider.height = size.y - 20;
    rightCollider.height = size.y - 20;
}

int Player::checkCollisionPointRecArr(Vector2 point, Rectangle* recs, int len) {
    for (int i = 0; i < len; i++) {
        if (CheckCollisionPointRec(point, recs[i])) return i;
    }
    return -1;
}

int Player::checkCollisionRecsArr(Rectangle rec1, vector<Rectangle> recs2, int len) {
    for (int i = 0; i < len; i++) {
        if (CheckCollisionRecs(rec1, recs2[i])) return i;
    }
    return -1;
}

void Player::groundCollision(Rectangle floorRec) {
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

void Player::groundCollision(vector<Rectangle> floorRec) {
    wasOnFloor = isOnFloor;
    int len = floorRec.size();
    Rectangle predictedRec = groundCollider;
    predictedRec.y += velocity.y * GetFrameTime();
    int i = checkCollisionRecsArr(predictedRec, floorRec, len);
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

void Player::ceilingCollision(vector<Rectangle> ceilingRec, int len) {
    int i = checkCollisionRecsArr(topCollider, ceilingRec, len);
    if (i != -1 && velocity.y < 0) {
        velocity.y = 0;
    }
}

void Player::wallCollision(vector<Rectangle> wallRec) {
    int len = wallRec.size();
    Rectangle predictedRec = leftCollider;
    predictedRec.x += velocity.x * GetFrameTime();

    int i = checkCollisionRecsArr(predictedRec, wallRec, len);
    if (i != -1) {
        leftBlocked = true;
        if (velocity.x < 0) position.x = wallRec[i].x + wallRec[i].width;
    }
    else {
        leftBlocked = false;
    }

    predictedRec = rightCollider;
    predictedRec.x += velocity.x * GetFrameTime();

    int j = checkCollisionRecsArr(predictedRec, wallRec, len);
    if (j != -1) {
        rightBlocked = true;
        if (velocity.x > 0) position.x = wallRec[j].x - size.x;
    }
    else {
        rightBlocked = false;
    }
}

void Player::moveH() {
    normalizedVelocity = getNormalizedVelocity();

    //Limit velocity
    if (abs(velocity.x) > maxSPD) {
        velocity.x = maxSPD * normalizedVelocity;
    }
    //Apply half of velocity
    else {
        increaseHalfOfVelocity();
    }

    //Apply Horizontal Movement
    if ((rightBlocked && velocity.x > 0) || (leftBlocked && velocity.x < 0)) {
        velocity.x = 0;
    }
    else {
        position.x += velocity.x * GetFrameTime();
    }

    //Apply other half of velocity
    if (abs(velocity.x) <= maxSPD && abs(velocity.x) >= minSPD) {
        increaseHalfOfVelocity();
    }

    //Prevent Flickering
    if (abs(velocity.x) < minSPD) {
        velocity.x = 0;
    }

    //Clamp position to World Size TODO: Add world size to Game Manager
    position.x = Clamp(position.x, 0, 256 - size.x);
}

void Player::moveV() {
    //If Space is down, gravity is halved (Variable Jump height)
    grav = IsKeyDown(KEY_SPACE) ? halfGrav : halfGrav * 2;

    //Apply half of gravity
    increaseHalfOfGravity();

    //Apply Vertical Movement
    position.y += velocity.y * GetFrameTime();

    //Apply other half of gravity
    increaseHalfOfGravity();

    //Clamp position to World Size TODO: Add world size to Game Manager
    position.y = Clamp(position.y, 0, 240);
}

void Player::increaseHalfOfGravity() {
    if (!isOnFloor && velocity.y < maxFALL) {
        velocity.y += grav * GetFrameTime() * 0.5f;
    }
}

void Player::increaseHalfOfVelocity() {
    velocity.x += getInputAxis() * GetFrameTime() * acc * 0.5f;
    float _dec = dec;
    if (!isOnFloor) {
        _dec = airDec;
    }
    velocity.x -= _dec * GetFrameTime() * normalizedVelocity * 0.5f;
}

int someCounter = 0;

void Player::update() {
    earlyUpdate(); // For things that need to be done before everything else
    if (isOnFloor && lowerState.current != JUMP) { // TODO: When frame buffer is implemented make it so that if the frame buffer is true, jump can be allowed from JUMP
        jumpAllowed = true;
    }

    DrawText(to_string(velocity.x).c_str(), 0, 0, 50, WHITE);
    DrawText(to_string(lowerState.current).c_str(), 0, 50, 50, WHITE);
    //Lower Body State Machine
    switch (lowerState.current) {

    case IDLE:
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (getInputAxis()) lowerState.changeState(WALK);
        break;

    case WALK:
        moveH();

        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (abs(velocity.x) < minSPD && !getInputAxis()) lowerState.changeState(IDLE);
        break;

    case JUMP:
        
        if (jumpAllowed) 
        {
            someCounter++;
            velocity.y -= jumpForce;
            jumpAllowed = false;
        }
        moveV();

        moveH();

        //Transition
        if (velocity.y >= 0) lowerState.changeState(FALL);
        if (isOnFloor && !wasOnFloor) {
            if (abs(velocity.x) >= minSPD && getInputAxis()) lowerState.changeState(WALK);
            else lowerState.changeState(IDLE);
        }
        break;

    case FALL:
        moveH();
        moveV();

        //Transition
        if (isOnFloor) {
            if (abs(velocity.x) >= minSPD || getInputAxis()) lowerState.changeState(WALK);
            else lowerState.changeState(IDLE);
        }
        break;

    case STAIRS:
        break;
    case DIE:
        break;
    case KNOCKBACK:
        break;
    case STUN:
        break;

    case ATTACK:
        //this should only happen if the player attacks from WALK, STAIRS or IDLE
        velocity.x = 0;
        break;
    }

    //Upper body state machine (remember that upper body is subserviant to lower body)
    switch (upperState.current) {
    
    case IDLE:
        break;
    case ATTACK:
        break;
    }
    updateColliderPosiotions();
    lateUpdate(); // For things that need to be done after everything else
}

void Player::drawPlayer() {
    Rectangle rec = { position.x, position.y, size.x, size.y };
    DrawRectangleRec(rec, DARKGREEN);
    DrawTextureV(sprite, position, WHITE);
    DrawRectangleRec(groundCollider, RED);
    DrawRectangleRec(topCollider, RED);
    DrawRectangleRec(leftCollider, RED);
    DrawRectangleRec(rightCollider, RED);
}

