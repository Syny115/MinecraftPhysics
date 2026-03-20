#include "Scene.h"
#include <stack>


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


	int background = 0; int foreground = 0;
	int len = doc["layers"].size();
	for (int i = 0; i < len; i++) {
		if (doc["layers"][i]["name"] == "BackGround") background = i;
		if (doc["layers"][i]["name"] == "ForeGround") foreground = i;
	}
	len = row * col;
	int bData, fData;
	for (int j = 0; j < len; j++) {
		bData = doc["layers"][background]["data"][j];
		fData = doc["layers"][foreground]["data"][j];
		tileMat[j / col][j % col] = bData;
		if (fData != 0) {
			collisionMat[j / col][j % col] = 1;
			tileMat[j / col][j % col] = fData;
		}
		else collisionMat[j / col][j % col] = 0;

	}

    matrixToRects(&solidRects, collisionMat, tileWidth, tileHeight);

	data.close();
}

