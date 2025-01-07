//
// Created by Dalton Gabriel Abambari on 26/12/24.
//

#include "Cubo.h"
#include <GL/glut.h>
#include "Utils.h"

Cubo::Cubo(float x, float y, float v) {
    this->xCubo = x;
    this->yCubo = y;
    this->v = v;
    this->escala = 1.0;
    this->theta[0] = 0.0;
    this->theta[1] = 0.0;
    this->theta[2] = 0.0;
}

void Cubo::dibujar() {
    glPushMatrix();
    glTranslatef(xCubo, yCubo, 0.0);
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glRotatef(theta[2], 0.0, 0.0, 1.0);
    glScalef(escala, escala, escala);
    colorcube();
    glPopMatrix();
}

void Cubo::actualizar() {
    theta[0] += v;
    theta[1] += v;
    theta[2] += v;
}
