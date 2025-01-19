//
// Created by Dalton Gabriel Abambari on 26/12/24.
//

#include "pm_maps.h"
#include <cstdlib>
#include <cstdio>

static FILE* file = NULL;

static char** map = NULL;
static int numMaps = 0;
static int mapCounter = 0;

static int ysize = 0;
static int xsize = 0;
static int ghosts = 0;
static int smartGhosts = 0;

//var in main.c
extern int visitedCount;
extern int totalVisitable;

void openMapsFile(char* name) {
    file = fopen(name, "r");

    if (file == NULL) {
        printf("ERROR opening PAC-MAN Maps file.\n%s file does not exist!\n", name);
        exit(1);
    }

    fscanf(file, "%i", &numMaps);
}

void deleteMap() {
    int i;
    if (map != NULL) {
        for (i = 0; i < xsize; i++)
            free(map[i]);
        free(map);
    }
    map = NULL;

    xsize = ysize = ghosts = smartGhosts = 0;
}

void closeMapsFile() {
    fclose(file);
    deleteMap();
    numMaps = 0;
    mapCounter = 0;
}

void resetMapCounter() {
    mapCounter = 0; // Restart map counter
}



char** nextMap() {
    int i, j;
    static char buffer[4096];

    deleteMap();

    if (mapCounter >= numMaps) return NULL;

    fscanf(file, "%i%i%i%i ", &ghosts, &smartGhosts, &xsize, &ysize);

    map = (char**)malloc(sizeof(char*) * xsize);
    for (i = 0; i < xsize; i++)
        map[i] = (char*)malloc(sizeof(char) * ysize);

    for (j = ysize - 1; j >= 0; j--) {
        fscanf(file, "%s ", buffer);
        for (i = 0; i < xsize; i++) {
            map[i][j] = buffer[i] - '0';
        }
    }

    totalVisitable = 0;
    visitedCount = 0;

    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (map[i][j] == 1) {
                totalVisitable++;
            }
        }
    }
    mapCounter++;
    return map;
}

char** getMap() {
    return map;
}

int yTabSize() {
    return ysize;
}

int xTabSize() {
    return xsize;
}

void renderMap() {
    char** map = getMap();
    if (!map) {
        printf("No map loaded!\n");
        exit(EXIT_FAILURE); // End program if ther is not maps available
    }

    int xSize = xTabSize();
    int ySize = yTabSize();

    glPushMatrix();

    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            glPushMatrix();
            glTranslatef((float)x - xSize / 2.0f, (float)y - ySize / 2.0f, 0.0f);

            // Made them thin
            glScalef(1.0f, 1.0f, 0.2f);

            if (map[x][y] == 0) {
                glColor3f(0.0f, 0.0f, 0.2f);
            } else if (map[x][y] == 1) {
                glColor4f(0.1f, 1.0f, 1.0f, 0.4f);
            } else if (map[x][y] == 2) {
                glColor3f(1.0f, 1.0f, 0.0f);
            }
            glutSolidCube(1.0f);

            glPopMatrix();
        }
    }

    glPopMatrix();
}




