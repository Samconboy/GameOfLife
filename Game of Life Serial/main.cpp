
#include "GridWindow.h"
#include <iostream>
#include <tbb\tbb.h>
#include <random>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>

using namespace std;
using namespace tbb;


void initialise(Grid& gridArray);
void update(Grid& inputArray, Grid& outputArray);

const int n = 40;
const int ALIVE = 1;
const int DEAD = 0;
int updateCount = 0;


// Main start point for the programme.  This sets up a 2D array (Grid) and creates the window to display the grid.
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	// *** Create a 2D array (Grid) and call the initialise function to set it's elements
	// Example array size (n x n)

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CONOUT$", "w", stdout);

	printf("HELLO!!! I AM THE CONSOLE!");
	

	Grid myArray = Grid(n);
	initialise(myArray);

	


	// Create and show a window that draws the 2D array.  This example creates a window (400 x 400) pixels in size.
	GridWindow mainWindow = GridWindow(600, 600, myArray, hInstance, iCmdShow);

	// Run the main event loop.  This calls the update function defined below to modify the 2D array elements
	mainWindow.mainLoop(update);

	return 0;
}


// Function called to initialise the 2D grid array.  This uses TBB's parallel_for to setup an example checker-board pattern.
// *** ADD YOUR OWN INITIALISATION CODE TO SET THE APPROPRIATE SQUARES FOR YOUR "GAME OF LIFE" IMPLEMENTATION ***
void initialise(Grid& gridArray) {

	const int n = gridArray.getNumElements();

	gridArray[3][6] = ALIVE;
	gridArray[4][6] = ALIVE;
	gridArray[3][7] = ALIVE;
	gridArray[4][7] = ALIVE;
	gridArray[13][6] = ALIVE;
	gridArray[13][7] = ALIVE;
	gridArray[13][8] = ALIVE;
	gridArray[14][9] = ALIVE;
	gridArray[15][10] = ALIVE;
	gridArray[16][10] = ALIVE;
	gridArray[18][9] = ALIVE;
	gridArray[17][7] = ALIVE;
	gridArray[19][8] = ALIVE;
	gridArray[19][7] = ALIVE;
	gridArray[19][6] = ALIVE;
	gridArray[18][5] = ALIVE;
	gridArray[14][5] = ALIVE;
	gridArray[15][4] = ALIVE;
	gridArray[16][4] = ALIVE;
	gridArray[20][7] = ALIVE;
	gridArray[23][6] = ALIVE;
	gridArray[23][5] = ALIVE;
	gridArray[23][4] = ALIVE;
	gridArray[24][6] = ALIVE;
	gridArray[24][5] = ALIVE;
	gridArray[24][4] = ALIVE;
	gridArray[25][3] = ALIVE;
	gridArray[25][7] = ALIVE;
	gridArray[27][7] = ALIVE;
	gridArray[27][8] = ALIVE;
	gridArray[27][3] = ALIVE;
	gridArray[27][2] = ALIVE;
	gridArray[37][4] = ALIVE;
	gridArray[38][4] = ALIVE;
	gridArray[37][5] = ALIVE;
	gridArray[38][5] = ALIVE;
	/*
	parallel_for(

		blocked_range2d<int, int>(0, n, 0, n),

		[&](blocked_range2d<int, int>& range) {

		int yStart = range.rows().begin();
		int yEnd = range.rows().end();

		for (int y = yStart; y < yEnd; y++) {

			int xStart = range.cols().begin();
			int xEnd = range.cols().end();

			for (int x = xStart; x < xEnd; x++) {

				bool randbool = rand() & 1;

				// Set array element to 1 (on) or 0 (off) to create a checker-board pattern.
				gridArray[x][y] = randbool;


			}
		}
	}
	);*/
}

int getNeighbours(Grid& inputArray, int x, int y) {

	int neighbours = 0;

	if (inputArray[x - 1][y] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x - 1][y + 1] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x][y + 1] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x + 1][y + 1] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x + 1][y] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x + 1][y - 1] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x][y - 1] == ALIVE) {
		++neighbours;
	}

	if (inputArray[x - 1][y - 1] == ALIVE) {
		++neighbours;
	}

	return neighbours;
}


// Update function - this is called automatically from the window's event loop.  This takes an input array (inputArray) and returns the result of any processing in 'outputArray'.  Here you can set the output array values.

void update(Grid& inputArray, Grid& outputArray) {

	tick_count t0 = tick_count::now();

	int neighbours = 0;

	for (int x = 1; x < n - 1; ++x) {
		for (int y = 1; y < n - 1; ++y) {
		

	//Check neighbours	
	neighbours = getNeighbours(inputArray, x, y);
				

	//underpopulation
	if (inputArray[x][y] == ALIVE && neighbours < 2) {
		outputArray[x][y] = DEAD;
	}

	//overpopulation
	else if (inputArray[x][y] == ALIVE && neighbours >= 4) {
		outputArray[x][y] = DEAD;
	}

	//continues to live
	else if (inputArray[x][y] == ALIVE && neighbours == 2 || neighbours == 3) {

		outputArray[x][y] = ALIVE;
	}

	//population growth
	else if (inputArray[x][y] == DEAD && neighbours == 3) {
		outputArray[x][y] = ALIVE;
	}



	neighbours = 0;

}

		

	}

	tick_count t1 = tick_count::now();

	tick_count t2 = tick_count::now();

	for (int x = 1; x < n - 1; ++x) {
		for (int y = 1; y < n - 1; ++y) {
			inputArray[x][y] = outputArray[x][y];
		}
	}

	tick_count t3 = tick_count::now();

	updateCount++;

	printf("%d \n", updateCount);
	printf("calculate next gen: %f \n", (t1 - t0).seconds());
	printf("reset arrays gen:  %f \n", (t3 - t2).seconds());
	printf("total time for update: %f \n\n", (t3 - t0).seconds());

	

	ofstream excelFile;
	excelFile.open("test.csv", ofstream::app);
	excelFile << updateCount << "," << (t1 - t0).seconds() << "," << (t3 - t2).seconds() << "," << (t3 - t0).seconds() << endl;
	excelFile.close();

}









//void update(Grid& inputArray, Grid& outputArray) {
//
//	int neighbours = 0;
//	
//	for (int x = 1; x < n - 1; ++x) {
//		for (int y = 1; y < n - 1; ++y) {
//
//			int cellState = inputArray[x][y];
//
//			if (outputArray[x - 1][y - 1] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x][y - 1] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x - 1][y] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x + 1][y - 1] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x - 1][y + 1] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x][y + 1] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x + 1][y] = alive) {
//				neighbours++;
//			}
//			if (outputArray[x + 1][y + 1] = alive) {
//				neighbours++;
//			}
//
//			//underpop
//			if (cellState = alive && neighbours < 2) {
//				outputArray[x][y] = dead;
//			}
//
//			//overpop
//			else if (cellState = alive && neighbours >= 4) {
//				outputArray[x][y] = dead;
//			}
//
//			//normal
//			else if (cellState = alive && neighbours == 2 || neighbours == 3) {
//				outputArray[x][y] = alive;
//			}
//
//			//birth
//			else if (cellState = dead && neighbours == 3) {
//				outputArray[x][y] = alive;
//			}
//
//			neighbours = 0;
//
//		}
//
//	}
//
//	for (int x = 1; x < n - 1; ++x) {
//		for (int y = 1; y < n - 1; ++y) {
//
//				inputArray[x][y] = outputArray[x][y];
//		}
//	}
//	
//}

