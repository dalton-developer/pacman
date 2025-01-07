//
// Created by Dalton Gabriel Abambari on 26/12/24.
//

#include "pm_maps.h"
#include <stdlib.h>
#include <stdio.h>

static FILE* file = NULL;

static char** map = NULL;
static int numMaps = 0;
static int mapCounter = 0;

static int ysize = 0;
static int xsize = 0;
static int ghosts = 0;
static int smartGhosts = 0;

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
    map = NULL;  // this was missed in the other version - bug when no more maps and calling getMap()

    xsize = ysize = ghosts = smartGhosts = 0;
}

void closeMapsFile() {
    fclose(file);
    deleteMap();
    numMaps = 0;
    mapCounter = 0;
}

int numberOfMaps() {
    return numMaps;
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

int numGhosts() {
    return ghosts;
}

int numSmartGhosts() {
    return smartGhosts;
}

void printMap() {
    int i, j;

    if (map == NULL) {
        printf("NO MAP!");
        return;
    }

    for (j = ysize - 1; j >= 0; j--) {
        for (i = 0; i < xsize; i++)
            if (map[i][j] == 1)
                printf("O ");
            else
                printf("  ");

        printf("\n");
    }
}

void renderMap() {
    char** map = getMap();
    if (map == nullptr) {
        printf("No map loaded!\n");
        return;
    }

    int xSize = xTabSize();
    int ySize = yTabSize();

    glPushMatrix();  // Guardar la matriz actual

    // Renderizar cada celda del mapa
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            if (map[x][y] == 0) {
                // Obstáculo: Renderizar un cubo en esta posición
                glPushMatrix();
                glTranslatef((float)x, (float)y, 0.0f);  // Mover el cubo a la posición
                glColor3f(1.0f, 0.0f, 0.0f);             // Color rojo para obstáculos
                glutSolidCube(1.0f);                     // Cubo de tamaño 1x1x1
                glPopMatrix();
            } else {
                // Celda libre: Opcionalmente, renderizar un plano o dejar vacío
                glPushMatrix();
                glTranslatef((float)x, (float)y, 0.0f);
                glColor3f(0.0f, 0.5f, 0.0f);             // Color verde para celdas libres
                glutSolidCube(0.2f);                     // Cubo pequeño o plano
                glPopMatrix();
            }
        }
    }

    glPopMatrix();  // Restaurar la matriz original
}


// Render each individual cell as a cube in 3D space
void renderCell(int x, int y, int size) {
    glPushMatrix();
    glTranslatef(x * size, 0, y * size);  // Move to the correct position
    glColor3f(0.8f, 0.8f, 0.8f);  // Set the color for the walls or cells
    glutSolidCube(size);  // Draw the cube
    glPopMatrix();
}
