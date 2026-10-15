#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "ModeloOBJ.hpp"
#include "OBJLoader.hpp"
#include "OBJRenderer.hpp"


ModeloOBJ modelo;


// Caminho do OBJ utilizado nesta etapa.
const std::string CAMINHO_MODELO =
"data/porsche.obj";


// Dados utilizados somente para enquadrar
// o modelo dentro da camera.
GLfloat centroModeloX = 0.0f;
GLfloat centroModeloY = 0.0f;
GLfloat centroModeloZ = 0.0f;

GLfloat escalaModelo = 1.0f;


void initGL();

void display();

void reshape(
    GLsizei width,
    GLsizei height
);

void keyboard(
    unsigned char key,
    int x,
    int y
);

void calcularEnquadramentoModelo();


int main(
    int argc,
    char** argv
) {
    glutInit(
        &argc,
        argv
    );

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(
        640,
        480
    );

    glutInitWindowPosition(
        50,
        50
    );

    glutCreateWindow(
        "M2 - Renderizacao OBJ"
    );


    initGL();


    std::string mensagemErro;

    if (
        !carregarOBJ(
            CAMINHO_MODELO,
            modelo,
            mensagemErro
        )
        ) {
        std::cerr
            << "Erro ao carregar OBJ: "
            << mensagemErro
            << std::endl;

        return 1;
    }


    if (
        modelo.vertices.empty() ||
        modelo.faces.empty()
        ) {
        std::cerr
            << "O arquivo OBJ nao possui "
            << "vertices ou faces para renderizar."
            << std::endl;

        return 1;
    }


    calcularEnquadramentoModelo();


    std::cout
        << "OBJ carregado com sucesso."
        << std::endl;

    std::cout
        << "Vertices: "
        << modelo.vertices.size()
        << std::endl;

    std::cout
        << "Coordenadas de textura: "
        << modelo.texturas.size()
        << std::endl;

    std::cout
        << "Normais: "
        << modelo.normais.size()
        << std::endl;

    std::cout
        << "Faces: "
        << modelo.faces.size()
        << std::endl;


    glutDisplayFunc(
        display
    );

    glutReshapeFunc(
        reshape
    );

    glutKeyboardFunc(
        keyboard
    );


    glutMainLoop();

    return 0;
}


void initGL() {
    glClearColor(
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    glClearDepth(
        1.0f
    );


    // Mantem o depth buffer utilizado
    // anteriormente pelo projeto.
    glEnable(
        GL_DEPTH_TEST
    );

    glDepthFunc(
        GL_LEQUAL
    );


    // Queremos superficies preenchidas,
    // e nao wireframe.
    glPolygonMode(
        GL_FRONT_AND_BACK,
        GL_FILL
    );


    // Iluminacao e textura serao configuradas
    // nas proximas etapas da M2.
    glDisable(
        GL_LIGHTING
    );

    glDisable(
        GL_TEXTURE_2D
    );
}


void display() {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();


    // Afasta o modelo da camera.
    glTranslatef(
        0.0f,
        0.0f,
        -5.0f
    );


    // Ajusta modelos de tamanhos diferentes
    // para uma escala adequada a visualizacao.
    glScalef(
        escalaModelo,
        escalaModelo,
        escalaModelo
    );


    // Coloca o centro geometrico do modelo
    // na origem antes do desenho.
    glTranslatef(
        -centroModeloX,
        -centroModeloY,
        -centroModeloZ
    );


    // Como ainda nao ha iluminacao nem textura,
    // utilizamos uma cor simples para visualizar
    // as superficies.
    glColor3f(
        0.8f,
        0.8f,
        0.8f
    );


    desenharModeloOBJ(
        modelo
    );


    glutSwapBuffers();
}


void reshape(
    GLsizei width,
    GLsizei height
) {
    if (height == 0) {
        height = 1;
    }


    GLfloat aspect =
        static_cast<GLfloat>(width) /
        static_cast<GLfloat>(height);


    glViewport(
        0,
        0,
        width,
        height
    );


    glMatrixMode(
        GL_PROJECTION
    );

    glLoadIdentity();


    // Mantem a visualizacao em perspectiva
    // ja utilizada pelo projeto.
    gluPerspective(
        45.0f,
        aspect,
        0.1f,
        100.0f
    );


    glMatrixMode(
        GL_MODELVIEW
    );
}


void keyboard(
    unsigned char key,
    int x,
    int y
) {
    switch (key) {

        // ESC
    case 27:
        std::exit(0);
        break;

    default:
        break;
    }
}


void calcularEnquadramentoModelo() {
    if (modelo.vertices.empty()) {
        return;
    }

    std::vector<double> valoresX;
    std::vector<double> valoresY;
    std::vector<double> valoresZ;

    for (const VerticeOBJ& vertice : modelo.vertices) {
        valoresX.push_back(vertice.x);
        valoresY.push_back(vertice.y);
        valoresZ.push_back(vertice.z);
    }

    std::sort(valoresX.begin(), valoresX.end());
    std::sort(valoresY.begin(), valoresY.end());
    std::sort(valoresZ.begin(), valoresZ.end());

    // Ignora os 1% menores e 1% maiores valores
    // para evitar que vertices muito distantes
    // distorcam o enquadramento.
    const std::size_t quantidade = valoresX.size();

    const std::size_t inicio =
        quantidade / 100;

    const std::size_t fim =
        quantidade - 1 - inicio;

    const double menorX = valoresX[inicio];
    const double maiorX = valoresX[fim];

    const double menorY = valoresY[inicio];
    const double maiorY = valoresY[fim];

    const double menorZ = valoresZ[inicio];
    const double maiorZ = valoresZ[fim];

    centroModeloX = static_cast<GLfloat>(
        (menorX + maiorX) / 2.0
        );

    centroModeloY = static_cast<GLfloat>(
        (menorY + maiorY) / 2.0
        );

    centroModeloZ = static_cast<GLfloat>(
        (menorZ + maiorZ) / 2.0
        );

    const double tamanhoX = maiorX - menorX;
    const double tamanhoY = maiorY - menorY;
    const double tamanhoZ = maiorZ - menorZ;

    const double maiorDimensao = std::max(
        tamanhoX,
        std::max(tamanhoY, tamanhoZ)
    );

    if (maiorDimensao > 0.0) {
        escalaModelo = static_cast<GLfloat>(
            2.5 / maiorDimensao
            );
    }

    std::cout
        << "Escala do modelo: "
        << escalaModelo
        << std::endl;
}