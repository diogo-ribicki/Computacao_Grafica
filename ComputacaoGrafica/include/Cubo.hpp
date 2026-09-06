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