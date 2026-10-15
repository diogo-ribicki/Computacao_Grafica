
#include "Iluminacao.hpp"

#include <GL/glut.h>
#include <iostream>

namespace {

    struct ConfiguracaoLuz {
        GLenum identificador;
        GLfloat ambiente[4];
        GLfloat difusa[4];
        GLfloat especular[4];
        GLfloat posicao[4];
    };

    // Posicoes definidas em coordenadas
    // do mundo, com o modelo centralizado
    // aproximadamente na origem.
    const ConfiguracaoLuz LUZES[] = {

        // Luz 0: branca, acima e a esquerda.
        {
            GL_LIGHT0,
            { 0.08f, 0.08f, 0.08f, 1.0f },
            { 0.70f, 0.70f, 0.70f, 1.0f },
            { 0.80f, 0.80f, 0.80f, 1.0f },
            { -3.0f, 3.0f, 4.0f, 1.0f }
        },

        // Luz 1: azulada, a direita.
        {
            GL_LIGHT1,
            { 0.01f, 0.02f, 0.05f, 1.0f },
            { 0.15f, 0.35f, 0.75f, 1.0f },
            { 0.20f, 0.45f, 0.85f, 1.0f },
            { 3.0f, 2.0f, 3.0f, 1.0f }
        },

        // Luz 2: quente, abaixo e a frente.
        {
            GL_LIGHT2,
            { 0.04f, 0.02f, 0.01f, 1.0f },
            { 0.75f, 0.30f, 0.10f, 1.0f },
            { 0.85f, 0.45f, 0.20f, 1.0f },
            { 0.0f, -3.0f, 3.0f, 1.0f }
        }
    };

    const int QUANTIDADE_LUZES =
        static_cast<int>(
            sizeof(LUZES) / sizeof(LUZES[0])
            );

}


void inicializarIluminacao() {

    // Ativa o calculo de iluminacao
    // do pipeline fixo do OpenGL.
    glEnable(GL_LIGHTING);

    // Normaliza as normais apos as
    // transformacoes de escala.
    glEnable(GL_NORMALIZE);

    // Interpola a iluminacao dos vertices.
    glShadeModel(GL_SMOOTH);

    // Considera a posicao real do observador
    // no calculo da reflexao especular.
    glLightModeli(
        GL_LIGHT_MODEL_LOCAL_VIEWER,
        GL_TRUE
    );

    // Permite iluminar os dois lados
    // das faces do modelo OBJ.
    glLightModeli(
        GL_LIGHT_MODEL_TWO_SIDE,
        GL_TRUE
    );

    // Remove contribuicao ambiente global.
    // Assim, desligar todas as luzes deixa
    // o modelo sem iluminacao.
    const GLfloat ambienteGlobal[] = {
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glLightModelfv(
        GL_LIGHT_MODEL_AMBIENT,
        ambienteGlobal
    );

    // Material neutro para visualizar
    // as cores e intensidades das luzes.
    const GLfloat materialAmbiente[] = {
        0.35f, 0.35f, 0.35f, 1.0f
    };

    const GLfloat materialDifuso[] = {
        0.80f, 0.80f, 0.80f, 1.0f
    };

    const GLfloat materialEspecular[] = {
        0.70f, 0.70f, 0.70f, 1.0f
    };

    // A cor do material sera definida
    // explicitamente, nao por glColor.
    glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_AMBIENT,
        materialAmbiente
    );

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_DIFFUSE,
        materialDifuso
    );

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_SPECULAR,
        materialEspecular
    );

    glMaterialf(
        GL_FRONT_AND_BACK,
        GL_SHININESS,
        48.0f
    );

    // Configura as tres fontes.
    for (const ConfiguracaoLuz& luz : LUZES) {

        glLightfv(
            luz.identificador,
            GL_AMBIENT,
            luz.ambiente
        );

        glLightfv(
            luz.identificador,
            GL_DIFFUSE,
            luz.difusa
        );

        glLightfv(
            luz.identificador,
            GL_SPECULAR,
            luz.especular
        );

        // Todas comecam ativadas.
        glEnable(luz.identificador);
    }
}


void posicionarLuzes() {

    // IMPORTANTE:
    // Chamar apos a transformacao da
    // camera, mas antes de transformar
    // o modelo OBJ.
    for (const ConfiguracaoLuz& luz : LUZES) {

        glLightfv(
            luz.identificador,
            GL_POSITION,
            luz.posicao
        );
    }
}


void alternarLuz(int numeroLuz) {

    // Evita acessar posicoes invalidas.
    if (
        numeroLuz < 0 ||
        numeroLuz >= QUANTIDADE_LUZES
        ) {
        return;
    }

    GLenum identificador =
        LUZES[numeroLuz].identificador;

    // Consulta o estado diretamente
    // no OpenGL, sem duplicar variaveis.
    if (glIsEnabled(identificador)) {

        glDisable(identificador);

        std::cout
            << "Luz " << numeroLuz
            << " desativada."
            << std::endl;
    }
    else {

        glEnable(identificador);

        std::cout
            << "Luz " << numeroLuz
            << " ativada."
            << std::endl;
    }
}
