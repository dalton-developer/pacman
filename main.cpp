#include <GL/glut.h>
#include <cstdio>
#include "pm_maps.h"

// Angles for the movement
float limbAngle = 0.0f;
bool limbDirection = true;
float pacmanAngle = 0.0f; // Angle for pacman rotation
bool isPaused = false;  //Check if the game is paused


int visitedCount = 0; // Counter
int totalVisitable = 0; //Total


int pacmanX = 0, pacmanY = 0;


int ghost1X = 0, ghost1Y = 5; // Random
int ghost2X = 5, ghost2Y = 0; // Search

int windowWidth = 800;
int windowHeight = 600;

// control camera and mouse interaction
float cameraX = 0.0f, cameraY = -6.0f, cameraZ = 11.0f;
float lastMouseX = 0.0f, lastMouseY = 0.0f;
bool isDragging = false;


void initOpenGL() {
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void renderPacman() {
    glPushMatrix();

    glTranslatef(pacmanX - xTabSize() / 2.0f, pacmanY - yTabSize() / 2.0f, 0.5f);

    glRotatef(pacmanAngle, 0.0f, 0.0f, 1.0f);

    // Escaling to make him smoler
    glScalef(0.5f, 0.5f, 0.5f);

    //Arms anim
    limbAngle += limbDirection ? 2.0f : -2.0f;
    if (limbAngle > 30.0f) limbDirection = false;
    if (limbAngle < -30.0f) limbDirection = true;

    //Main body
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glScalef(1.0f, 1.0f, 1.5f);
    glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();
    //Eyes and mouth
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(-0.2f, 0.4f, 0.7f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.2f, 0.4f, 0.7f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.5f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.75f);
    glRotatef(-80.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.1f, 0.3f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(-0.7f, 0.0f, 0.4f);
    glRotatef(limbAngle, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.7f, 0.1f); // Alargada
    glutSolidSphere(0.5f, 20, 20);

    // Shield
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f);
    glTranslatef(0.0f, 0.7f, 0.0f);
    glScalef(3.0f, 0.5f, 0.1f);
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix(); // Sword
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.7f, 0.0f, 0.4f);
    glRotatef(-limbAngle, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.7f, 0.1f);
    glutSolidSphere(0.5f, 20, 20);

    // Sword edge
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.8f);
    glTranslatef(0.0f, 0.8f, 0.0f);
    glScalef(0.4f, 2.0f, 0.1f);
    glutSolidCube(0.9f);
    glPopMatrix();

    //Sword handler
    glPushMatrix();
    glColor3f(0.4f, 0.2f, 0.1f); // Marrón oscuro
    glTranslatef(0.0f, 0.55f, 0.0f); // Justo debajo de la hoja
    glScalef(0.25f, 0.5f, 0.15f); // Más gruesa
    glutSolidCube(0.5f);
    glPopMatrix();

    glPopMatrix();

    //tail
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f); // Gris
    glTranslatef(0.0f, -0.7f, 0.3f); // Detrás del cuerpo
    glScalef(0.3f, 0.3f, 0.3f);
    glutSolidSphere(0.4f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

//render ghosts
void renderGhost(int x, int y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x - xTabSize() / 2.0f, y - yTabSize() / 2.0f, 0.5f);
    glColor3f(r, g, b);
    glutSolidCube(1.0f); // Cambia la esfera por un cubo
    glPopMatrix();
}


bool isOccupiedByGhost(int x, int y) {
    return (x == ghost1X && y == ghost1Y) || (x == ghost2X && y == ghost2Y);
}

int isAvailableForMove(int x, int y) {
    char** map = getMap();
    if (map[x][y] != 0 && !isOccupiedByGhost(x, y)) {
        return 1;
    }
    return 0;
}



void moveGhost1() {
    if (isPaused) return;  //If the game is paused the ghoest dosent move

    int dx = 0, dy = 0;
    int direction = rand() % 4;

    //check if the ghost can move to that direction
    if (direction == 0 && ghost1Y < yTabSize() - 1 && isAvailableForMove(ghost1X, ghost1Y + 1)) dy = 1;
    if (direction == 1 && ghost1Y > 0 && isAvailableForMove(ghost1X, ghost1Y - 1)) dy = -1;
    if (direction == 2 && ghost1X > 0 && isAvailableForMove(ghost1X - 1, ghost1Y)) dx = -1;
    if (direction == 3 && ghost1X < xTabSize() - 1 && isAvailableForMove(ghost1X + 1, ghost1Y)) dx = 1;

    ghost1X += dx;
    ghost1Y += dy;
}

//Absolute diferences alg.
void moveGhost2() {
    if (isPaused) return;

    int dx = 0, dy = 0;

    // Determine the direction of movement based on the distance between the ghost and Pac-Man.
    if (abs(pacmanX - ghost2X) > abs(pacmanY - ghost2Y)) {
        if (pacmanX > ghost2X && ghost2X < xTabSize() - 1 && isAvailableForMove(ghost2X + 1, ghost2Y)) {
            dx = 1; // Move right
        } else if (pacmanX < ghost2X && ghost2X > 0 && isAvailableForMove(ghost2X - 1, ghost2Y)) {
            dx = -1; // Move left
        }
    } else {
        // Move vertically if the distance along the y-axis is greater or equal.
        if (pacmanY > ghost2Y && ghost2Y < yTabSize() - 1 && isAvailableForMove(ghost2X, ghost2Y + 1)) {
            dy = 1;
        } else if (pacmanY < ghost2Y && ghost2Y > 0 && isAvailableForMove(ghost2X, ghost2Y - 1)) {
            dy = -1;
        }
    }

    // If the ghost cannot move, do a random movement direction up to 4 times.
    if (dx == 0 && dy == 0) {
        // Try a random movement direction up to 4 times.
        for (int i = 0; i < 4; i++) {
            int direction = rand() % 4;
            if (direction == 0 && ghost2Y < yTabSize() - 1 && isAvailableForMove(ghost2X, ghost2Y + 1)) {
                dy = 1; // Move down
                break;
            }
            if (direction == 1 && ghost2Y > 0 && isAvailableForMove(ghost2X, ghost2Y - 1)) {
                dy = -1; // Move up
                break;
            }
            if (direction == 2 && ghost2X > 0 && isAvailableForMove(ghost2X - 1, ghost2Y)) {
                dx = -1; // Move left
                break;
            }
            if (direction == 3 && ghost2X < xTabSize() - 1 && isAvailableForMove(ghost2X + 1, ghost2Y)) {
                dx = 1; // Move right
                break;
            }
        }
    }

    ghost2X += dx;
    ghost2Y += dy;
}


//TEMP MOVE GHOST EVERY 1S
void ghostTimer(int value) {
    moveGhost1();
    moveGhost2();
    glutTimerFunc(1000, ghostTimer, 0);
}

void resetGame() {
    pacmanX = 0;
    pacmanY = 0;
    ghost1X = 0;
    ghost1Y = 5;
    ghost2X = 5;
    ghost2Y = 0;
}

void checkCollision() {
    if ((pacmanX == ghost1X && pacmanY == ghost1Y) || (pacmanX == ghost2X && pacmanY == ghost2Y)) {
        printf("Pac-Man ha sido atrapado. Reiniciando juego...\n");

        openMapsFile("pmsimple.txt");
        resetMapCounter();
        nextMap();

        resetGame();
    }
}


void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

//Mouse motion function
void mouseMotion(int x, int y) {
    if (isDragging) {
        float dx = (x - lastMouseX) * 0.05f;
        float dy = (y - lastMouseY) * 0.05f;

        cameraX -= dx;
        cameraY += dy;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    renderMap();
    renderPacman();
    renderGhost(ghost1X, ghost1Y, 0.0f, 1.0f, 1.0f);
    renderGhost(ghost2X, ghost2Y, 1.0f, 0.0f, 0.0f);

    checkCollision();

    glutSwapBuffers();
    glutPostRedisplay();
}


void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (double)w / (double)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    //Pause the game
    if (key == 'p') {
        isPaused = !isPaused;
        if (isPaused) {
            printf("Juego pausado.\n");
        } else {
            printf("Juego reanudado.\n");
        }
    }else{
    switch (key) {
        case 'w':
            if (isPaused) return;
            if (pacmanY < yTabSize() - 1 && isAvailableForMove(pacmanX, pacmanY + 1)) {
                pacmanY++;
                pacmanAngle = 0.0f; //look up

                // Check if it is visited
                if (getMap()[pacmanX][pacmanY] == 1) {
                    getMap()[pacmanX][pacmanY] = 2; // visted
                    visitedCount++;
                    printf("Score: %d.\n", visitedCount); //shows score
                }
            }
            break;
        case 's':
            if (isPaused) return;
            if (pacmanY > 0 && isAvailableForMove(pacmanX, pacmanY - 1)) {
                pacmanY--;
                pacmanAngle = 180.0f; // look down

                if (getMap()[pacmanX][pacmanY] == 1) {
                    getMap()[pacmanX][pacmanY] = 2;
                    visitedCount++;
                    printf("Score: %d.\n", visitedCount);
                }
            }
            break;
        case 'a':
            if (isPaused) return;
            if (pacmanX > 0 && isAvailableForMove(pacmanX - 1, pacmanY)) {
                pacmanX--;
                pacmanAngle = 90.0f; //look left

                if (getMap()[pacmanX][pacmanY] == 1) {
                    getMap()[pacmanX][pacmanY] = 2;
                    visitedCount++;
                    printf("Score: %d.\n", visitedCount);
                }
            }
            break;
        case 'd':
            if (isPaused) return;
            if (pacmanX < xTabSize() - 1 && isAvailableForMove(pacmanX + 1, pacmanY)) {
                pacmanX++;
                pacmanAngle = -90.0f; //look right

                if (getMap()[pacmanX][pacmanY] == 1) {
                    getMap()[pacmanX][pacmanY] = 2;
                    visitedCount++;
                    printf("Score: %d.\n", visitedCount);
                }
            }
            break;

        case 'n':  // Load next map
            if (isPaused) return;
            if (nextMap() != nullptr) {
                pacmanX = 0; pacmanY = 0;  // Restart pacman position
                glutPostRedisplay();
            } else {
                printf("No more maps!\n");
            }
            break;
        case 27:  //Exit game
            closeMapsFile();
            exit(0);
            break;
    }
    }

    //Check win
    if (visitedCount == totalVisitable) {
        printf("¡Winner! Map complete.\n");

        if (nextMap() != nullptr) {
            pacmanX = 0;
            pacmanY = 0;
            glutPostRedisplay();
        } else {
            printf("No more maps available.\n");
            closeMapsFile();
            exit(0);
        }
    }

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PAC-MAN pr3");

    initOpenGL();
    openMapsFile("pmsimple.txt");
    nextMap();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(200, ghostTimer, 0);
    glutMainLoop();

    return 0;
}