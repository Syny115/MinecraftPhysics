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
    groundCollider.x = offsetX;
    groundCollider.y = offsetY + size.y - 1;

    topCollider.x = offsetX;
    topCollider.y = offsetY;

    leftCollider.x = offsetX - 1;
    leftCollider.y = offsetY + 5;
    rightCollider.x = offsetX + size.x;
    rightCollider.y = offsetY + 5;

    leftCollider.height = size.y - 10;
    rightCollider.height = size.y - 10;

    whipCollider.x = position.x -whipCollider.width/2 + (whipCollider.width + size.x) * 0.5 * direction;
    

    if (lowerState.current == STUN || lowerState.current == CROUCH) {
        hurtbox = Rectangle{ offsetX, position.y, size.x , size.y / 2 };
        whipCollider.y = position.y;
    }
    else {
        hurtbox = Rectangle{ offsetX, offsetY, size.x, size.y };
        whipCollider.y = offsetY;
    }
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
        position.y = floorRec.y - size.y/2;
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
        position.y = floorRec[i].y - size.y/2;
        floorHeight = position.y;
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
        if (velocity.x < 0) position.x = wallRec[i].x + wallRec[i].width + size.x/2;
    }
    else {
        leftBlocked = false;
    }

    predictedRec = rightCollider;
    predictedRec.x += velocity.x * GetFrameTime();

    int j = checkCollisionRecsArr(predictedRec, wallRec, len);
    if (j != -1) {
        rightBlocked = true;
        if (velocity.x > 0) position.x = wallRec[j].x - size.x/2;
    }
    else {
        rightBlocked = false;
    }
}

void Player::moveH(bool accelerate, bool decelerate) {
    normalizedVelocity = getNormalizedVelocity();
    //Limit velocity
    if (abs(velocity.x) > maxSPD) {
        velocity.x = maxSPD * normalizedVelocity;
    }
    //Apply half of velocity
    else {
        increaseHalfOfVelocity(accelerate, decelerate);
        normalizedVelocity = getNormalizedVelocity();
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
        increaseHalfOfVelocity(accelerate, decelerate);
        normalizedVelocity = getNormalizedVelocity();
    }

    //Prevent Flickering
    if (abs(velocity.x) <= minSPD && normalizedVelocity * getInputAxis() <= 0) {
        velocity.x = 0;
    }

    //Clamp position to World Size TODO: Add world size to Game Manager
    position.x = Clamp(position.x, +size.x/2, worldWidth - size.x/2);
    offsetY = position.y - size.y / 2;
    offsetX = position.x - size.x / 2;
}

void Player::moveV() {
    //If Space is down, gravity is halved (Variable Jump height)
    grav = IsKeyDown(KEY_SPACE) ? halfGrav : halfGrav * 2;

    Entity::moveV();
}

void Player::increaseHalfOfVelocity(bool accelerate, bool decelerate) {
    if (accelerate) velocity.x += getInputAxis() * GetFrameTime() * acc * 0.5f;
    if (!decelerate) return;
    float _dec = dec;
    if (!isOnFloor) {
        _dec = airDec;
    }
    
    if ((normalizedVelocity * getInputAxis() <= 0 && abs(velocity.x) > minSPD) || !accelerate) velocity.x -= _dec * GetFrameTime() * normalizedVelocity * 0.5f;
}

int someCounter = 0;

void Player::earlyUpdate() {

}

void Player::update() {
    earlyUpdate(); // For things that need to be done before everything else

    
    if (isOnFloor && lowerState.current != JUMP) { // TODO: When frame buffer is implemented make it so that if the frame buffer is true, jump can be allowed from JUMP
        jumpAllowed = true;
    }

    //Lower Body State Machine
    switch (lowerState.current) {

    case IDLE:
        velocity.y = 0;
        moveH(false, true);
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (getInputAxis()) lowerState.changeState(WALK);
        if (IsKeyDown(KEY_DOWN)) lowerState.changeState(CROUCH);
        break;

    case WALK:
        moveH(true, true);
        updateDirection();
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (IsKeyDown(KEY_DOWN)) lowerState.changeState(CROUCH);
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

        moveH(true, true);
        updateDirection();
        //Transition
        if (velocity.y >= 0) lowerState.changeState(FALL);
        if (isOnFloor && !wasOnFloor) {
            if (abs(velocity.x) >= minSPD && getInputAxis()) lowerState.changeState(WALK);
            else lowerState.changeState(IDLE);
        }
        break;

    case FALL:
        moveH(true, true);
        moveV();
        updateDirection();
        if (position.y < maxHeight) maxHeight = position.y;
        //Transition
        if (isOnFloor) {
            if (floorHeight-maxHeight >= stunHeight) upperState.changeState(STUN);
            else if (abs(velocity.x) >= minSPD || getInputAxis()) lowerState.changeState(WALK);
            else lowerState.changeState(IDLE);
        }
        break;

    case STAIRS:
        break;
    case DIE:
        break;
    case KNOCKBACK:
        break;

    case ATTACK:
        moveV();
        if (!(lowerState.previous == WALK && lowerState.previous == IDLE) && !isOnFloor) moveH(false, false);
        else moveH(false, true);
        //this should only happen if the player attacks from WALK, STAIRS or IDLE
        break;

    case CROUCH:
        moveH(false, true);
        if (IsKeyUp(KEY_DOWN) && !attackTimer.isActive() && !stunTimer.isActive()) lowerState.changeState(IDLE);
    }

    //Upper body state machine
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
            if (lowerState.current != CROUCH) lowerState.changeState(IDLE);
        }
        break;
    case STUN:
        stunTimer.updateTimer();
        if (stunTimer.isTriggerd()) upperState.changeState(IDLE);
    }
    
   
    lateUpdate(); // For things that need to be done after everything else
}

void Player::lateUpdate() {
    updateColliderPosiotions();
    GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(upperState.current), 1);
    GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(lowerState.current), 2);
    
    if (IsKeyPressed(KEY_A)) position.y = 0;
}

void Player::drawPlayer() {
    DrawRectangleRec(hurtbox, DARKGREEN);
    DrawTextureV(sprite, position, WHITE);
    DrawRectangleRec(groundCollider, RED);
    DrawRectangleRec(topCollider, RED);
    DrawRectangleRec(leftCollider, RED);
    DrawRectangleRec(rightCollider, RED);
    if (upperState.current == STARTATTACK) DrawRectangle(offsetX- 16*direction, offsetY, 16, 30, WHITE);
    if (upperState.current == ATTACK) DrawRectangleRec(whipCollider, WHITE);
}

void Player::betweenStates(int previous, int current, int future, PlayerState* state) {
    if (state == &upperState) {
        if (current == IDLE && future == STARTATTACK) {
            whipCollider.width = GameManager::getInstance().whipLevel < 2 ? 32 : 48;
            GameManager::getInstance().getActiveScene()->pushPlayerHitBoxes(&whipCollider);
            startAttackTimer.startTimer();
            if (lowerState.current != CROUCH) lowerState.changeState(ATTACK);
        }
        else if (current == STARTATTACK && future == ATTACK) {
            GameManager::getInstance().getGamePointer()->publicPlaySound(0);
            attackTimer.startTimer();
        }
        else if (current == ATTACK && future == IDLE) {
            GameManager::getInstance().getActiveScene()->removePlayerHitBoxes(&whipCollider);
        }
        else if (future == STUN) {
            stunTimer.startTimer();
            lowerState.changeState(CROUCH);
        }
    }
    else {
        if (current == FALL) maxHeight = 256;
    }
    
}

void Player::updateDirection() {
    if (getInputAxis() != 0) direction = getInputAxis();
}