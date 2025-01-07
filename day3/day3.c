#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

typedef enum {
	NUMBER,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	COMMA,
	MULT,
	GARBAGE,
	ALPHA,
	MULSTMT,
	APOS,
	DONT,
	DO,
} TokenType;

const char *tokentypes[] = {
	"NUMBER",
	"OPEN_BRACKET",
	"CLOSE_BRACKET",
	"COMMA",
	"MULT",
	"GARBAGE",
	"ALPHA",
	"MULSTMT",
	"APOS",
	"DONT",
	"DO"
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
void findMul(Node *top);
void removeNext(Node *curr);
void combineNum(Node *top);
void combineGarb(Node *top);
void evalMult(Node *top);
void evalDont(Node *top);
void evalDo(Node *top);


int main() {
	FILE *file;
	int ch;
	Node *top = malloc(sizeof(Node));
	Node *last = top;

	top->next = NULL;
	top->token = NULL;

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((ch = fgetc(file)) != EOF) {
		if(ch != EOF && ch >= 'a' && ch <= 'z') {
			// if ch is a lower case letter, add an alpha token
			// PRINT_CHAR(ch);

			addToken(ALPHA, ch, last);
			last = last->next;

			continue;
		}

		if(ch != EOF && ch >= '0' && ch <= '9') {
			// if ch is a number add a number token
			// PRINT_CHAR(ch);
			addToken(NUMBER, ch-'0', last);
			last = last->next;

			continue;
		}

		// whitespace is also GARBAGE
		if(isspace(ch)) {
			addToken(GARBAGE, 0, last);
			last = last->next;

			continue;
		}

		if(ch == '(') {
			addToken(OPEN_BRACKET, 0, last);
			last = last->next;

			continue;
		}

		if(ch == ')') {
			addToken(CLOSE_BRACKET, 0, last);
			last = last->next;

			continue;
		}

		if(ch == ',') {
			addToken(COMMA, 0, last);
			last = last->next;

			continue;
		}

		if(ch == '\'') {
			addToken(APOS, 0, last);
			last = last->next;

			continue;
		}

		// anything else is garbage
		addToken(GARBAGE, 0, last);
		last = last->next;
	}

	// printf("\n");
	// printTokens(top);
	// printf("\n");

	findMul(top);
	combineNum(top);
	combineGarb(top);
	evalMult(top);
	evalDont(top);
	evalDo(top);

	// printf("\n");
	// printTokens(top);
	// printf("\n");

	int sum = 0;
	bool do_mul = true;
	Node *p = top->next;
	while(p != NULL) {
		if(p->token->kind == MULSTMT && do_mul) {
			sum += p->token->value;
		}
		if(p->token->kind == DO) {
			do_mul = true;
		}
		if(p->token->kind == DONT) {
			do_mul = false;
		}
		p = p->next;
	}

	PRINT_INT(sum);

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
		printf("type: %s", tokentypes[n->token->kind]);
		if(n->token->kind == ALPHA) {
			printf(", Value: %c", n->token->value);
		} else if(n->token->kind == NUMBER || n->token->kind == MULSTMT) {
			printf(", Value: %d", n->token->value);
		}
		printf(" -> ");
	}
	printf("NULL\n");
}

void findMul(Node *top) {
	Node *posM = top->next, *posU, *posL;
	while(posM != NULL) {
		// check the next two nodes are not NULL before accessing fields
		posU = posM->next;
		if(posU == NULL) return;
		posL = posU->next;
		if(posL == NULL) return;

		// all 3 are real Nodes, now can check the values
		if(posM->token->value == 'm' &&
				posU->token->value == 'u' &&
				posL->token->value == 'l') {
			// found 'm'->'u'->'l' so combine into one MULT token
			posM->token->kind = MULT;
			posM->token->value = 0;

			// free unused 'u' and 'l' nodes
			removeNext(posM);
			removeNext(posM);
		}

		posM = posM->next;
	}
}

void removeNext(Node *curr) {
	Node *next = curr->next;
	curr->next = next->next;
	free(next->token);
	free(next);
}

void combineNum(Node *top) {
	Node *pNumTen = top->next, *pNumOne;
	while(pNumTen != NULL) {
		// check we are not at the end
		pNumOne = pNumTen->next;
		if(pNumOne == NULL) return;

		// we have two tokens, check if both are numbers
		if(pNumTen->token->kind == NUMBER && pNumOne->token->kind == NUMBER) {
			// combine the numbers
			pNumTen->token->value *= 10;
			pNumTen->token->value += pNumOne->token->value;
			removeNext(pNumTen);
			// do not increment position of pNumTen here so we can check if the 
			// next Node is a number again
		} else {
			// one of the two is not a number, so nothing to combine
			// increment position of pNumTen
			pNumTen = pNumTen->next;
		}
	}
}

void combineGarb(Node *top) {
	Node *pGarb1 = top->next, *pGarb2;
	while(pGarb1 != NULL) {
		// check we are not at the end
		pGarb2 = pGarb1->next;
		if(pGarb2 == NULL) return;

		// we have two tokens, check if both are garbage
		if(pGarb1->token->kind == GARBAGE && pGarb2->token->kind == GARBAGE) {
			// combine the garbage
			removeNext(pGarb1);
			// do not increment position so we can test the next node
		} else {
			// no combine to do, so increment position
			pGarb1 = pGarb1->next;
		}
	}
}

void evalMult(Node *top) {
	Node *pMul = top, *pOB, *pn1, *pn2, *pCom, *pCB;

	while(pMul->next != NULL) {
		pMul = pMul->next;
		
		// test for "mul"
		if(pMul->token->kind != MULT) continue;

		// test if next node exists and is '('
		if(pMul->next == NULL) return;
		pOB = pMul->next;
		if(pOB->token->kind != OPEN_BRACKET) continue;

		// test if next node exists and is a number
		if(pOB->next == NULL) return;
		pn1 = pOB->next;
		if(pn1->token->kind != NUMBER) continue;

		// test if next node exists and is a COMMA
		if(pn1->next == NULL) return;
		pCom = pn1->next;
		if(pCom->token->kind != COMMA) continue;

		// test if next node exists and is a number
		if(pCom->next == NULL) return;
		pn2 = pCom->next;
		if(pn2->token->kind != NUMBER) continue;

		// test if next node exists and is a ')'
		if(pn2->next == NULL) return;
		pCB = pn2->next;
		if(pCB->token->kind != CLOSE_BRACKET) continue;

		// if we get to here we have a proper mul(n,m) statement
		pMul->token->kind = MULSTMT;
		pMul->token->value = pn1->token->value * pn2->token->value;

		// remove unused nodes
		for(int i = 0; i < 5; i++) {
			// remove 5 nodes (pOB, pn1, pn2, pCom, pCB)
			removeNext(pMul);
		}
	}
}

void evalDont(Node *top) {
	Node *posD = top, *posO, *posN, *pApos, *posT, *pOB, *pCB;
	while(posD->next != NULL) {
		posD = posD->next;
		
		// test for 'd'
		if(posD->token->kind != ALPHA || posD->token->value != 'd') continue;

		// test if next node exists and is 'o'
		if(posD->next == NULL) return;
		posO = posD->next;
		if(posO->token->kind != ALPHA || posO->token->value != 'o') continue;

		// test if next node exists and is 'n'
		if(posO->next == NULL) return;
		posN = posO->next;
		if(posN->token->kind != ALPHA || posN->token->value != 'n') continue;

		// test if next node exists and is a APOS
		if(posN->next == NULL) return;
		pApos = posN->next;
		if(pApos->token->kind != APOS) continue;

		// test if next node exists and is a 't'
		if(pApos->next == NULL) return;
		posT = pApos->next;
		if(posT->token->kind != ALPHA || posT->token->value != 't') continue;

		// test if next node exists and is a '('
		if(posT->next == NULL) return;
		pOB = posT->next;
		if(pOB->token->kind != OPEN_BRACKET) continue;

		// test if next node exists and is a ')'
		if(pOB->next == NULL) return;
		pCB = pOB->next;
		if(pCB->token->kind != CLOSE_BRACKET) continue;

		// if we get to here we have a proper don't() statement
		posD->token->kind = DONT;
		posD->token->value = 0;

		// remove unused nodes
		for(int i = 0; i < 6; i++) {
			// remove 6 nodes (O, N, ', T, (, ))
			removeNext(posD);
		}
	}
}

void evalDo(Node *top) {
	Node *posD = top, *posO, *pOB, *pCB;
	while(posD->next != NULL) {
		posD = posD->next;
		
		// test for 'd'
		if(posD->token->kind != ALPHA || posD->token->value != 'd') continue;

		// test if next node exists and is 'o'
		if(posD->next == NULL) return;
		posO = posD->next;
		if(posO->token->kind != ALPHA || posO->token->value != 'o') continue;

		// test if next node exists and is a '('
		if(posO->next == NULL) return;
		pOB = posO->next;
		if(pOB->token->kind != OPEN_BRACKET) continue;

		// test if next node exists and is a ')'
		if(pOB->next == NULL) return;
		pCB = pOB->next;
		if(pCB->token->kind != CLOSE_BRACKET) continue;

		// if we get to here we have a proper don't() statement
		posD->token->kind = DO;
		posD->token->value = 0;

		// remove unused nodes
		for(int i = 0; i < 3; i++) {
			// remove 3 nodes (O, (, ))
			removeNext(posD);
		}
	}
}
