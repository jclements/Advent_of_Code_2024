#include "array2d.h"

int resizeArray2d(Array2D *arr, int newRows, int newCols) {
	if(!arr) return -1;

	if(newRows > arr->maxRows) {
		// add rows if needed
		void **newArr = realloc(arr->array, sizeof(void*) * newRows);
		if(newArr) return -1;

		// set new rows to NULL, to be allocated in the column step
		for(int i = arr->maxRows; i < newRows; i++) {
			newArr[i] = NULL;
		}

		arr->array = newArr;
		arr->maxRows = newRows;
	}

	// if needed, resize each row to new max columns value
	for(int i = 0; i < arr->maxRows; i++) {
		if(!arr->array[i]) {
			// this is a brand new row that needs to be allocated for the first time
			arr->array[i] = calloc(arr->maxCols, arr->typeSize);
		} else if(newCols > arr->maxCols) {
			void *newRow = realloc(arr->array[i], newCols * arr->typeSize);
			if(!newRow) return -1;
			arr->array[i] = newRow;
		}
	}

	arr->maxCols = newCols;

	return 0;
}

int moreCols(Array2D *arr) {
	return resizeArray2d(arr, arr->maxRows, arr->maxCols * 2);
}
int moreRows(Array2D *arr) {
	return resizeArray2d(arr, arr->maxRows * 2, arr->maxCols);
}

void *getElement(Array2D *arr, int row, int col) {
	if(row >= arr->maxRows || col >= arr->maxCols) return NULL;

	void *value = (char *)arr->array[row] + (arr->typeSize * col);

	return value;
}

int setElement(Array2D *arr, int row, int col, const void *value) {
	if(row < 0 || col < 0) return -1;

	while(row >= arr->maxRows) moreRows(arr);
	while(col >= arr->maxCols) moreCols(arr);

	void *dest = (char *)arr->array[row] + (arr->typeSize * col);
	memcpy(dest, value, arr->typeSize);
	return 0;
}

Array2D *newArray2D(int initRows, int initCols, size_t typeSize) {
	Array2D *arr = malloc(sizeof(Array2D));
	arr->numRows = 0;
	arr->numCols = 0;
	arr->maxRows = initRows;
	arr->maxCols = initCols;
	arr->typeSize = typeSize;

	arr->array = malloc(sizeof(void *) * initRows);
	for(int i = 0; i < initRows; i++) {
		arr->array[i] = calloc(initCols, typeSize);
	}

	return arr;
}

void freeArray2D(Array2D *arr) {
	for(int row = 0; row < arr->maxRows; row++) {
		free(arr->array[row]);
	}
	free(arr->array);

	arr->array = NULL;
	arr->numRows = arr->numCols = arr->maxRows = arr->maxCols = 0;
}
