#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

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
										if (mat[i][j] == 1) rects->push_back(Rectangle{j*80, i*80, 80, 80});

                }
            }
        } else if (finalRect.area == 0) {
            done = true;
        }
        else {
            cout << "final rectangle\nx:" << finalRect.left << " y:" << rectLevel - finalRect.height + 1 << " width:" << finalRect.right - finalRect.left + 1 << " height:" << finalRect.height << endl << "left:" << finalRect.left << " right:" << finalRect.right << " level:" << rectLevel <<endl;
						rects->push_back(Rectangle{finalRect.left*80, (rectLevel - finalRect.height + 1)*80, (finalRect.right - finalRect.left + 1)*80, (finalRect.height)*80});
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







int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 640, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Image tileInput = LoadImage("SceneTile.png");
	Texture2D tileMap = LoadTexture("TileSetTest.png");
	bool debug = true;

	vector<vector<int>> mat(tileInput.height, std::vector<int>(tileInput.width));


	for (int i = 0; i < tileInput.width; i++) {
		for (int j = 0; j < tileInput.height; j++) {
			if (ColorIsEqual(GetImageColor(tileInput, i, j), BLACK) ||
				ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0x00, 0xFF, 0xFF}) ||
				ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0xFF, 0x00, 0xFF}) ||
				ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0xFF, 0xFF, 0xFF}) ||
				ColorIsEqual(GetImageColor(tileInput, i, j), {0xFF, 0x00, 0xFF, 0xFF})) {
				mat[j][i] = 1;
			}
		}
	}


	vector<Rectangle> collisionRects;
	matrixToRects(&collisionRects, mat);
	/*for (int i = 0; i < collisionRects.size() && !debug; i++) {
				cout << " x:"<< collisionRects[i].x<< " y:"<< collisionRects[i].y<< " width:"<< collisionRects[i].width<< " height:"<< collisionRects[i].height <<endl;
	}*/

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{

		if (IsKeyPressed(KEY_SPACE)) debug = !debug;

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground({0xA0, 0xF0, 0xFF, 0xFF});


		// draw some text using the default fon
	Rectangle src;
	Rectangle dest;
		for (int i = 0; i < tileInput.width; i++) {
			for (int j = 0; j < tileInput.height; j++) {
				if (ColorIsEqual(GetImageColor(tileInput, i, j), BLACK)) {
					src = {0, 3*16, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				}	else if (ColorIsEqual(GetImageColor(tileInput, i, j), WHITE)) {
					src = {16, 16, 32, 32};
					dest = {i*80, j*80, 160, 160};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0x00, 0xFF, 0xFF})) {
					src = {0, 2*16, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0xFF, 0x00, 0xFF})) {
					src = {0, 16, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0x00, 0xFF, 0xFF, 0xFF})) {
					src = {0, 0, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0xFF, 0x00, 0xFF, 0xFF})) {
					src = {16, 0, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0xFF, 0xFF, 0x00, 0xFF})) {
					src = {16*2, 0, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				} else if (ColorIsEqual(GetImageColor(tileInput, i, j), {0xFF, 0x00, 0x00, 0xFF})) {
					src = {16*3, 0, 16, 16};
					dest = {i*80, j*80, 80, 80};
					DrawTexturePro(tileMap, src, dest, Vector2{0, 0}, 0, WHITE);
				}
			}
		}

		for (int i = 0; i < collisionRects.size() && !debug; i++) {
					DrawRectangleRec(collisionRects[i], BLACK);
					DrawRectangle(collisionRects[i].x+5, collisionRects[i].y+5, collisionRects[i].width-10, collisionRects[i].height-10, GREEN);
		}


		// draw our texture to the screen


		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadImage(tileInput);
	UnloadTexture(tileMap);


	CloseWindow();
	return 0;
}
