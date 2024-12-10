#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)

int main() {
	FILE *file;
	char line[256];
	char *word;
	int prev, curr, safe = 0, dir = 0;

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each line from the file
	while(fgets(line, sizeof(line), file)) {
		dir = 0;

		// split into words
		word = strtok(line, " ");

		// convert to ints
		curr = atoi(word);

		while(word != NULL) {
			word = strtok(NULL, " ");

			// test if we are at the end
			if(word == NULL) break;
			
			prev = curr;
			curr = atoi(word);

			// test if no change
			if(curr == prev) break;

			// test if jump too large
			if(abs(curr - prev) > 3) break;

			// if no direction yet, save current direction
			if(dir == 0) {
				dir = (curr-prev) / abs(curr-prev);
			} else // otherwise test direction is the same
			{
				if(dir != (curr-prev)/abs(curr-prev)) break;
			}

		}

		// if word is NULL we made it to the end of the line without 
		// failing any tests
		if(word == NULL) {
			safe += 1;
		}

	}

	PRINT_INT(safe);
}
