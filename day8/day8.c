#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array2d.h"
#include "array.h"
#include "define.h"

#define INITIAL_SIZE 1

typedef struct Node {
	struct Node *nextLoc;
	struct Node *nextSym;
	int x;
	int y;
	char sym;
} Node;

typedef struct {
	Array2d *antinodes;
	Array2d *symMap;
	Array *symbols;
	int rowSize;
} Map;

char *getLine(FILE *file);
Node *newNode();
void printMap(Map *m);
void printAntis(Map *m);
Node *findSymbol(Map *m, char s);
void addSymbolNode(Map *m, char s, int row, int col);

int main() {
	FILE *file;

	Map *map = malloc(sizeof(Map));
	map->rowSize = 0;
	char *line;

	map->symbols = newArray(0, sizeof(Node *));
	map->antinodes = newArray2d(0, 0, sizeof(bool));
	map->symMap = newArray2d(0, 0, sizeof(bool));

	// open the file for reading
	file = fopen("example.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((line = getLine(file))) {
		// if this is the first line, get the width and initialize
		// array of antinodes
		if(arr2d_numRows(map->antinodes) == 0) {
			map->rowSize = strlen(line);
			resizeArray2d(map->antinodes, 1, map->rowSize);
			resizeArray2d(map->symMap, 1, map->rowSize);
		}


		// iterate through row and add new nodes if needed
		// also fill in 0s to antinodes array so it's ready for later
		bool initFalse = false;
		for(int i = 0; i < map->rowSize; i++) {
			int currRow = arr2d_numRows(map->antinodes)-1;
			arr2d_setElement(map->antinodes,currRow,i,&initFalse);

			char initSym = line[i];
			PRINT_CHAR(initSym);
			arr2d_setElement(map->symMap,currRow,i,&initSym);

			if(initSym != '.') {
				// add symbol to symbol array2d
				addSymbolNode(map, initSym, currRow, i);
			}
		}
	}

	printMap(map);
}

char *getLine(FILE *file) {
	int size = 16;	// initial size
	int len = 0;		// current length
	char *buffer = malloc(size);
	int ch;

	while((ch = fgetc(file)) != EOF) {
		if(ch == '\n') break;
		buffer[len++] = ch;

		if(len + 1 >= size) {
			// we need one more space for null-term char
			size *= 2;
			buffer = realloc(buffer, size);
		}
	}

	// now ch is EOF or newline
	if(len == 0 && ch == EOF) {
		free(buffer);
		buffer = NULL;
		return NULL;
	}

	buffer[len] = '\0';
	return buffer;
}

Node *newNode() {
	Node *new = malloc(sizeof(Node));

	if(new == NULL) return NULL;

	new->next = NULL;

	return new;
}

int power(int x, int n) {
	int v = 1;
	for(int i = 0; i < n; i++) {
		v *= x;
	}
	return v;
}


void toTrinary(int x, char *x3, int places) {
	int index = 0;
	for(int i = 0; i < places; i++) {
		x3[i] = '0';
	}
	x3[places] = '\0';

	while(x > 0) {
		x3[index++] = '0' + (x % 3);
		x /= 3;
	}
}

void printMap(Map *m) {
	printf("\n");
	for(int row = 0; row < arr2d_numRows(m->symMap); row++) {
		char *currRow = arr2d_getRow(m->symMap, row);
		printf("%s\n", currRow);
	}
}

void printAntis(Map *m);

void addSymbolNode(Map *m, char s, int row, int col) {
	PRINT_CHAR(s);

	Node *dest = findSymbol(m, s);
	if(dest) {
		// found this symbol in table already, add to end of linked list
		printf("%c is in table already\n", s);
	} else {
		// symbol not found, add a new element to the array
		Node *n = newNode();
		n->sym = s;
		n->x = col;
		n->y = row;
		int size = array_numElements(m->symbols);
		resizeArray(m->symbols, size+1);
		array_setElement(m->symbols, 0, &n);
		printf("I just put %c (%c) into the Array that has %d elements\n", s, ((Node *)(array_getElement(m->symbols,size)))->sym,size); 
	}
}

Node *findSymbol(Map *m, char s) {
	if(array_numElements(m->symbols) == 0) return NULL;
	Node *head = array_getElement(m->symbols,0);
	printf("head has sym: %c\n", head->sym);
	for(Node *p = head; (p - head) < array_numElements(m->symbols); p++) {
		printf("%c == %c?\n", s, p->sym);
		if(p->sym == s) return p;
	}
	return NULL;
}
