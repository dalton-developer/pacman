//
// Created by Dalton Gabriel Abambari on 26/12/24.
//

#ifndef ESCENE_H
#define ESCENE_H

#include "Cubo.h"

class Escene {
private:
    Cubo* cubos[2];

public:
    Escene();
    void iniciarCubos();
    void dibujar();
    void actualizar();
};

#endif

