#include "raylib.h"
//#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct maxRect {
    int area, left, right, height;
};


bool ColorIsEqual(Color c1, Color c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

maxRect maxRectInHist(vector<int>& heights) {
    int length = heights.size();
    vector<int> left(length), right(length);


    stack<int> limitStack;

    //left limits
    for (int i = 0; i < length; i++) {
        if (limitStack.empty()) {
            limitStack.push(i);
            left[i] = 0;
        } else {
            while (!limitStack.empty() && heights[limitStack.top()] >= heights[i])
            { limitStack.pop(); }
            left[i] = limitStack.empty()? 0 : limitStack.top() + 1;
            limitStack.push(i);

        }
    }

    //empty stack
    while (!limitStack.empty()) { limitStack.pop(); }

    //right limits
    for (int i = length-1; i >= 0; i--) {
        if (limitStack.empty()) {
            limitStack.push(i);
            right[i] = length-1;
        } else {
            while (!limitStack.empty() && heights[limitStack.top()] >= heights[i])
            { limitStack.pop(); }
            right[i] = limitStack.empty()? length -1: limitStack.top() - 1;
            limitStack.push(i);

        }
    }

    maxRect mr = {0, 0, 0, 0};
    int curArea;
    for (int i = 0; i < length; i++) {
        curArea = (right[i] - left[i] + 1) * heights[i];
        if (mr.area < curArea) {
            mr.area = curArea;
            mr.left = left[i];
            mr.right = right[i];
            mr.height = heights[i];
        }
       // cout <<left[i] << " " << right[i] << " = " << curArea <<  endl;
    }
    //cout << maxArea << endl;
    return mr;
}



void matrixToRects(vector<Rectangle>* rects, vector<vector<int>> mat) {
    int row = mat.size();
    int col = mat[0].size();
    bool done = false;

    while(!done) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }


        vector<vector<int>> histMat = mat;
        for (int i = 1; i < row; i++) {
            for (int j = 0; j < col; j++) {
                histMat[i][j] = mat[i][j] == 0 ? 0 : histMat[i-1][j] + mat[i][j];
            }
        }
        maxRect finalRect = {0, 0, 0, 0};
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
            cout << "The following are isolated rectangles\n";
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < col; j++) {
										if (mat[i][j] == 1) rects->push_back(Rectangle{ (float)j*80, (float)i*80, 80, 80});

                }
            }
        } else if (finalRect.area == 0) {
            done = true;
        }
        else {
            cout << "final rectangle\nx:" << finalRect.left << " y:" << rectLevel - finalRect.height + 1 << " width:" << finalRect.right - finalRect.left + 1 << " height:" << finalRect.height << endl << "left:" << finalRect.left << " right:" << finalRect.right << " level:" << rectLevel <<endl;
						rects->push_back(Rectangle{(float)finalRect.left*80, (float)(rectLevel - finalRect.height + 1)*80, (float)(finalRect.right - finalRect.left + 1)*80, (float)(finalRect.height)*80});
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
