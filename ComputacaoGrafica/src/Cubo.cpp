#include <cmath>
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
        // Frontal
        { posicao_x - metade, posicao_y - metade, posicao_z + metade }, // 0
        { posicao_x + metade, posicao_y - metade, posicao_z + metade }, // 1
        { posicao_x + metade, posicao_y + metade, posicao_z + metade }, // 2
        { posicao_x - metade, posicao_y + metade, posicao_z + metade }, // 3

        // Traseira
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

void movimentar_cubo(
    Cubo& cubo,
    double deslocamento_x,
    double deslocamento_y,
    double deslocamento_z
) {

    // X' = X+deltaX
    // Y' = Y+deltaY
    // Z' = Z+deltaZ

    // Alterar o centro
    cubo.posicao.x += deslocamento_x;
    cubo.posicao.y += deslocamento_y;
    cubo.posicao.z += deslocamento_z;

    // Alteramos os vértices
    for (Vertice& vertice : cubo.vertices) {
        vertice.x += deslocamento_x;
        vertice.y += deslocamento_y;
        vertice.z += deslocamento_z;
    }

}


void escalar_cubo(
    Cubo& cubo,
    double escala_x,
    double escala_y,
    double escala_z

) {

    for (Vertice& vertice : cubo.vertices) {

        // Desloca vértice para centro do cubo
        vertice.x -= cubo.posicao.x;
        vertice.y -= cubo.posicao.y;
        vertice.z -= cubo.posicao.z;

        // Aplica a escala nos eixos
        vertice.x *= escala_x;
        vertice.y *= escala_y;
        vertice.z *= escala_z;

        // Devole o vértice para sua posição original
        vertice.x += cubo.posicao.x;
        vertice.y += cubo.posicao.y;
        vertice.z += cubo.posicao.z;

    }

}


void rotacionar_cubo_x(
    Cubo& cubo,
    double angulo_graus
) {
    const double PI = 3.14159265358979323846;
    double angulo = angulo_graus * PI / 180.0;

    double seno = std::sin(angulo);
    double cosseno = std::cos(angulo);

    for (Vertice& vertice : cubo.vertices) {

        // Leva o vertice para o centro do cubo
        double x = vertice.x - cubo.posicao.x;
        double y = vertice.y - cubo.posicao.y;
        double z = vertice.z - cubo.posicao.z;

        // Rotação no plano YZ
        double novo_y = y * cosseno - z * seno;
        double novo_z = y * seno + z * cosseno;

        // Retorna o centro original
        vertice.x = x + cubo.posicao.x;
        vertice.y = novo_y + cubo.posicao.y;
        vertice.z = novo_z + cubo.posicao.z;

    }
}

void rotacionar_cubo_y(
    Cubo& cubo,
    double angulo_graus
) {
    const double PI = 3.14159265358979323846;
    double angulo = angulo_graus * PI / 180.0;

    double seno = std::sin(angulo);
    double cosseno = std::cos(angulo);

    for (Vertice& vertice : cubo.vertices) {

        // Leva o vertice para o centro do cubo
        double x = vertice.x - cubo.posicao.x;
        double y = vertice.y - cubo.posicao.y;
        double z = vertice.z - cubo.posicao.z;

        // Rotação no plano XZ
        double novo_x = x * cosseno - z * seno;
        double novo_z = x * seno + z * cosseno;

        // Retorna o centro original
        vertice.x = novo_x + cubo.posicao.x;
        vertice.y = y + cubo.posicao.y;
        vertice.z = novo_z + cubo.posicao.z;

    }
}

void rotacionar_cubo_z(
    Cubo& cubo,
    double angulo_graus
) {
    const double PI = 3.14159265358979323846;
    double angulo = angulo_graus * PI / 180.0;

    double seno = std::sin(angulo);
    double cosseno = std::cos(angulo);

    for (Vertice& vertice : cubo.vertices) {

        // Leva o vertice para o centro do cubo
        double x = vertice.x - cubo.posicao.x;
        double y = vertice.y - cubo.posicao.y;
        double z = vertice.z - cubo.posicao.z;

        // Rotação no plano XY
        double novo_x = x * cosseno - y * seno;
        double novo_y = x * seno + y * cosseno;

        // Retorna o centro original
        vertice.x = novo_x + cubo.posicao.x;
        vertice.y = novo_y + cubo.posicao.y;
        vertice.z = z + cubo.posicao.z;
    }
}

void desenhar_cubo(const Cubo& cubo) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);

    // Percorre as arestas do cubo
    for (const Aresta& aresta : cubo.arestas) {
        const Vertice& vertice_origem =
            cubo.vertices[aresta.first];

        const Vertice& vertice_destino =
            cubo.vertices[aresta.second];

        // Ponto onde a linha começa
        glVertex3f(
            static_cast<GLfloat>(vertice_origem.x),
            static_cast<GLfloat>(vertice_origem.y),
            static_cast<GLfloat>(vertice_origem.z)
        );

        // Ponto onde a linha termina
        glVertex3f(
            static_cast<GLfloat>(vertice_destino.x),
            static_cast<GLfloat>(vertice_destino.y),
            static_cast<GLfloat>(vertice_destino.z)
        );
    }

    glEnd();
}