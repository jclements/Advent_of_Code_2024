#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

typedef struct rule {
	int first;
	int last;
} rule;

typedef struct update {
	int *pages;
	int size;
	int middle;
	struct update *next;
} update;

typedef struct ruleNode {
	rule *val;
	struct ruleNode *next;
} ruleNode;

void printRules(ruleNode *n);
void printPages(update *u);
void printUpdates(update *u);
void addPage(int p, update *u);
bool contains(int n, int *a, int size);
bool checkUp(update *u, rule *r);
bool checkRules(update *u, ruleNode *r);
int fixUpdate(update *u, ruleNode *r);

int main() {

	FILE *file;
	int ch;
	bool doneRules = false;
	ruleNode *rules = NULL;
	ruleNode *last = NULL;
	update *updates = NULL;
	update *lastUp = NULL;
	int answer = 0, fixAns = 0;

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((ch = fgetc(file)) != EOF) {
		if(!doneRules) {
			// somewhere in rules at beginning of a line
			if(ch == '\n') {
				// the only character in the line is a newline, time for updates
				doneRules = true;
				continue;
			}
			char *word = malloc(6);
			word[5] = 0;
			int i = 0;
			while(ch != '\n') {
				*(word + i++) = ch;
				ch = fgetc(file);
			}
			rule *newRule = malloc(sizeof(rule));
			newRule->first = (word[0] - '0') * 10 + (word[1] - '0');
			newRule->last = (word[3] - '0') * 10 + (word[4] - '0');

			if(rules == NULL) {
				rules = malloc(sizeof(ruleNode));
				rules->val = newRule;
				rules->next = NULL;
				last = rules;
			} else {
				ruleNode *newNode = malloc(sizeof(ruleNode));
				newNode->val = newRule;
				last->next = newNode;
				last = newNode;
			}
		} else {
			// done rules, process updates now
			// we are at the start of a new update
			char *word = malloc(3);
			update *newUpdate = malloc(sizeof(update));
			newUpdate->next = NULL;
			newUpdate->size = 0;

			while(ch != '\n') {
				// if ch is a comma we are building the existing update
				if(ch == ',') {
					ch = fgetc(file);
				}
				int page = 0;
				page += (ch - '0') * 10;
				ch = fgetc(file);
				page += ch - '0';
				
				addPage(page, newUpdate);
				

				// grab comma or newline
				ch = fgetc(file);
			}
			// done a whole update
			newUpdate->middle = newUpdate->pages[newUpdate->size/2];
			if(checkRules(newUpdate, rules)) {
				answer += newUpdate->middle;
			} else {
				fixAns += fixUpdate(newUpdate, rules);
			}
				
			if(updates == NULL) {
				updates = newUpdate;
				lastUp = newUpdate;
			} else {
				lastUp->next = newUpdate;
				lastUp = newUpdate;
			}
		} 
	}

	// printRules(rules);
	// printUpdates(updates);
	PRINT_INT(answer);
	PRINT_INT(fixAns);

}

void printRules(ruleNode *n) {
	while(n != NULL) {
		printf("%d|%d\n", n->val->first, n->val->last);
		n = n->next;
	}
}

void addPage(int p, update *u) {
	if(u->size == 0) {
		u->pages = malloc(sizeof(int));
		u->size++;
	} else {
		u->size++;
		u->pages = realloc(u->pages, sizeof(int) * u->size);
	}
	u->pages[u->size - 1] = p;
}
void printArray(int *a, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d,", a[i]);
	}
}

void printPages(update *u) {
	printArray(u->pages, u->size);
	printf(" middle is %d\n", u->middle);
}

void printUpdates(update *u) {
	while(u != NULL) {
		printPages(u);
		u = u->next;
	}
}

bool contains(int n, int *a, int size) {
	for(int i = 0; i < size; i ++) {
		if(n == a[i]) return true;
	}
	return false;
}

bool checkUp(update *u, rule *r) {
	if(!contains(r->first, u->pages, u->size)) return true;
	if(!contains(r->last, u->pages, u->size)) return true;
	for(int i = 0; i < u->size; i++) {
		if(u->pages[i] == r->first) return true;
		if(u->pages[i] == r->last) return false;
	}
	return true;
}

bool checkRules(update *u, ruleNode *r) {
	while(r != NULL) {
		if(!checkUp(u, r->val)) return false;
		r = r->next;
	}
	return true;
}

void swapPlaces(update *u, rule *r) {
	for(int i = 0; i < u->size; i++) {
		if(u->pages[i] == r->first) u->pages[i] = r->last;
		else if(u->pages[i] == r->last) u->pages[i] = r->first;
	}
}

int fixUpdate(update *u, ruleNode *r) {
	int size = u->size;
	int *a = malloc(sizeof(int) * size);
	int newMid;

	for(int i = 0; i < size; i ++) {
		a[i] = u->pages[i];
	}

	// make fake update to test against rules
	update *fakeUp = malloc(sizeof(update));
	fakeUp->size = size;
	fakeUp->pages = a;

	ruleNode *riter = r;

	while(!checkRules(fakeUp, r)) {
		while(riter != NULL) {
			if(!checkUp(fakeUp, riter->val)) swapPlaces(fakeUp, riter->val);
			riter = riter->next;
		}
		riter = r;
	}

	// PRINT_INT(checkRules(fakeUp, r));

	newMid = fakeUp->pages[size/2];
	// PRINT_INT(newMid);
	return newMid;
}
