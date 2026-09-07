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

void desenhar_cubo(const Cubo& cubo);