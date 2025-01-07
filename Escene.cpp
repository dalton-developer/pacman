//
// Created by Dalton Gabriel Abambari on 26/12/24.
//

#include "Escene.h"
#include <GL/glut.h>
#include "Utils.h"

Escene::Escene() {
    cubos[0] = nullptr;
    cubos[1] = nullptr;
}

void Escene::iniciarCubos() {
    cubos[0] = new Cubo(-1.5, 0.0, -0.2);
    cubos[1] = new Cubo(1.5, 0.0, 0.8);
}

void Escene::dibujar() {
    glPushMatrix();
    eixos();
    for (int i = 0; i < 2; i++) {
        if (cubos[i] != nullptr) {
            cubos[i]->dibujar();
        }
    }
    glPopMatrix();
}

void Escene::actualizar() {
    for (int i = 0; i < 2; i++) {
        if (cubos[i] != nullptr) {
            cubos[i]->actualizar();
        }
    }
}
