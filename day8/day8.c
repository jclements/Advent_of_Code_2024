#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/array2d.h"

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)
#define INITIAL_SIZE 2

typedef struct Node {
	int x;
	int y;
	char sym;
	struct Node *next;
} Node;


typedef struct {
	Array2D *symbols;
	Array2D *antinodes;
	Array2D *symMap;
	int rowSize;
} Map;

char *getLine(FILE *file);
Node *newNode();
void printMap(Map *m);
void printAntis(Map *m);
Node *findSymbol(Map *m, char s);

int main() {

	FILE *file;

	Map *map = malloc(sizeof(Map));
	map->rowSize = 0;
	char *line;

	map->symbols = newArray2D(INITIAL_SIZE, INITIAL_SIZE, sizeof(Node));
	map->antinodes = newArray2D(INITIAL_SIZE, INITIAL_SIZE, sizeof(bool));
	map->symMap = newArray2D(INITIAL_SIZE, INITIAL_SIZE, sizeof(bool));

	// open the file for reading
	file = fopen("input.txt", "r");
	// ex2.txt should be 3965, 11603 pt 2

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((line = getLine(file))) {
		// if this is the first line, get the width and initialize
		// array of antinodes
		if(map->antinodes->numRows == 0) {
			map->rowSize = strlen(line);
			while(map->rowSize > map->antinodes->maxCols) {
				moreCols(map->antinodes);
				moreCols(map->symMap);
			}
		}

		// if antinode rows are already full, grow size
		if(map->antinodes->numRows >= map->antinodes->maxRows) {
			moreRows(map->antinodes);
			moreRows(map->symMap);
		}

		// iterate through row and add new nodes if needed
		// also fill in 0s to antinodes array so it's ready for later
		bool initFalse = false;
		for(int i = 0; i < map->rowSize; i++) {
			setElement(map->antinodes,map->antinodes->numRows,i,&initFalse);

			char initSym = line[i];
			setElement(map->symMap,map->symMap->numRows,i,&initSym);

			if(line[i] != '.') {
				// add symbol to symbol array2d
			}
			
			map->antinodes->numRows++;
			map->symMap->numRows++;
		}
	}
	printf("got to end\n");
	printf("double free might be happening after program is done\n");
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


