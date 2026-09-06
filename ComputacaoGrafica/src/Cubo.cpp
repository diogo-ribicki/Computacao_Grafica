#include "GL/glut.h"
#include "Cubo.hpp"

Cubo criar_cubo(
    double posicao_x,
    double posicao_y,
    double posicao_z,
    double tamanho_lado
) {
    Cubo novo_cubo;

    novo_cubo.tamanhoLado = tamanho_lado;

    novo_cubo.posicao = {
        posicao_x,
        posicao_y,
        posicao_z
    };

    double metade = tamanho_lado / 2.0;

    novo_cubo.vertices = {
        { posicao_x - metade, posicao_y - metade, posicao_z + metade }, // 0
        { posicao_x + metade, posicao_y - metade, posicao_z + metade }, // 1
        { posicao_x + metade, posicao_y + metade, posicao_z + metade }, // 2
        { posicao_x - metade, posicao_y + metade, posicao_z + metade }, // 3

        { posicao_x - metade, posicao_y - metade, posicao_z - metade }, // 4
        { posicao_x + metade, posicao_y - metade, posicao_z - metade }, // 5
        { posicao_x + metade, posicao_y + metade, posicao_z - metade }, // 6
        { posicao_x - metade, posicao_y + metade, posicao_z - metade }  // 7
    };

    novo_cubo.arestas = {
        // Frente
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        // Tras
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},

        // Ligacoes entre frente e tras
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

    return novo_cubo;
}

void desenhar_cubo(const Cubo& cubo) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);


    for (const Aresta& aresta : cubo.arestas) {
        const Vertice& vertice_origem =
            cubo.vertices[aresta.first];

        const Vertice& vertice_destino =
            cubo.vertices[aresta.second];

        glVertex3f(
            static_cast<GLfloat>(vertice_origem.x),
            static_cast<GLfloat>(vertice_origem.y),
            static_cast<GLfloat>(vertice_origem.z)
        );

        glVertex3f(
            static_cast<GLfloat>(vertice_destino.x),
            static_cast<GLfloat>(vertice_destino.y),
            static_cast<GLfloat>(vertice_destino.z)
        );
    }

    glEnd();
}