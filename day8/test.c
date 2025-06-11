#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array2d.h"
#include "array.h"
#include "define.h"
#include <assert.h>

int main() {
	Array2d* arr = newArray2d(2, 3, sizeof(int));
	Array* ar = newArray(3, sizeof(int));
	assert(arr != NULL);
	assert(ar != NULL);
	assert(arr2d_numRows(arr) == 2);
	assert(arr2d_numCols(arr) == 3);
	assert(array_numElements(ar) == 3);

	// ints to store
	int a = 10, b = 20;

	// set pointers
	arr2d_setElement(arr, 0, 0, &a);
	arr2d_setElement(arr, 1, 1, &b);
	array_setElement(ar, 1, &a);

	// get values
	int* a_out = arr2d_getElement(arr, 0, 0);
	int* b_out = arr2d_getElement(arr, 1, 1);
	int* a2_out = array_getElement(ar, 1);

	// check return the proper value
	assert(*a_out == a);
	assert(*b_out == b);
	assert(*a2_out == a);

	// resize up
	assert(resizeArray2d(arr, 4, 4) == 0);
	assert(resizeArray(ar, 4) == 0);
	assert(arr2d_numRows(arr) == 4);
	assert(arr2d_numCols(arr) == 4);
	assert(array_numElements(ar) == 4);

	// check preserved values
	assert(*(int*)arr2d_getElement(arr, 0, 0) == 10);
	assert(*(int*)arr2d_getElement(arr, 1, 1) == 20);
	assert(*(int*)array_getElement(ar, 1) == 10);

	// try with doubles
	Array2d* darr = newArray2d(2, 2, sizeof(double));
	double x = 3.14;
	arr2d_setElement(darr, 0, 1, &x);
	double *x_out = arr2d_getElement(darr, 0, 1);
	assert(*x_out == x);

	freeArray2d(arr);
	freeArray2d(darr);
	freeArray(ar);

	printf("All tests passed\n");
}
