#include <GL/glut.h>
#include <cstdio>
#include "pm_maps.h"
#include <cmath>

// Ángulo para animar el movimiento de las extremidades
float limbAngle = 0.0f;
bool limbDirection = true;
float pacmanAngle = 0.0f; // Ángulo de rotación del pingüino

// Función para dibujar un cubo escalado
void drawScaledCube(float sx, float sy, float sz, float r, float g, float b) {
    glPushMatrix();
    glScalef(sx, sy, sz);
    glColor3f(r, g, b);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Posición del Pac-Man
int pacmanX = 1, pacmanY = 1;



// Variables globales
int windowWidth = 800;
int windowHeight = 600;

// Posición de la cámara
float cameraX = 00.0f, cameraY = -6.0f, cameraZ =11.0f; // Posición de la cámara
float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f;   // Punto al que mira la cámara
float zoomFactor = 1.0f;                                // Factor de zoom


// Función para configurar la ventana y la proyección
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

// Función para renderizar al muñeco
void renderPacman() {
    glPushMatrix();

    // Ajustar la posición del pingüino en el mapa
    glTranslatef(pacmanX - xTabSize() / 2.0f, pacmanY - yTabSize() / 2.0f, 0.5f);

    // Rotar el modelo según la dirección
    glRotatef(pacmanAngle, 0.0f, 0.0f, 1.0f);

    // Escalar todo el modelo para hacerlo más pequeño
    glScalef(0.5f, 0.5f, 0.5f);

    // Animación de las aletas
    limbAngle += limbDirection ? 2.0f : -2.0f;
    if (limbAngle > 30.0f) limbDirection = false;
    if (limbAngle < -30.0f) limbDirection = true;

    // Dibujar el cuerpo principal (óvalo gris)
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f); // Gris
    glScalef(1.0f, 1.0f, 1.5f); // Escalar para que parezca un óvalo
    glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();

    // Dibujar la barriga (círculo blanco)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); // Blanco
    glTranslatef(0.0f, 0.0f, 0.61f); // Delante del cuerpo
    glScalef(0.7f, 0.7f, 0.01f); // Escalar en 2D
    glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();

    // Dibujar los ojos
    glPushMatrix(); // Ojo derecho
    glColor3f(0.0f, 0.0f, 0.0f); // Negro
    glTranslatef(-0.2f, 0.4f, 0.7f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPushMatrix(); // Ojo izquierdo
    glColor3f(0.0f, 0.0f, 0.0f); // Negro
    glTranslatef(0.2f, 0.4f, 0.7f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    // Dibujar el pico
    glPushMatrix();
    glColor3f(1.0f, 0.5f, 0.0f); // Naranja
    glTranslatef(0.0f, 0.2f, 0.75f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Apuntar hacia adelante
    glutSolidCone(0.1f, 0.3f, 20, 20);
    glPopMatrix();

    // Dibujar las aletas
    glPushMatrix(); // Aleta derecha
    glColor3f(0.5f, 0.5f, 0.5f); // Gris
    glTranslatef(-0.7f, 0.0f, 0.4f);
    glRotatef(limbAngle, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.7f, 0.1f); // Alargada
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    glPushMatrix(); // Aleta izquierda
    glColor3f(0.5f, 0.5f, 0.5f); // Gris
    glTranslatef(0.7f, 0.0f, 0.4f);
    glRotatef(-limbAngle, 1.0f, 0.0f, 0.0f);
    glScalef(0.2f, 0.7f, 0.1f); // Alargada
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}



// Función de renderizado
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Configurar la cámara
    gluLookAt(cameraX, cameraY, cameraZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Dibujar el mapa
    renderMap();

    // Dibujar al muñeco
    renderPacman();

    glutSwapBuffers();

    // Forzar la actualización para la animación
    glutPostRedisplay();
}


// Función de actualización de la ventana
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (double)w / (double)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Función para manejar el teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'i': zoomFactor *= 0.9f; break;  // Acercar (reducir zoomFactor)
        case 'k': zoomFactor *= 1.1f; break;  // Alejar (aumentar zoomFactor)
        case 'j': cameraX -= 1.0f; targetX -= 1.0f; break;  // Mover a la izquierda
        case 'l': cameraX += 1.0f; targetX += 1.0f; break;  // Mover a la derecha


        case 'w':
            if (pacmanY < yTabSize() - 1 && getMap()[pacmanX][pacmanY + 1] == 1) {
                pacmanY++;
                pacmanAngle = 0.0f; // Mirando hacia arriba
            }
            break;
        case 's':
            if (pacmanY > 0 && getMap()[pacmanX][pacmanY - 1] == 1) {
                pacmanY--;
                pacmanAngle = 180.0f; // Mirando hacia abajo
            }
            break;
        case 'a':
            if (pacmanX > 0 && getMap()[pacmanX - 1][pacmanY] == 1) {
                pacmanX--;
                pacmanAngle = 90.0f; // Mirando a la izquierda
            }
            break;
        case 'd':
            if (pacmanX < xTabSize() - 1 && getMap()[pacmanX + 1][pacmanY] == 1) {
                pacmanX++;
                pacmanAngle = -90.0f; // Mirando a la derecha
            }
            break;


        case 'n':  // Cargar el siguiente mapa
            if (nextMap() != nullptr) {
                pacmanX = 1; pacmanY = 1;  // Reiniciar posición de Pac-Man
                glutPostRedisplay();
            } else {
                printf("No more maps!\n");
            }
            break;
        case 27:  // Salir con la tecla ESC
            closeMapsFile();
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// Programa principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PAC-MAN Map Viewer");

    initOpenGL();
    openMapsFile("pmsimple.txt");
    nextMap();  // Cargar el primer mapa

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}