#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define TABLE_SIZE 50

typedef struct Node {
	int number;
	int times;
	struct Node *next;
} Node;

typedef struct {
	Node *table[50];
} HashTable;

void print_list(int *l, int *r, int size);
void insert_sort(int *l, int v, int size);
void count_entry(int r, HashTable *htable);
int hash(int num);
int find(int num, HashTable *htable);

int main() {
	FILE *file;
	char line[256];
	char *left_word, *right_word;
	int size, left, right, *left_list, *right_list, used, sum;

	HashTable *htable = malloc(sizeof(HashTable));
	for(int i = 0; i < TABLE_SIZE; i++) {
		htable->table[i] = NULL;
	}

	size = INITIAL_SIZE;
	used = 0;

	left_list = malloc(size * sizeof(int));
	right_list = malloc(size * sizeof(int));

	// check pointers
	if(left_list == NULL || right_list == NULL) {
		printf("error allocating lists\n");
		exit(1);
	}

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each line from the file
	while(fgets(line, sizeof(line), file)) {
		// split into words
		left_word = strtok(line, " ");
		right_word = strtok(NULL, " ");

		// convert to ints
		left = atoi(left_word);
		right = atoi(right_word);

		// grow list if it is full
		if(used >= size) {
			size *= 2;
			left_list = realloc(left_list, size * sizeof(int));
			right_list = realloc(right_list, size * sizeof(int));
			if(left_list == NULL || right_list == NULL) {
				printf("Error resizing list\n");
				exit(1);
			}
		}

		// insert into list
		insert_sort(left_list, left, used);
		insert_sort(right_list, right, used);

		count_entry(right, htable);

		used++;
	}

	// print_list(left_list, right_list, used);

	sum = 0;
	for(int i = 0; i < used; i++) {
		sum += abs(*(left_list+i) - *(right_list+i));
	}

	printf("the sum of the distances is %d\n", sum);

	printf("the smallest numbers are %d %d\n", *left_list, *right_list);

	int sim = 0;

	for(int i = 0; i < used; i++) {
		int num = left_list[i];
		int h = hash(num);
		int dsim = num * find(num, htable);
		sim += dsim;
	}

	printf("similarity is %d\n", sim);

	// close the FILE
	fclose(file);
	free(left_list);
	free(right_list);

	return 0;
}

void print_list(int *l, int *r, int size) {
	for(int i = 0; i < size; i++) {
		printf("L: %d,   R: %d\n", *(l+i), *(r+i));
	}
}

void insert_sort(int *l, int v, int size) {
	// find place to insert
	int i = 0;
	while(i < size && v > *(l+i)) {
		i++;
	}

	// save in new value
	int curr = *(l+i);
	*(l+i) = v;

	// shift remaining, if any
	while(i < size) {
		i++;
		int next = *(l+i);
		*(l+i) = curr;
		curr = next;
	}
}

void count_entry(int r, HashTable *htable) {
	Node *curr, *prev;
	int h = hash(r);

	curr = htable->table[h];
	while(curr != NULL && curr->number != r) {
		prev = curr;
		curr = curr->next;
	}

	if(curr == NULL) {
		curr = malloc(sizeof(Node));
		curr->number = r;
		curr->times = 0;
		curr->next = NULL;
		if(htable->table[h] == NULL) {
			htable->table[h] = curr;
		} else {
			prev->next = curr;
		}
		// printf("added new number to %d: %d\n", h, curr->number);
	}

	curr->times += 1;
	// printf("%d times is %d\n", r, curr->times);
}

int hash(int num) {
	int sum = 0;
	while(num != 0) {
		sum += num % 10;
		num /= 10;
	}
	return sum%TABLE_SIZE;
}

int find(int num, HashTable *htable) {
	Node *curr = htable->table[hash(num)];
	while(curr != NULL) {
		if(curr->number == num) {
			return curr->times;
		}
		curr = curr->next;
	}
	return 0;
}
