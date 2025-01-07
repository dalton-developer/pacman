//
// Created by Dalton Gabriel Abambari on 26/12/24.
//
#ifndef CUBO_H
#define CUBO_H

class Cubo {
public:
    float theta[3];
    float escala;
    float xCubo;
    float yCubo;
    float v;

    Cubo(float x, float y, float v);
    void dibujar();
    void actualizar();
};

#endif

