#include <GL/glut.h>
#include <cstdio>
#include "pm_maps.h"

// Variables globales para controlar el tamaño de la ventana y el mapa
int windowWidth = 800;
int windowHeight = 600;

// Función para configurar la ventana y la proyección
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);  // Habilitar profundidad para cubos 3D

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);  // Configurar la perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);              // Habilitar transparencia
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Función de renderizado
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(10.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);  // Ajustar la vista

    renderMap();

    glutSwapBuffers();
}


// Función de actualización de la ventana
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);  // Ajustar el viewport al nuevo tamaño de la ventana

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (double)w / (double)h, 0.1f, 100.0f);  // Actualizar la proyección
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'n':  // Cargar el siguiente mapa
            if (nextMap() != nullptr) {
                glutPostRedisplay();  // Redibujar la escena
            } else {
                printf("No more maps!\n");
            }
            break;
        case 27:  // Salir con la tecla ESC
            exit(0);
            break;
    }
}


int main(int argc, char** argv) {
    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("PAC-MAN Map Viewer");

    // Inicializar OpenGL
    initOpenGL();

    // Cargar el archivo de mapas
    openMapsFile("pmsimple.txt");
    nextMap();  // Cargar el primer mapa

    // Configurar funciones de GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Iniciar el bucle principal
    glutMainLoop();

    // Cerrar archivo de mapas al salir
    closeMapsFile();
    return 0;
}

