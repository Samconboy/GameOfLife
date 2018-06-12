
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
const int alive = 1;
const int dead = 0;
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

	

	gridArray[3][6] = alive;
	gridArray[4][6] = alive;
	gridArray[3][7] = alive;
	gridArray[4][7] = alive;
	gridArray[13][6] = alive;
	gridArray[13][7] = alive;
	gridArray[13][8] = alive;
	gridArray[14][9] = alive;
	gridArray[15][10] = alive;
	gridArray[16][10] = alive;
	gridArray[18][9] = alive;
	gridArray[17][7] = alive;
	gridArray[19][8] = alive;
	gridArray[19][7] = alive;
	gridArray[19][6] = alive;
	gridArray[18][5] = alive;
	gridArray[14][5] = alive;
	gridArray[15][4] = alive;
	gridArray[16][4] = alive;
	gridArray[20][7] = alive;
	gridArray[23][6] = alive;
	gridArray[23][5] = alive;
	gridArray[23][4] = alive;
	gridArray[24][6] = alive;
	gridArray[24][5] = alive;
	gridArray[24][4] = alive;
	gridArray[25][3] = alive;
	gridArray[25][7] = alive;
	gridArray[27][7] = alive;
	gridArray[27][8] = alive;
	gridArray[27][3] = alive;
	gridArray[27][2] = alive;
	gridArray[37][4] = alive;
	gridArray[38][4] = alive;
	gridArray[37][5] = alive;
	gridArray[38][5] = alive;
	
	
}

int getNeighbours(Grid& inputArray, int x, int y) {

	int neighbours = 0;



	if (inputArray[x - 1][y] == alive) {
		++neighbours;
	}

	if (inputArray[x - 1][y + 1] == alive) {
		++neighbours;
	}

	if (inputArray[x][y + 1] == alive) {
		++neighbours;
	}

	if (inputArray[x + 1][y + 1] == alive) {
		++neighbours;
	}

	if (inputArray[x + 1][y] == alive) {
		++neighbours;
	}

	if (inputArray[x + 1][y - 1] == alive) {
		++neighbours;
	}

	if (inputArray[x][y - 1] == alive) {
		++neighbours;
	}

	if (inputArray[x - 1][y - 1] == alive) {
		++neighbours;
	}

	return neighbours;
}


// Update function - this is called automatically from the window's event loop.  This takes an input array (inputArray) and returns the result of any processing in 'outputArray'.  Here you can set the output array values.

void update(Grid& inputArray, Grid& outputArray) {

	
	tick_count t0 = tick_count::now();


	tbb::parallel_for(

		blocked_range2d<int, int>::blocked_range2d(1, n -1, 1, n -1),

		[&](const blocked_range2d<int, int>& r) {
		for (int x = r.rows().begin(); x != r.rows().end(); x++) {
			for (int y = r.cols().begin(); y != r.cols().end(); y++) {
				int neighbours = 0;

				if (inputArray[x - 1][y] == alive) {
					++neighbours;
				}

				if (inputArray[x - 1][y + 1] == alive) {
					++neighbours;
				}

				if (inputArray[x][y + 1] == alive) {
					++neighbours;
				}

				if (inputArray[x + 1][y + 1] == alive) {
					++neighbours;
				}

				if (inputArray[x + 1][y] == alive) {
					++neighbours;
				}

				if (inputArray[x + 1][y - 1] == alive) {
					++neighbours;
				}

				if (inputArray[x][y - 1] == alive) {
					++neighbours;
				}

				if (inputArray[x - 1][y - 1] == alive) {
					++neighbours;
				}


				//underpopulation
				if (inputArray[x][y] == alive && neighbours < 2) {
					outputArray[x][y] = dead;
				}

				//overpopulation
				else if (inputArray[x][y] == alive && neighbours >= 4) {
					outputArray[x][y] = dead;
				}

				//continues to live
				else if (inputArray[x][y] == alive && neighbours == 2 || neighbours == 3) {

					outputArray[x][y] = alive;
				}

				//population growth
				else if (inputArray[x][y] == dead && neighbours == 3) {
					outputArray[x][y] = alive;
				}



				neighbours = 0;
			}
			}
	}
	);

	tick_count t1 = tick_count::now();

	tick_count t2 = tick_count::now();
	tbb::parallel_for(

		blocked_range2d<int, int>::blocked_range2d(1, n - 1, 1, n - 1),

		[&](const blocked_range2d<int, int>& r) {
		for (int x = r.rows().begin(); x != r.rows().end(); x++) {
			for (int y = r.cols().begin(); y != r.cols().end(); y++) {
				inputArray[x][y] = outputArray[x][y];
			}
			}
	}
	);

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

void nextGen(Grid& inputArray, Grid& outputArray, int x, int y) {


	


}



