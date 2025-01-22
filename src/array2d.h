#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdlib.h>
#include <string.h>

typedef struct {
	void **array;			// pointer to array of rows
	int numRows;
	int maxRows;
	int numCols;
	int maxCols;
	size_t typeSize;	// size of the type stored
} Array2D;

int resizeArray2d(Array2D *arr, int newRows, int newCols);
void *getElement(Array2D *arr, int row, int col);
int setElement(Array2D *arr, int row, int col, const void *value);
Array2D *newArray2D(int initRows, int initCols, size_t typeSize);
void freeArray2D(Array2D *arr);
int moreCols(Array2D *arr);
int moreRows(Array2D *arr);

#endif // ARRAY2D_H
