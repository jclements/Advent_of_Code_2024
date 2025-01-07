#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct node {
	struct node *up;
	struct node *down;
	struct node *left;
	struct node *right;
	struct node *row;
	char val;
} node;

node *newNode(char val);
void printGrid(node *n);
int checkGrid(node *n);
int checkNode(node *n);

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

int main() {

	FILE *file;
	int ch;

	node *top = NULL;
	node *prev = top;

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((ch = fgetc(file)) != EOF) {
		if(ch != '\n') {
			node *new = newNode(ch);

			if(top == NULL) {
				top = new;
				new->row = new;
			} else {
				prev->right = new;
				new->left = prev;
				new->row = prev->row;
			}

			if(prev != NULL && prev->up != NULL) {
				node *above = prev->up->right;
				new->up = above;
				above->down = new;
			}

			prev = new;

		} else {
			ch = fgetc(file);
			node *new = newNode(ch);
			node *above = prev->row;
			above->down = new;
			new->up = above;
			new->row = new;
			prev = new;
		}
	}

	printGrid(top);

	printf("\n");

	PRINT_INT(checkGrid(top));

	printf("\n");
}

node *newNode(char val) {
	node *n = malloc(sizeof(node));

	n->val = val;
	n->up = NULL;
	n->down = NULL;
	n->right = NULL;
	n->left = NULL;
	n->row = NULL;

	return n;
}

void printGrid(node *n) {
	bool running = true;
	while(running) {
		printf("%c ", n->val);
		if(n->right == NULL) {
			// end of row or end of grid
			if(n->down == NULL) return; // end of grid
			
			printf("\n");
			n = n->row->down;	// go to start of next row
		} else {
			// go to next node
			n = n->right;
		}
	}
}

int checkGrid(node *n) {
	int num = 0;
	
	bool running = true;
	while(running) {
		num += checkNode(n);
		if(n->right == NULL) {
			// end of row or end of grid
			if(n->down == NULL) {
				running = false; // end of grid
			} else {		
				n = n->row->down;	// go to start of next row
			}
		} else {
			// go to next node
			n = n->right;
		}
	}

	return num;
}

int checkUR(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->up == NULL) return 0;
	if(pX->up->right == NULL) return 0;

	pM = pX->up->right;
	if(pM->val != 'M') return 0;
	if(pM->up == NULL) return 0;
	if(pM->up->right == NULL) return 0;

	pA = pM->up->right;
	if(pA->val != 'A') return 0;
	if(pA->up == NULL) return 0;
	if(pA->up->right == NULL) return 0;

	pS = pA->up->right;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkUL(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->up == NULL) return 0;
	if(pX->up->left == NULL) return 0;

	pM = pX->up->left;
	if(pM->val != 'M') return 0;
	if(pM->up == NULL) return 0;
	if(pM->up->left == NULL) return 0;

	pA = pM->up->left;
	if(pA->val != 'A') return 0;
	if(pA->up == NULL) return 0;
	if(pA->up->left == NULL) return 0;

	pS = pA->up->left;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkDL(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->down == NULL) return 0;
	if(pX->down->left == NULL) return 0;

	pM = pX->down->left;
	if(pM->val != 'M') return 0;
	if(pM->down == NULL) return 0;
	if(pM->down->left == NULL) return 0;

	pA = pM->down->left;
	if(pA->val != 'A') return 0;
	if(pA->down == NULL) return 0;
	if(pA->down->left == NULL) return 0;

	pS = pA->down->left;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkDR(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->down == NULL) return 0;
	if(pX->down->right == NULL) return 0;

	pM = pX->down->right;
	if(pM->val != 'M') return 0;
	if(pM->down == NULL) return 0;
	if(pM->down->right == NULL) return 0;

	pA = pM->down->right;
	if(pA->val != 'A') return 0;
	if(pA->down == NULL) return 0;
	if(pA->down->right == NULL) return 0;

	pS = pA->down->right;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkUp(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->up == NULL) return 0;

	pM = pX->up;
	if(pM->val != 'M') return 0;
	if(pM->up == NULL) return 0;

	pA = pM->up;
	if(pA->val != 'A') return 0;
	if(pA->up == NULL) return 0;

	pS = pA->up;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkDown(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->down == NULL) return 0;

	pM = pX->down;
	if(pM->val != 'M') return 0;
	if(pM->down == NULL) return 0;

	pA = pM->down;
	if(pA->val != 'A') return 0;
	if(pA->down == NULL) return 0;

	pS = pA->down;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkLeft(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->left == NULL) return 0;

	pM = pX->left;
	if(pM->val != 'M') return 0;
	if(pM->left == NULL) return 0;

	pA = pM->left;
	if(pA->val != 'A') return 0;
	if(pA->left == NULL) return 0;

	pS = pA->left;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkRight(node *n) {
	node *pX, *pM, *pA, *pS;
	pX = n;
	if(pX->val != 'X') return 0;
	if(pX->right == NULL) return 0;

	pM = pX->right;
	if(pM->val != 'M') return 0;
	if(pM->right == NULL) return 0;

	pA = pM->right;
	if(pA->val != 'A') return 0;
	if(pA->right == NULL) return 0;

	pS = pA->right;
	if(pS->val != 'S') return 0;

	return 1;
}

int checkNode(node *n) {
	return checkUp(n) + checkDown(n) + checkLeft(n) + checkRight(n)
				+ checkUR(n) + checkUL(n) + checkDR(n) + checkDL(n);
}