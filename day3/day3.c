#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

typedef enum {
	NUMBER,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	COMMA,
	MULT,
	GARBAGE,
	ALPHA,
} TokenType;

const char *tokentypes[] = {
	"NUMBER",
	"OPEN_BRACKET",
	"CLOSE_BRACKET",
	"COMMA",
	"MULT",
	"GARBAGE",
	"ALPHA"
};


typedef struct Token {
	TokenType kind;
	int value;
} Token;

typedef struct Node {
	Token *token;
	struct Node *next;
} Node;

bool streq(char *s1, char *s2);
bool strcont(char *s1, char *s2);
void addToken(TokenType t, int v, Node *n);
void printTokens(Node *n);


int main() {
	FILE *file;
	int ch;
	char *buff = malloc(256);
	Node *top = malloc(sizeof(Node));
	Node *last = top;

	*buff = 0;
	PRINT_STRING(buff);

	top->next = NULL;
	top->token = NULL;

	// open the file for reading
	file = fopen("example.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((ch = fgetc(file)) != EOF) {
		while(ch != EOF && ch >= 'a' && ch <= 'z') {
			// if next char is a lowercase letter, add it onto the string buffer
			PRINT_INT(ch);

			// make a null terminated string out of the char
			char *s = malloc(2);
			*s = ch;
			*(s+1) = 0;
			PRINT_STRING(s);

			// concat onto the buffer
			buff = strncat(buff, s, 1);
			PRINT_STRING(buff);

			if(streq(buff, "mul")) {
				// finished the operator, add MULT token and flushbuffer
				addToken(MULT, 0, last);
				last = last->next;
				*buff = 0;
			} else if(!strcont(buff, "mul")) {
				// the current state of buffer isn't part of operator
				*buff = 0;
			}
		}

		// buffer is not empty if we just ran out of letters without 
		// finishing an operator
		if(*buff != 0) {
			// add GARBAGE token
			addToken(GARBAGE, 0, last);
			last = last->next;
			PRINT_STRING(buff);
			*buff = 0;
		}

		/**
		while(ch != EOF && ch >= '0' && ch <= '9') {
			// if next char is a number, add it onto the string buffer
			PRINT_INT(ch);

			// make a null terminated string out of the char
			char *s = malloc(2);
			*s = ch;
			*(s+1) = 0;
			PRINT_STRING(s);

			// concat onto the buffer
			buff = strncat(buff, s, 1);
			PRINT_STRING(buff);
			ch = fgetc(file);

			// if ch isn't number we have the whole number in the buffer
			if(ch)
		}**/

		// whitespace is also GARBAGE
		if(isspace(ch)) {
			addToken(GARBAGE, 0, last);
			last = last->next;
		}

		if(ch == '(') {
			addToken(OPEN_BRACKET, 0, last);
			last = last->next;
		}

		if(ch == ')') {
			addToken(CLOSE_BRACKET, 0, last);
			last = last->next;
		}

		if(ch == ',') {
			addToken(COMMA, 0, last);
			last = last->next;
		}
	}

	printTokens(top);
	printf("\n");

	fclose(file);

}

bool streq(char *s1, char *s2) {
	return strcmp(s1, s2) == 0;
}

bool strcont(char *s1, char *s2) {
	while(*s1 != 0 && *s2 != 0) {
		if(*s1 != *s2) {
			return false;
		}
		s1 += 1;
		s2 += 1;
	}
	return true;
}

void addToken(TokenType t, int v, Node *n) {
	Node *newNode = malloc(sizeof(Node));
	newNode->token = malloc(sizeof(Token));

	newNode->token->kind = t;
	newNode->token->value = v;
	newNode->next = NULL;

	n->next = newNode;
}

void printTokens(Node *n) {
	while(n->next != NULL) {
		n = n->next;
		printf("type: %s, Value: %d->", tokentypes[n->token->kind], n->token->value);
	}
	printf("NULL\n");
}
