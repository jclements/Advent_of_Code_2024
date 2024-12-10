#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)

typedef struct Node {
	int number;
	struct Node *next;
} Node;

typedef struct Row {
	int size;
	Node *first;
	struct Row *next;
} Row;

int main() {
	FILE *file;
	char line[256];
	char *word;
	int curr, safe = 0;

	Row *rows;

	Row *last_row = NULL;


	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each line from the file
	while(fgets(line, sizeof(line), file)) {
		// new row, create a new item in rows
		Row *newRow = malloc(sizeof(Row));
		newRow->first = NULL;
		newRow->size = 0;
		newRow->next = NULL;

		if(last_row == NULL) {
			// this is the first row
			rows = newRow;
			last_row = newRow;
		} else {
			// this is not the first row
			last_row->next = newRow;
			last_row = newRow;
		}

		// split into words
		word = strtok(line, " ");

		while(word != NULL) {
			// convert to ints and add into new node
			curr = atoi(word);
			Node *newNode = malloc(sizeof(Node));
			newNode->next = NULL;
			newNode->number = curr;


			// try to get another word
			word = strtok(NULL, " ");
		}

		// if word is NULL we made it to the end of the line without 
		// failing any tests
		if(word == NULL) {
			safe += 1;
		}

	}

	PRINT_INT(safe);
}
