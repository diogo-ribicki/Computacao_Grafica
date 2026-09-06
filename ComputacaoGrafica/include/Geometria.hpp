#pragma once

#include <vector>
#include <utility>

struct Vertice {
    double x;
    double y;
    double z;
};

using Aresta = std::pair<int, int>;
using ListaVertices = std::vector<Vertice>;
using ListaArestas = std::vector<Aresta>;