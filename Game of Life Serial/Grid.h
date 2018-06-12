#pragma once

#include <vector>


// The Grid class models a 2D array of integers

class Grid {

private:

	int								n;		// Number of elements (n x n)
	std::vector<std::vector<int>>	data;	// Each column is represented by a vector (c++ version of Java's ArrayList)

public:

	Grid() {

		n = 0;
	}


	Grid(int size) {

		n = size;

		data = std::vector<std::vector<int>>(size);

		for (int i = 0; i < size; ++i) {

			data[i] = std::vector<int>(size);

			for (int j = 0; j < size; ++j) {

				data[i][j] = 0;
			}
		}
	}

	std::vector<int>& operator[](const int i) {

		return data[i];
	}

	int getNumElements() {

		return n;
	}
};
