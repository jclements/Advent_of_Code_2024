#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
	Node **array;
	int numRows;
	int maxRows;
	int numCols;
	int maxCols;
} NodeArray2d;

typedef struct {
	bool ** array;
	int numRows;
	int maxRows;
	int numCols;
	int maxCols;
} BoolArray2d;

typedef struct {
	NodeArray2d symbols;
	BoolArray2d antinodes;
	BoolArray2d symMap;
} Map;

char *getLine(FILE *file);
Node *newNode();
void printMap(Map *m);
void printAntis(Map *m);
Node *findSymbol(Map *m, char s);

int main() {

	FILE *file;
	int ch;

	Map *map = malloc(sizeof(Map));
	char *line;

	map->symbols = malloc(sizeof(Node*) * map->maxSym);
	map->antinodes = malloc(sizeof(bool*) * map->maxRows);
	map->symMap = malloc(sizeof(bool*) * map->maxRows);

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
		if(map->rowSize == 0) {
			map->rowSize = strlen(line);
		}

		// if antinode rows are already full, grow size
		if(map->numRows >= map->maxRows) {
			map->maxRows *= 2;
			map->antinodes = realloc(map->antinodes, sizeof(bool*) * map->maxRows);
		}

		// iterate through row and add new nodes if needed
		// also fill in 0s to antinodes array so it's ready for later
		for(int i = 0; i < map->rowSize; i ++) {
			map->antinodes[map->numRows][i] = false;
			if(line[i] != '.') {
				map->curr->x = i;
				map->curr->y = map->numRows;
				map->curr->sym = line[i];
				map->curr->next = newNode();
				map->curr = map->curr->next;
			}

			map->numRows++;
		}
	}
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

bool testEq(equation *eq) {
	int places = eq->size - 1;
	int max = power(2, places);	// size-1 places ^2 possible values
	int test = 0;
	unsigned long long int val;

	while(test < max) {
		val = eq->numbers[0];
		for(int i = 0; i < places; i++) {
			int op = (test >> i) & 1;
			if(op) {
				// printf("times ");
				val *= eq->numbers[i+1];
			} else {
				// printf("plus ");
				val += eq->numbers[i+1];
			}
		}
		// printf("= %lld\n", val);
		if(val == eq->value) eq->num_correct++;
		test++;
	}
	if(eq->num_correct > 0) return true;

	return false;
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

bool testEq3(equation *eq) {
	int places = eq->size - 1;
	int max = power(3, places);	// size-1 places ^3 possible values
	int test = 0;
	unsigned long long int val;
	char ops[100];

	while(test < max) {
		toTrinary(test, ops, places);
		val = eq->numbers[0];
		for(int i = 0; i < places; i++) {
			int op = (test >> i) & 1;
			if(ops[i] == '0') {
				// printf("times ");
				val *= eq->numbers[i+1];
			} else if(ops[i] == '1') {
				// printf("plus ");
				val += eq->numbers[i+1];
			} else {
				// printf("cat ");
				char val_str[20];
				char next[20];
				snprintf(val_str, sizeof(val_str), "%lld", val);
				snprintf(next, sizeof(next), "%d", eq->numbers[i+1]);
				strcat(val_str, next);
				char *endp;
				val = strtol(val_str, &endp, 10);

				if(*endp != '\0') {
					// printf("error converting value to long int\n");
					return -1;
				}
				// printf("val is %lld\n", val);
			}
		}
		// printf("= %lld\n", val);
		if(val == eq->value) eq->num_correct++;
		test++;
	}
	if(eq->num_correct > 0) return true;

	return false;
}

void printMap(Map *m) {
	for(int y = 0; y < m->maxRows; y++) {
		for(int x = 0; x < m->rowSize; x++) {

		}
	}
}

void printAntis(Map *m);

Node *findSymbol(Map *m, char s) {
	for(int i = 0; i < m->numSym; i++) {
		if(m->symbols[i]->sym == s) return m->symbols[i];
	}

	return NULL;
}
