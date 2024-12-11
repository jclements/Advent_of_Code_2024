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

			if(newRow->first == NULL) {
				// this is the first entry in the row
				newRow->first = newNode;
			} else {
				// this is not the first entry
				Node *last = newRow->first;
				while(last->next != NULL) {
					last = last->next;
				}	// last now points to the last node since last->next == NULL

				last->next = newNode;
			}

			// increase size of row
			newRow->size += 1;

			// try to get another word
			word = strtok(NULL, " ");
		}

	}

	PRINT_INT(safe);
}
