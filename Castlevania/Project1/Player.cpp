#include "Player.h"
#include "GameManager.h"

Player::Player() {
    sprite = LoadTexture(imageName);
    size = { (float) GameManager::getInstance().getActiveScene()->getTileWidth(), (float) GameManager::getInstance().getActiveScene()->getTileHeight() *2};
    position = { 0, 100 };
    velocity = { 0, 0 };
    worldHeight = (float)GameManager::getInstance().getActiveScene()->getWorldHeight();
    worldWidth = (float)GameManager::getInstance().getActiveScene()->getWorldWidth();

    groundCollider = { 0, 0, size.x, 4 };
    topCollider = { 0, 0, size.x, 2 };
    leftCollider = { 0, 0, 1, size.y };
    rightCollider = { 0, 0, 1, size.y };

    health = &GameManager::getInstance().playerHealth;
    whipLevel = &GameManager::getInstance().whipLevel;
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
    leftCollider.y = position.y + 5;
    rightCollider.x = position.x + size.x;
    rightCollider.y = position.y + 5;

    leftCollider.height = size.y - 10;
    rightCollider.height = size.y - 10;

    whipCollider.x = (position.x - 8) + 24 * direction;
    whipCollider.y = position.y;
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

void Player::ceilingCollision(vector<Rectangle> ceilingRec) {
    int len = ceilingRec.size();
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

void Player::moveH(bool accelerate) {
    normalizedVelocity = getNormalizedVelocity();

    //Limit velocity
    if (abs(velocity.x) > maxSPD) {
        velocity.x = maxSPD * normalizedVelocity;
    }
    //Apply half of velocity
    else {
        increaseHalfOfVelocity(accelerate);
    }

    //Apply Horizontal Movement
    if ((rightBlocked && velocity.x > 0) || (leftBlocked && velocity.x < 0)) {
        velocity.x = 0;
    }
    else {
        position.x += velocity.x * GetFrameTime();
    }

    //Limit velocity
    if (abs(velocity.x) > maxSPD) {
        velocity.x = maxSPD * normalizedVelocity;
    }
    //Apply half of velocity
    else {
        increaseHalfOfVelocity(accelerate);
    }

    //Prevent Flickering
    if (abs(velocity.x) < minSPD) {
        velocity.x = 0;
    }

    //Clamp position to World Size TODO: Add world size to Game Manager
    position.x = Clamp(position.x, 0, worldWidth - size.x);
}

void Player::moveV() {
    //If Space is down, gravity is halved (Variable Jump height)
    grav = IsKeyDown(KEY_SPACE) ? halfGrav : halfGrav * 2;

    Entity::moveV();
}

void Player::increaseHalfOfVelocity(bool accelerate) {
    if (accelerate) velocity.x += getInputAxis() * GetFrameTime() * acc * 0.5f;
    float _dec = dec;
    if (!isOnFloor) {
        _dec = airDec;
    }
    velocity.x -= _dec * GetFrameTime() * normalizedVelocity * 0.5f;
}

int someCounter = 0;

void Player::update() {

    GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(upperState.current), 1);
   // GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(lowerState.current), 2);

    //earlyUpdate(); // For things that need to be done before everything else
    if (isOnFloor && lowerState.current != JUMP) { // TODO: When frame buffer is implemented make it so that if the frame buffer is true, jump can be allowed from JUMP
        jumpAllowed = true;
    }

    //Lower Body State Machine
    switch (lowerState.current) {

    case IDLE:
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (getInputAxis()) lowerState.changeState(WALK);
        break;

    case WALK:
        moveH(true);
        updateDirection();
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

        moveH(true);
        updateDirection();
        //Transition
        if (velocity.y >= 0) lowerState.changeState(FALL);
        if (isOnFloor && !wasOnFloor) {
            if (abs(velocity.x) >= minSPD && getInputAxis()) lowerState.changeState(WALK);
            else lowerState.changeState(IDLE);
        }
        break;

    case FALL:
        moveH(true);
        moveV();
        updateDirection();
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
        moveV();
        if (!(lowerState.previous == WALK && lowerState.previous == IDLE) && !isOnFloor) moveH(true);
        else moveH(false);
        //this should only happen if the player attacks from WALK, STAIRS or IDLE
        break;
    }

    //Upper body state machine (remember that upper body is subserviant to lower body)
    switch (upperState.current) {
    
    case IDLE:
        if (IsKeyPressed(KEY_D)) upperState.changeState(STARTATTACK);
        break;
    case STARTATTACK:
        startAttackTimer.updateTimer();
        if (startAttackTimer.isTriggerd()) upperState.changeState(ATTACK);
        break;
    case ATTACK:
        //Transition
        attackTimer.updateTimer();
        if (attackTimer.isTriggerd()) {
            upperState.changeState(IDLE);
            lowerState.changeState(IDLE);
        }
        break;
    }
    updateColliderPosiotions();
    //lateUpdate(); // For things that need to be done after everything else
}

void Player::drawPlayer() {
    Rectangle rec = { position.x, position.y, size.x, size.y };
    DrawRectangleRec(rec, DARKGREEN);
    DrawTextureV(sprite, position, WHITE);
    DrawRectangleRec(groundCollider, RED);
    DrawRectangleRec(topCollider, RED);
    DrawRectangleRec(leftCollider, RED);
    DrawRectangleRec(rightCollider, RED);

    if (upperState.current == STARTATTACK) DrawRectangle(position.x- 16*direction, position.y, 16, 30, WHITE);
    if (upperState.current == ATTACK) DrawRectangle((position.x - 8) + 24 * direction, position.y, whipCollider.width, 16, WHITE);
}

void Player::betweenStates(int previous, int current, int future, PlayerState* state) {
    if (state == &upperState) {
        if (current == IDLE && future == STARTATTACK) {
            whipCollider.width = GameManager::getInstance().whipLevel < 2 ? 32 : 48;
            GameManager::getInstance().getActiveScene()->pushPlayerHitBoxes(&whipCollider);
            startAttackTimer.startTimer();
            lowerState.changeState(ATTACK);
        }
        else if (current == STARTATTACK && future == ATTACK) {
            attackTimer.startTimer();
        }
        else if (current == ATTACK && future == IDLE) {
            GameManager::getInstance().getActiveScene()->removePlayerHitBoxes(&whipCollider);
        }
    }
    
}

void Player::updateDirection() {
    if (getInputAxis() != 0) direction = getInputAxis();
}