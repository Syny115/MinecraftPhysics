#include "Scene.h"
#include <stack>
#include <queue>

#define door0 897
#define door1 door0+1
#define door2 door0+2
#define door3 door0+3
#define spawn0 door0+32
#define spawn1 door1+32
#define spawn2 door2+32
#define spawn3 door3+32
#define save spawn3+1
#define stairsDownStart door0+64
#define stairsDownEnd door0+65
#define stairsUpStart door0+67
#define stairsUpEnd door0+66
#define breakableWall door0+4
#define breakableRoast door0+5
#define breakableUpgrade door0+37
#define candle door0+4+64
#define torch door0+5+64
#define jarCandle torch+1
#define crossCandle torch+2
#define axeChest door0+6
#define daggerChest axeChest+1
#define dashChest axeChest+2
#define holyChest axeChest+32
#define stopChest axeChest+32+1
#define boomerChest axeChest+32+2
#define bossCam torch+3
#define zombieStart door0+96
#define zombieEnd zombieStart+1
#define medusaStart zombieStart+2
#define medusaEnd zombieStart+3
#define batSpawn zombieStart+4
#define pantherSpawn zombieStart+5
#define mermanStart zombieStart+6
#define mermanEnd zombieStart+7
#define bossSpawn zombieStart+8
#define shieldKnightSpawn zombieStart+9
#define spearKnightSpawn zombieStart+10




struct maxRect {
    int area, left, right, height;
};

maxRect maxRectInHist(vector<int>& heights) {
    int length = heights.size();
    vector<int> left(length), right(length);


    stack<int> limitStack;

    //left limits
    for (int i = 0; i < length; i++) {
        if (limitStack.empty()) {
            limitStack.push(i);
            left[i] = 0;
        }
        else {
            while (!limitStack.empty() && heights[limitStack.top()] >= heights[i])
            {
                limitStack.pop();
            }
            left[i] = limitStack.empty() ? 0 : limitStack.top() + 1;
            limitStack.push(i);

        }
    }

    //empty stack
    while (!limitStack.empty()) { limitStack.pop(); }

    //right limits
    for (int i = length - 1; i >= 0; i--) {
        if (limitStack.empty()) {
            limitStack.push(i);
            right[i] = length - 1;
        }
        else {
            while (!limitStack.empty() && heights[limitStack.top()] >= heights[i])
            {
                limitStack.pop();
            }
            right[i] = limitStack.empty() ? length - 1 : limitStack.top() - 1;
            limitStack.push(i);

        }
    }

    maxRect mr = { 0, 0, 0, 0 };
    int curArea;
    for (int i = 0; i < length; i++) {
        curArea = (right[i] - left[i] + 1) * heights[i];
        if (mr.area < curArea) {
            mr.area = curArea;
            mr.left = left[i];
            mr.right = right[i];
            mr.height = heights[i];
        }
    }
    return mr;
}


void matrixToRects(vector<Rectangle>* rects, vector<vector<int>> mat, int tW, int tH) {
    int row = mat.size();
    int col = mat[0].size();
    bool done = false;

    while (!done) {
        vector<vector<int>> histMat = mat;
        for (int i = 1; i < row; i++) {
            for (int j = 0; j < col; j++) {
                histMat[i][j] = mat[i][j] == 0 ? 0 : histMat[i - 1][j] + mat[i][j];
            }
        }
        maxRect finalRect = { 0, 0, 0, 0 };
        maxRect currentRec;
        int rectLevel;
        for (int i = 0; i < row; i++) {
            currentRec = maxRectInHist(histMat[i]);
            if (finalRect.area < currentRec.area) {
                finalRect = currentRec;
                rectLevel = i;
            }
        }
        if (finalRect.area == 1) {
            done = true;
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (mat[i][j] == 1) rects->push_back(Rectangle{ (float)j * tW, (float)i * tH, (float) tW, (float) tH});

                }
            }
        }
        else if (finalRect.area == 0) {
            done = true;
        }
        else {
            rects->push_back(Rectangle{ (float)finalRect.left * tW, (float)(rectLevel - finalRect.height + 1) * tH, (float)(finalRect.right - finalRect.left + 1) * tW, (float)(finalRect.height) * tH });
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if ((j >= finalRect.left && j <= finalRect.right) &&
                        (i >= rectLevel - finalRect.height + 1 && i <= rectLevel))
                        mat[i][j] = 0;
                }
            }
        }
    }
}

void matrixToRect(Rectangle* rect, vector<vector<int>> mat, int tW, int tH) {
    int row = mat.size();
    int col = mat[0].size();

        vector<vector<int>> histMat = mat;
        for (int i = 1; i < row; i++) {
            for (int j = 0; j < col; j++) {
                histMat[i][j] = mat[i][j] == 0 ? 0 : histMat[i - 1][j] + mat[i][j];
            }
        }
        maxRect finalRect = { 0, 0, 0, 0 };
        maxRect currentRec;
        int rectLevel;
        for (int i = 0; i < row; i++) {
            currentRec = maxRectInHist(histMat[i]);
            if (finalRect.area < currentRec.area) {
                finalRect = currentRec;
                rectLevel = i;
            }
        }
        if (finalRect.area == 1) {
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if (mat[i][j] == 1) *rect = Rectangle{ (float)j * tW, (float)i * tH, (float)tW, (float)tH };

                }
            }
        }
        else if (finalRect.area == 0) {}
        else {
            *rect = Rectangle{ (float)finalRect.left * tW, (float)(rectLevel - finalRect.height + 1) * tH, (float)(finalRect.right - finalRect.left + 1) * tW, (float)(finalRect.height) * tH };
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
                    if ((j >= finalRect.left && j <= finalRect.right) &&
                        (i >= rectLevel - finalRect.height + 1 && i <= rectLevel))
                        mat[i][j] = 0;
                }
            }
        }
    
}






void PlayableScene::parseTiles(const char* path) {
	ifstream data(path);
	json doc = json::parse(data);
	int row = doc["height"];
	int col = doc["width"];

	vector<vector<int>> collisionMat(row, vector<int>(col));
	tileMat = collisionMat;
	tileHeight = doc["tileheight"];
	tileWidth = doc["tilewidth"];
	worldHeight = row * tileHeight;
	worldWidth = col * tileWidth;

    vector<Vector2> upVec;
    vector<Vector2> dVec;
    queue<Vector2> zombQ;
    queue<Vector2> meduQ;

    vector<vector<int>> doorMat0(row, vector<int>(col));
    vector<vector<int>> doorMat1(row, vector<int>(col));
    vector<vector<int>> doorMat2(row, vector<int>(col));
    vector<vector<int>> doorMat3(row, vector<int>(col));


	int background = 0, foreground = 0, breakables = 0, level = 0;
	int len = doc["layers"].size();
	for (int i = 0; i < len; i++) {
		if (doc["layers"][i]["name"] == "BackGround") background = i;
		if (doc["layers"][i]["name"] == "ForeGround") foreground = i;
		if (doc["layers"][i]["name"] == "Breakables") breakables = i;
		if (doc["layers"][i]["name"] == "Level" || doc["layers"][i]["name"] == "Levels") level = i;
	}
	len = row * col;
	int bData, fData, lData, bkData;
	for (int j = 0; j < len; j++) {
		bData = doc["layers"][background]["data"][j];
		fData = doc["layers"][foreground]["data"][j];
        bkData = doc["layers"][breakables]["data"][j];
        lData = doc["layers"][level]["data"][j];
		tileMat[j / col][j % col] = bData;
		if (fData != 0) {
			collisionMat[j / col][j % col] = 1;
			tileMat[j / col][j % col] = fData;
		}
		else collisionMat[j / col][j % col] = 0;
        //Breakables Data
        if (lData == door0) doorMat0[j / col][j % col] = 1; //Door 0
        else doorMat0[j / col][j % col] = 0;
        if (lData == door1) doorMat1[j / col][j % col] = 1; //Door 1
        else doorMat1[j / col][j % col] = 0;
        if (lData == door2) doorMat2[j / col][j % col] = 1; //Door 2
        else doorMat2[j / col][j % col] = 0;
        if (lData == door3) doorMat3[j / col][j % col] = 1; //Door 3
        else doorMat3[j / col][j % col] = 0;
        
        if (bkData == breakableWall) destructables.push_back(new DestructableWall(Vector2{ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }, 0)); //breakable block
        else if (bkData == breakableRoast) destructables.push_back(new DestructableWall(Vector2{ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }, 1)); //WITH CHICKEN!!
        else if (bkData == breakableUpgrade) destructables.push_back(new DestructableWall(Vector2{ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }, 2)); //projectile upgrade!!

        else if (bkData == candle) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth/2, tileHeight * (float)(j / col) }, 0, 0)); //Candle with random
        else if (bkData == torch) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) - tileHeight }, 1, 0)); //Torch with random
        else if (bkData == axeChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 1)); //Chest with axe
        else if (bkData == daggerChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 2)); //Chest with dagger
        else if (bkData == holyChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 3)); //Chest with Holy Water
        else if (bkData == stopChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 4)); //Chest with Stopwatch
        else if (bkData == boomerChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 5)); //Chest with Boomerang
        else if (bkData == jarCandle) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 0, 6)); //Candle with jar
        else if (bkData == crossCandle) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 0, 7)); //Candle with cross
        else if (bkData == dashChest) destructables.push_back(new DestructableLoot(Vector2{ (float)(j % col) * tileWidth + tileWidth / 2, tileHeight * (float)(j / col) }, 2, 8)); //Candle with dash

        //Level Data
        if (lData == spawn0) spawnponits[0] = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; //SPAWN 1 
        else if (lData == spawn1) spawnponits[1] = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; //SPAWN 2
        else if (lData == spawn2) spawnponits[2] = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; //SPAWN 3
        else if (lData == spawn3) spawnponits[3] = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; //SPAWN 4
        else if (lData == save) spawnponits[4] = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; //SAVE POINT
        else if (lData == stairsDownStart) dVec.push_back({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }); //STAIR START DOWN
        else if (lData == stairsUpEnd) {
            upVec.push_back({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col ) }); //STAIR END UP
        }
        else if (lData == zombieStart) zombQ.push({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) });       //START ZOMBER
        else if (lData == medusaStart) meduQ.push({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) });       //START MEDUSA
        else if (lData == batSpawn) batSpawners.push_back({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }); // BAT
        else if (lData == pantherSpawn) pantherSpawners.push_back({ (float)(j % col+1) * tileWidth, tileHeight * (float)(j / col+1) }); // PANTHER
       // else if (lData == mermanStart) batSpawners.push_back({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }); // MERMAN START
        else if (lData == bossSpawn) bossSpawner = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; // BOSS
        else if (lData == bossCam) bossStart = { (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }; // BOSS START
        else if (lData == shieldKnightSpawn) knightSpawners.push_back({ (float)(j % col) * tileWidth, tileHeight * (float)(j / col) }); // KNIGHT

        else if (lData == stairsDownEnd && !dVec.empty()) { //STAIR END DOWN
            int i = -1;
            float adX, adY;
            do {
                i++;
                adX = dVec[i].x - ((float)(j % col) * tileWidth);
                adY = dVec[i].y - ((float)(j / col) * tileHeight);
                
            } while (adX != adY && i < dVec.size() && adX < 0 && adY < 0);
            

            staircase s = {
                Rectangle{ dVec[i].x, dVec[i].y, 2.0f * tileWidth, 1.0f * tileHeight},
                Rectangle{ (float)(j % col) * tileWidth, (float)(j / col) * tileHeight, 2.0f * tileWidth, 1.0f * tileHeight},
                false
            };
            dVec.erase(dVec.begin() + i);
            stairs.push_back(s);
        }

        else if (lData == stairsUpStart && !upVec.empty()) { //STAIR START UP
            int i = -1;
            float adX, adY;
            do {
                i++;
                adX = upVec[i].x - ((float)(j % col) * tileWidth);
                adY = upVec[i].y - ((float)(j / col) * tileHeight);

            } while (adX != adY * -1 && i < upVec.size());
            



            staircase s = {
                Rectangle{ upVec[i].x- tileWidth, upVec[i].y, 2.0f * tileWidth, 1.0f * tileHeight},
                Rectangle{ (float)(j % col) * tileWidth - tileWidth, (float)(j / col) * tileHeight, 2.0f * tileWidth, 1.0f * tileHeight},
                true
            };
            upVec.erase(upVec.begin() + i);
            stairs.push_back(s);
        }

        else if (lData == zombieEnd) { //ZOMBER END
            Rectangle r = {
                zombQ.front().x,
                zombQ.front().y,
                (((float)(j % col)+1) * tileWidth) - zombQ.front().x,
                (((float)(j / col)+1) * tileHeight) - zombQ.front().y
            };
            zombQ.pop();
            zombieSpawners.push_back(r);
        }

        else if (lData == medusaEnd && !meduQ.empty()) { //MEDUSA END
            Rectangle r = {
                meduQ.front().x,
                meduQ.front().y,
                (((float)(j % col) + 1) * tileWidth) - meduQ.front().x,
                (((float)(j / col) + 1) * tileHeight) - meduQ.front().y
            };
            meduQ.pop();
            medusaSpawners.push_back(r);
        }
	}

   


    matrixToRects(&solidRects, collisionMat, tileWidth, tileHeight);
    matrixToRect(&exits[0], doorMat0, tileWidth, tileHeight);
    matrixToRect(&exits[1], doorMat1, tileWidth, tileHeight);
    matrixToRect(&exits[2], doorMat2, tileWidth, tileHeight);
    matrixToRect(&exits[3], doorMat3, tileWidth, tileHeight);

	data.close();
}



