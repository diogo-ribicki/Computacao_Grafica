#pragma once

#include "Geometria.hpp"

struct Cubo {
    double tamanhoLado;
    Vertice posicao;

    ListaVertices vertices;
    ListaArestas arestas;
};

Cubo criar_cubo(
    double posicao_x,
    double posicao_y,
    double posicao_z,
    double tamanho_lado
);

void movimentar_cubo(
    Cubo& cubo,
    double deslocamento_x,
    double deslocamento_y,
    double deslocamento_z
);

void escalar_cubo(
    Cubo& cubo,
    double escala_x,
    double escala_y,
    double escala_z

);

void rotacionar_cubo_x(
    Cubo& cubo,
    double angulo_graus
);

void rotacionar_cubo_y(
    Cubo& cubo,
    double angulo_graus
);


void rotacionar_cubo_z(
    Cubo& cubo,
    double angulo_graus
);
void desenhar_cubo(const Cubo& cubo);