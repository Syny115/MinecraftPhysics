#include "Player.h"
#include "GameManager.h"

Player::Player(Vector2 pos) {
    topSprite = new SpriteRenderer("resources/sprites/simon_sprites.png", SpriteRenderer::PLAYER_TOP);
    topSprite->setAnimation("idle");
    bottomSprite = new SpriteRenderer("resources/sprites/simon_sprites.png", SpriteRenderer::PLAYER_BOTTOM);
    bottomSprite->setAnimation("idle");
    whipSprite = new SpriteRenderer("resources/sprites/simon_sprites.png", SpriteRenderer::WHIP);
    whipSprite->setAnimation("hidden");

    size = { (float) GameManager::getInstance().getActiveScene()->getTileWidth(), (float) GameManager::getInstance().getActiveScene()->getTileHeight() *2};
    position = pos;
    velocity = { 0, 0 };
    worldHeight = (float)GameManager::getInstance().getActiveScene()->getWorldHeight();
    worldWidth = (float)GameManager::getInstance().getActiveScene()->getWorldWidth();

    groundCollider = { 0, 0, size.x, 4 };
    topCollider = { 0, 0, size.x, 2 };
    leftCollider = { 0, 0, 1, size.y };
    rightCollider = { 0, 0, 1, size.y };

    health = &GameManager::getInstance().playerHealth;
    whipLevel = &GameManager::getInstance().whipLevel;
    subWeapon = &GameManager::getInstance().subWeapon;
    ammo = &GameManager::getInstance().ammo;
    projectileUpgrade = &GameManager::getInstance().projectileUpgrade;
    projectileCount = &GameManager::getInstance().projectileCount;
}
Player::~Player() {
    delete topSprite;
    delete bottomSprite;
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
    else if (lowerState.current == JUMP || lowerState.current == FALL) {
        hurtbox = Rectangle{ offsetX, offsetY, size.x , size.y / 2 };
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



void Player::groundCollision(Rectangle floorRec) {
    wasOnFloor = isOnFloor;
    if (CheckCollisionRecs(groundCollider, floorRec)) {
        
        if (velocity.y > 0) {
            isOnFloor = true;
            position.y = floorRec.y - size.y / 2;
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
    predictedRec.y += velocity.y * deltaTime;
    int i = checkCollisionRecsArr(predictedRec, floorRec, len);
    if (i != -1) {
        
        if (velocity.y > 0) {
            velocity.y = 0;
            isOnFloor = true;
            if (lockStair == 0) position.y = floorRec[i].y - size.y / 2;
            floorHeight = position.y;
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
    predictedRec.x += velocity.x * deltaTime;

    int i = checkCollisionRecsArr(predictedRec, wallRec, len);
    if (i != -1) {
        leftBlocked = true;
        if (velocity.x < 0) position.x = wallRec[i].x + wallRec[i].width + size.x/2;
    }
    else {
        leftBlocked = false;
    }

    predictedRec = rightCollider;
    predictedRec.x += velocity.x * deltaTime;

    int j = checkCollisionRecsArr(predictedRec, wallRec, len);
    if (j != -1) {
        rightBlocked = true;
        if (velocity.x > 0) position.x = wallRec[j].x - size.x/2;
    }
    else {
        rightBlocked = false;
    }
}

void Player::stairCollision(vector<staircase>& stairs) { // 0 = no stair; 1 = stair up start; 2 = stair up end; -1 = stair down start; -2 = stair down end;
    for (int i = 0; i < stairs.size(); i++) {
        if (CheckCollisionRecs(stairs[i].start, groundCollider)) {
            isOnStair = 1;
            stairPos =  stairs[i].start.x + stairs[i].start.width / 2;
        }
        else if (CheckCollisionRecs(stairs[i].end, groundCollider)) {
            isOnStair = 2;
            stairPos = stairs[i].end.x + stairs[i].end.width / 2;
        }
        else isOnStair = 0;
        if (!stairs[i].up) isOnStair *= -1;
        if (isOnStair != 0) break;
    }
}

void Player::enemyCollision(vector<damageRect>& dmgRect) {
    for (int i = 0; i < dmgRect.size(); i++) {
        if (CheckCollisionRecs(*dmgRect[i].rect, hurtbox) && !invincibilityTimer.isActive()) {
            isDamaged = dmgRect[i].damage;
            invincibilityTimer.startTimer();
        }
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
        position.x += velocity.x * deltaTime;
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
    if (!GameManager::getInstance().bossStarted) position.x = Clamp(position.x, size.x/2, worldWidth - size.x/2);
    else  position.x = Clamp(position.x, size.x / 2 + worldWidth - 256, worldWidth - size.x / 2);
    offsetY = position.y - size.y / 2;
    offsetX = position.x - size.x / 2;
}

void Player::moveV() {
    //If Space is down, gravity is halved (Variable Jump height)
    grav = IsKeyDown(KEY_SPACE) ? halfGrav : halfGrav * 2;

    Entity::moveV();
    if (position.y >= worldHeight) { isDamaged = true; position = lastViablePos; }
}

void Player::increaseHalfOfVelocity(bool accelerate, bool decelerate) {
    if (accelerate) velocity.x += getInputAxis() * deltaTime * acc * 0.5f;
    if (!decelerate) return;
    float _dec = dec;
    if (!isOnFloor) {
        _dec = airDec;
    }
    
    if ((normalizedVelocity * getInputAxis() <= 0 && abs(velocity.x) > minSPD) || !accelerate) velocity.x -= _dec * deltaTime * normalizedVelocity * 0.5f;
    if (velocity.x * normalizedVelocity < 0) velocity.x = 0;
}

int someCounter = 0;

void Player::earlyUpdate() {
    Entity::earlyUpdate();
    if (isOnFloor) lastViablePos = position;
}

void Player::update() {
    earlyUpdate(); // For things that need to be done before everything else

    

    if (*subWeapon == GameManager::STOPWATCH) projCost = 5;
    else if (*subWeapon == GameManager::HOLYWATER) projCost = 2;
    else projCost = 1;

    invincibilityTimer.updateTimer(deltaTime);
    if (isOnFloor && lowerState.current != JUMP && lowerState.current != KNOCKBACK) { // TODO: When frame buffer is implemented make it so that if the frame buffer is true, jump can be allowed from JUMP
        jumpAllowed = true;
    }
    if (IsKeyPressed(KEY_F6)) isDamaged = 6;
    if (isDamaged != 0 && lowerState.current != DIE) 
    {
        attackTimer.stopTimer();
        startAttackTimer.stopTimer();
        lowerState.changeState(KNOCKBACK);
    }

    //Lower Body State Machine
    switch (lowerState.current) {

    case IDLE:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        bottomSprite->setAnimation("idle");
        velocity.y = 0;
        moveH(false, true);
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (getInputAxis()) lowerState.changeState(WALK);
        if (IsKeyDown(KEY_DOWN)) {
            if (isOnStair == -1 || isOnStair == 1) lowerState.changeState(STAIRS);
            else lowerState.changeState(CROUCH);
        }
        if (IsKeyDown(KEY_UP) && (isOnStair == -2 || isOnStair == 2)) lowerState.changeState(STAIRS);
        break;

    case WALK:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        velocity.y = 0;
        bottomSprite->setAnimation("walk");
        moveH(true, true);
        updateDirection();
        //Transition
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        else if (!isOnFloor) lowerState.changeState(FALL);
        else if (IsKeyDown(KEY_DOWN)) {
            if (isOnStair == -1 || isOnStair == 1) lowerState.changeState(STAIRS);
            else lowerState.changeState(CROUCH);
        }
        else if (IsKeyDown(KEY_UP) && (isOnStair == -2 || isOnStair == 2)) lowerState.changeState(STAIRS);
        else if (abs(velocity.x) < minSPD && !getInputAxis()) lowerState.changeState(IDLE);
        break;

    case JUMP:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        bottomSprite->setAnimation("crouch");
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
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        bottomSprite->setAnimation("crouch");
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
        if (IsKeyPressed(KEY_SPACE)) lowerState.changeState(JUMP);
        velocity = Vector2{ 0, 0 };
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
            topAnimOffsetY = -6;
            topAnimOffsetX = -2;
        if (abs(stairPos - position.x) > 0.5f  && lockStair == 0) {
            if ((isOnStair == -1 || isOnStair == 1) && IsKeyDown(KEY_DOWN) || (isOnStair == 2 || isOnStair == -2) && IsKeyDown(KEY_UP)) {
                topSprite->setAnimation("walk");
                bottomSprite->setAnimation("walk");
                if (stairPos - position.x > 0) {
                    moveHLinear(maxSPD);
                    direction = 1;
                }
                else {
                    direction = -1;
                    moveHLinear(-maxSPD);
                }
                
            }
            else lowerState.changeState(IDLE);
        }
        else {
            float y = GameManager::getInstance().getActiveScene()->getTileHeight() / 2;
            if (isOnStair != 0) lockStair = isOnStair;
            if ((position.x < stairPos - 0.5f && (lockStair == -1 || lockStair == 2))
            || (position.x > stairPos + 0.5f && (lockStair == 1 || lockStair == -2))) lockStair = 0;
            if (lockStair < 0) {
                if (IsKeyDown(KEY_UP) || direction == -1 && abs(position.y / y - (int)(position.y / y)) > 0.1 ) {
                    moveVLinear(-maxSPD * 0.7);
                    moveHLinear(-maxSPD * 0.7);
                    direction = -1;
                    bottomSprite->setAnimation("stairsWalkUp");
                } 
                else if (IsKeyDown(KEY_DOWN) || direction == 1 && abs(position.y / y - (int)(position.y / y)) > 0.1) {
                    moveVLinear(maxSPD * 0.7);
                    moveHLinear(maxSPD * 0.7);
                    direction = 1;
                    bottomSprite->setAnimation("stairsWalkDown");
                }
                else {
                    direction > 0 ? bottomSprite->setAnimation("stairsIdleDown") : bottomSprite->setAnimation("stairsIdleUp");
                }
            }
            else if (lockStair > 0) {
                if (IsKeyDown(KEY_UP) || direction == 1 && abs(position.y / y - (int)(position.y / y)) > 0.1) {
                    moveVLinear(-maxSPD * 0.7);
                    moveHLinear(maxSPD * 0.7);
                    direction = 1;
                    bottomSprite->setAnimation("stairsWalkUp");
                }
                else if (IsKeyDown(KEY_DOWN) || direction == -1 && abs(position.y / y - (int)(position.y / y)) > 0.1) {
                    moveVLinear(maxSPD * 0.7);
                    moveHLinear(-maxSPD * 0.7);
                    direction = -1;
                    bottomSprite->setAnimation("stairsWalkDown");
                }
                else {
                    direction > 0 ? bottomSprite->setAnimation("stairsIdleUp") : bottomSprite->setAnimation("stairsIdleDown");
                }
            }
            if (bottomSprite->getAnimation() == "stairsIdleDown" || bottomSprite->getAnimation() == "stairsIdleUp") topSprite->setAnimation("stairsIdle");
            else topSprite->setAnimation("stairsWalk");

            //if (IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_DOWN)) {
            //    float x = GameManager::getInstance().getActiveScene()->getTileWidth() / 2;
            //    float y = GameManager::getInstance().getActiveScene()->getTileHeight() / 2;
            //    position.x = ((int)(position.x / x)) * x;
            //    position.y = ((int)(position.y / y)) * y;
            //    
            //}

        }

        break;
    case DIE:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -13;
        bottomSprite->setAnimation("dead");
        GameManager::getInstance().getGamePointer()->publicPlayMusic(Game::PLAYER_MISS);
        deathTimer.updateTimer();
        if (deathTimer.isTriggerd()) GameManager::getInstance().getGamePointer()->requestSceneReload();
        break;
    case KNOCKBACK:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        bottomSprite->setAnimation("stairsIdleDown");
        if (lowerState.previous != STAIRS && jumpAllowed) {
            jumpAllowed = false;
            velocity.x = -50 * direction;
            velocity.y = -180;
        }
        *health -= isDamaged;
        isDamaged = 0;

        moveH(false, false);
        moveV();
        
        //Transition
        if (isOnFloor && velocity.y >= 0) upperState.changeState(STUN);
        break;

    case ATTACK:
            bottomAnimOffsetY = 17;
            bottomAnimOffsetX = -2;
        
        if (!isOnFloor) moveV();
        if (!(lowerState.previous == WALK && lowerState.previous == IDLE) && !isOnFloor) {
            moveH(false, false);
            bottomSprite->setAnimation("crouch");
        }
        else {
            moveH(false, true);
            bottomSprite->setAnimation("idle");
        }
        //this should only happen if the player attacks from WALK, STAIRS or IDLE
        break;

    case CROUCH:
            bottomAnimOffsetY = 25;
            bottomAnimOffsetX = -2;
        bottomSprite->setAnimation("crouch");
        moveH(false, true);

        if (IsKeyUp(KEY_DOWN) && !attackTimer.isActive() && !stunTimer.isActive()) lowerState.changeState(IDLE);
    }

    //Upper body state machine
    switch (upperState.current) {
    
    case IDLE:
        
            topAnimOffsetY = - 6;
            topAnimOffsetX = -2;
        if (lowerState.current != WALK && lowerState.current != STAIRS) topSprite->setAnimation("idle");
        else if (lowerState.current != STAIRS) topSprite->setAnimation("walk");

        //transition
        if (IsKeyPressed(KEY_D)) upperState.changeState(STARTATTACK);
        else if (IsKeyPressed(KEY_A) && 
            *subWeapon != GameManager::EMPTY &&
            *projectileCount <= *projectileUpgrade &&
            *ammo >= projCost) { subAttack = true;  upperState.changeState(STARTATTACK); }
        break;
    case STARTATTACK:
            topAnimOffsetY = -6;
            topAnimOffsetX = -13;
        topSprite->setAnimation("startAttack");
        if (subAttack) whipSprite->setAnimation("hidden");
        else if (*whipLevel > 0) whipSprite->setAnimation("longStart");
        else whipSprite->setAnimation("shortStart");
        startAttackTimer.updateTimer(deltaTime);

        //transition
        if (startAttackTimer.isTriggerd()) upperState.changeState(ATTACK);
        break;
    case ATTACK:
            topAnimOffsetY = -6;
            topAnimOffsetX = -13;
        topSprite->setAnimation("attack");
        if (subAttack) ;
        else if (*whipLevel == 2) whipSprite->setAnimation("lv3Attack");
        else if (*whipLevel == 1) whipSprite->setAnimation("lv2Attack");
        else whipSprite->setAnimation("lv1Attack");

        attackTimer.updateTimer(deltaTime);

        //Transition
        if (attackTimer.isTriggerd()) {
            whipSprite->setAnimation("hidden");
            upperState.changeState(IDLE);
            if (lowerState.current != CROUCH && lowerState.current != STAIRS) {
                if (!isOnFloor) lowerState.changeState(FALL);
                else if (isOnStair == 0) lowerState.changeState(IDLE);
                else lowerState.changeState(STAIRS);
            }
        }
        break;
    case STUN:
            topAnimOffsetY = -6;
            topAnimOffsetX = -2;
        topSprite->setAnimation("hurt");
        stunTimer.updateTimer(deltaTime);

        //Transition
        if (*health <= 0) lowerState.changeState(DIE);
        if (stunTimer.isTriggerd()) upperState.changeState(IDLE);
        break;
    case KNOCKBACK:
        topAnimOffsetY = -6;
        topAnimOffsetX = -2;
        topSprite->setAnimation("hurt");
        break;
    case DIE:
        topAnimOffsetY = -6;
        topAnimOffsetX = -13;
        topSprite->setAnimation("dead");
        break;

    }
    if (lowerState.current == CROUCH || lowerState.current == STUN) topAnimOffsetY = 2;
    updateAnimation();

    lateUpdate(); // For things that need to be done after everything else
}

void Player::lateUpdate() {
    updateColliderPosiotions();
    GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(*ammo), 1);
    GameManager::getInstance().getActiveScene()->setDebugMessage(to_string(*projectileCount), 2);
}

void Player::drawPlayer() {
    //DrawRectangleRec(hurtbox, DARKGREEN);
    if (!invincibilityTimer.isActive() || (int) (GetTime()/GetFrameTime()) % 8 > 0 || lowerState.current == DIE)
    {
        topSprite->draw(Vector2{ offsetX + topAnimOffsetX, offsetY + topAnimOffsetY });
        bottomSprite->draw(Vector2{ offsetX + bottomAnimOffsetX, offsetY + bottomAnimOffsetY });
    }
    /*DrawRectangleRec(groundCollider, RED);
    DrawRectangleRec(topCollider, RED);
    DrawRectangleRec(leftCollider, RED);
    DrawRectangleRec(rightCollider, RED);*/
    if (upperState.current == STARTATTACK) {
        whipSprite->draw(Vector2{ offsetX + (topAnimOffsetX * direction)-4, offsetY + topAnimOffsetY + 13 });
    }
    if (upperState.current == ATTACK && whipSprite->getAnimation().find("Start") == string::npos) {
       // DrawRectangleRec(whipCollider, WHITE);
        whipSprite->draw(Vector2{ offsetX - 23.5f + 39.5f * direction, offsetY + topAnimOffsetY + 9});
    } 
}

void Player::betweenStates(int previous, int current, int future, PlayerState* state) {
    if (state == &upperState) {
        if (current == IDLE && future == STARTATTACK) {
            whipCollider.width = GameManager::getInstance().whipLevel < 2 ? 32 : 48;
            startAttackTimer.startTimer();
            if (lowerState.current != CROUCH && lowerState.current != STAIRS) lowerState.changeState(ATTACK);
        }
        else if (current == STARTATTACK && future == ATTACK) {
            if (subAttack) {
                if (*subWeapon == GameManager::DAGGER) GameManager::getInstance().getActiveScene()->pushProjectile(new Dagger({ position.x + direction * 8, position.y }, direction, Projectile::PLAYER));
                else if (*subWeapon == GameManager::AXE) GameManager::getInstance().getActiveScene()->pushProjectile(new Axe({ position.x + direction * 8, position.y }, direction, Projectile::PLAYER));
                *ammo-=projCost;
                (*projectileCount)++;
            }
            else {
                GameManager::getInstance().getActiveScene()->pushPlayerHitBoxes(damageRect{ &whipCollider, *whipLevel == 0 ? (short)1 : (short)2 });
                GameManager::getInstance().getGamePointer()->publicPlaySound(Game::WHIP);
            }
            
            attackTimer.startTimer();
        }
        else if (current == ATTACK) {
            if (subAttack == false) GameManager::getInstance().getActiveScene()->removePlayerHitBoxes(&whipCollider);
            subAttack = false;
        }
        else if (future == STUN) {
            stunTimer.startTimer();
            lowerState.changeState(CROUCH);
        }
    }
    else {
        if (current == FALL) maxHeight = 256;
        if (current == STAIRS) lockStair = 0;
        if (future == KNOCKBACK) 
        {
            upperState.changeState(KNOCKBACK);
            GameManager::getInstance().getGamePointer()->publicPlaySound(Game::HURT);
        }
        if (future == DIE) {
            upperState.changeState(DIE);
            deathTimer.startTimer();
        }
    }
    
}

void Player::updateDirection() {
    if (getInputAxis() != 0) direction = getInputAxis();
}

void Player::updateAnimation() {
    topSprite->setFlipX(direction == -1);
    bottomSprite->setFlipX(direction == -1);
    whipSprite->setFlipX(direction == -1);
    topSprite->update(deltaTime);
    bottomSprite->update(deltaTime);
    whipSprite->update(deltaTime);
}