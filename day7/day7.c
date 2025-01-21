#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

typedef struct equation {
	unsigned long long int value;
	int *numbers;
	int size;
	
	int *correct;
	int num_correct;

	struct equation *next;
} equation;

char *getLine(FILE *file);
void printEq(equation *eq);
equation *newEq();
bool testEq(equation *eq);
bool testEq3(equation *eq);

int main() {

	FILE *file;
	int ch;
	char *line;
	char *token;

	equation *top = newEq();

	equation *curr = top;

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
		token = strtok(line, ":");
		char *endp;
		curr->value = strtol(token, &endp, 10);

		if(*endp != '\0') {
			printf("error converting value to long int\n");
			return -1;
		}

		int len = 2;
		curr->numbers = malloc(sizeof(int) * len);

		while((token = strtok(NULL, " "))) {
			// resize if full
			if(curr->size + 1 >= len) {
				len *= 2;
				curr-> numbers = realloc(curr->numbers, sizeof(int) * len);
			}

			// save current number at end of array
			curr->numbers[curr->size++] = atoi(token);
		}

		// done a line, was it empty? then no new node needed
		if(curr->size == 0) continue;

		curr->next = newEq();
		curr = curr->next;
	}

	unsigned long long int sum = 0;
	unsigned long long int sum3 = 0;
	equation *e = top;
	while(e->size > 0) {
		printEq(e);
		if(testEq(e)) {
			sum += e->value;
		}
		if(testEq3(e)) {
			sum3 += e->value;
		}
		e = e->next;
	}

	printf("sum = %lld\n", sum);
	printf("sum3 = %lld\n", sum3);
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

void printEq(equation *eq) {
	printf("Value: %lld Numbers: ", eq->value);
	for(int i = 0; i < eq->size; i++) {
		printf("%d ", *(eq->numbers + i));
	}
	printf("\n");
}

equation *newEq() {
	equation *new = malloc(sizeof(equation));

	if(new == NULL) return NULL;

	new->next = NULL;
	new->size = 0;
	new->numbers = NULL;
	new->correct = NULL;
	new->num_correct = 0;

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
