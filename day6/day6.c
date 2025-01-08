#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRINT_INT(x) printf("%s is %d\n", #x, x)
#define PRINT_CHAR(x) printf("%s is %c\n", #x, x)
#define PRINT_STRING(x) printf("%s is %s\n", #x, x)

#define INIT_SIZE 2

typedef enum Dir {
	NORTH,
	EAST,
	SOUTH,
	WEST
} Dir;

char *DIR_SYMBOL = "^>v<";

typedef struct NPC {
	int x;
	int y;
	int spawnX;
	int spawnY;
	Dir dir;
	bool onMap;
	bool inLoop;
} NPC;

typedef struct Map {
	bool ** walkable;
	char ** visited;
	int totalVisits;
	int xMax;
	int yMax;
	NPC *guard;
} Map;

bool isWalkable(Map *map, int x, int y);
void setWalkable(Map *map, int x, int y, bool w);
void setVisited(Map *map, int x, int y, char w);
void printMap(Map *map);
void moveGuard(Map *map);
void resetMap(Map *map);

int main() {

	FILE *file;
	int ch;

	int xsize = INIT_SIZE, ysize = INIT_SIZE;
	int readX = 0, readY = 0;

	Map *map = malloc(sizeof(Map));
	// create first row only to get xsize correct before making more rows
	map->walkable = malloc(ysize * sizeof(bool*));
	map->walkable[0] = malloc(xsize * sizeof(bool));
	map->visited = malloc(ysize * sizeof(char*));
	map->visited[0] = malloc(xsize * sizeof(char));
	map->totalVisits = 0;
	map->xMax = xsize;
	map->yMax = 1;
	map->guard = malloc(sizeof(NPC));
	map->guard->dir = NORTH;
	map->guard->onMap = true;
	map->guard->inLoop = false;

	// open the file for reading
	file = fopen("input.txt", "r");

	// check if it's open
	if(file == NULL) {
		printf("Error opening file.\n");
		exit(1);
	}

	// read each char from the file
	while((ch = fgetc(file)) != EOF) {
		// in a row
		if(ch == '.' || ch == '^' || ch == '#') {
			// if this is NPC start, set coords
			if(ch == '^') {
				map->guard->x = readX;
				map->guard->spawnX = readX;
				map->guard->y = readY;
				map->guard->spawnY = readY;
				// printf("guard here-> ");
			}

			// printf("set %d,%d walkable-> ", readX, readY);
			setWalkable(map, readX, readY, ch != '#');
			setVisited(map, readX, readY, false);
			readX++;
			// if this is first row, grow xMax
			if(readY == 0) map->xMax = readX;

		}

		// end of row, add a new row
		if(ch == '\n') {
			readY++;
			readX = 0;
			map->yMax = readY;
			map->walkable[readY] = malloc(xsize * sizeof(bool));
			map->visited[readY] = malloc(xsize * sizeof(char));
		}

		// check if rows need to grow, next write would fill
		if(readX >= xsize - 1) {
			xsize *= 2;
			map->walkable[readY] = realloc(map->walkable[readY], xsize * sizeof(bool));
			map->visited[readY] = realloc(map->visited[readY], xsize * sizeof(char));
		}

		// check if height needs to grow, next row will fill
		if(readY >= ysize - 1) {
			ysize *= 2;
			map->walkable = realloc(map->walkable, ysize * sizeof(bool*));
			map->visited = realloc(map->visited, ysize * sizeof(char*));
		}
	}

	PRINT_INT(map->xMax);
	PRINT_INT(xsize);

	setVisited(map, map->guard->spawnX, map->guard->spawnY, '^');

	printMap(map);

	while(map->guard->onMap) {
		moveGuard(map);
	}

	printMap(map);

	PRINT_INT(map->totalVisits);

	int loops = 0;
	for(int y = 0; y < map->yMax; y++) {
		for(int x = 0; x < map->xMax; x++) {
			if(y == map->guard->spawnY && x == map->guard->spawnX) continue;
			if(!isWalkable(map, x, y)) continue;
			resetMap(map);

			setWalkable(map, x, y, false);

			while(map->guard->onMap && !map->guard->inLoop) {
				moveGuard(map);
			}
			
			if(map->guard->inLoop) {
				loops++;
				// printMap(map);
			}

			setWalkable(map, x, y, true);
		}
	}

	PRINT_INT(loops);

}

bool isWalkable(Map *map, int x, int y) {
	return map->walkable[y][x];
}

void setWalkable(Map *map, int x, int y, bool w) {
	map->walkable[y][x] = w;
	// printf("%d,%d is walkable = %d\n", x, y, w);
	return;
}

bool isVisited(Map *map, int x, int y) {
	return map->visited[y][x];
}

void setVisited(Map *map, int x, int y, char w) {
	map->visited[y][x] = w;
	// printf("%d,%d is visited = %d\n", x, y, w);
	if(w) map->totalVisits++;
	return;
}

bool guardHere(Map *map, int x, int y) {
	return (map->guard->x == x && map->guard->y == y);
}

void printMap(Map *map) {
	for(int y = 0; y < map->yMax; y++) {
		for(int x = 0; x < map->xMax; x++) {
			if(map->guard->onMap && guardHere(map, x, y)) {
				printf("%c", DIR_SYMBOL[map->guard->dir]);
			} else if(isVisited(map, x, y)) {
				printf("%c", map->visited[y][x]);
			} else if(isWalkable(map, x, y)) {
				printf(".");
			} else {
				printf("#");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void testLoop(Map *map) {
	int x = map->guard->x, y = map->guard->y;
	if(!isVisited(map, x, y)) return;
	if(map->visited[y][x] == DIR_SYMBOL[map->guard->dir]) {
		map->guard->inLoop = true;
	}
}

void setDir(NPC *g, Dir d) {
	g->dir = d;
}

void setLoc(NPC *g, int x, int y) {
	g->x = x;
	g->y = y;
}

void moveGuard(Map *map) {
	int dx = map->guard->x, dy = map->guard->y;
	Dir dir = map->guard->dir;

	switch(dir) {
		case NORTH:
			dy += -1;
			break;
		case EAST:
			dx += 1;
			break;
		case SOUTH:
			dy += 1;
			break;
		case WEST:
			dx += -1;
			break;
	}

	if(dx < 0 || dy < 0 || dx >= map->xMax || dy >= map->yMax) {
		map->guard->onMap = false;
		return;
	}

	if(isWalkable(map, dx, dy)) {
		setLoc(map->guard, dx, dy);
		testLoop(map);
		if(!isVisited(map, dx, dy)){
			setVisited(map, dx, dy, DIR_SYMBOL[map->guard->dir]);
		}
		return;
	}

	setDir(map->guard, (map->guard->dir + 1) % 4); 
}

void resetMap(Map *map) {
	map->totalVisits = 0;
	map->guard->x = map->guard->spawnX;
	map->guard->y = map->guard->spawnY;
	map->guard->onMap = true;
	map->guard->inLoop = false;
	map->guard->dir = NORTH;

	for(int y = 0; y < map->yMax; y++) {
		for(int x = 0; x < map->xMax; x++) {
			setVisited(map, x, y, false);
		}
	}

	setVisited(map, map->guard->spawnX, map->guard->spawnY, true);
}
