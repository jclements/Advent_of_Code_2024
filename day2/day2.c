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

bool isSafe(Node *n);

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

	Row *r = rows;
	while(r != NULL) {
		if(isSafe(r->first)) {
			safe += 1;
		}
		r = r->next;
	}

	PRINT_INT(safe);
}

bool isSafe(Node *n) {
	int curr, prev, dir = 0;
	curr = n->number;
	while(n->next != NULL) {
		// set up next numbers to test
				prev = curr;
		curr = n->next->number;

		int gap = prev - curr;

		// must increase or decrease by 1, 2, or 3
		if(abs(gap) < 1 || abs(gap) > 3) return false;

		// calculate current direction
		int cur_dir = (gap) / abs(gap);

		if(dir == 0) {
			// this is the first direction to compare all others to
			dir = cur_dir;
		} else {
			// current direction must equal first direction
			if(cur_dir != dir) return false;
		}

		n = n->next;
	}
	return true;
}
