#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "ModeloOBJ.hpp"
#include "OBJLoader.hpp"
#include "OBJRenderer.hpp"
#include "Iluminacao.hpp"
#include "TexturaBMP.hpp"


// Modelo geometrico e textura visual.
// Sao recursos independentes.
ModeloOBJ modelo;
TexturaBMP texturaBMP;


// Caminhos relativos a pasta do projeto.
const std::string CAMINHO_MODELO = "data/elepham.obj";
const std::string CAMINHO_TEXTURA = "Textures/launch.bmp";


// Enquadramento automatico do OBJ.
GLfloat centroModeloX = 0.0f;
GLfloat centroModeloY = 0.0f;
GLfloat centroModeloZ = 0.0f;

GLfloat escalaModelo = 1.0f;


// Estado das transformacoes.
GLfloat posicaoX = 0.0f;
GLfloat posicaoY = 0.0f;
GLfloat posicaoZ = 0.0f;

GLfloat rotacaoX = 0.0f;
GLfloat rotacaoY = 0.0f;
GLfloat rotacaoZ = 0.0f;

GLfloat escalaTransformacao = 1.0f;


// Incrementos dos controles.
const GLfloat PASSO_MOVIMENTO = 0.2f;
const GLfloat PASSO_ROTACAO = 5.0f;

const GLfloat FATOR_AUMENTO = 1.1f;
const GLfloat FATOR_REDUCAO = 0.9f;

const GLfloat ESCALA_MINIMA = 0.1f;
const GLfloat ESCALA_MAXIMA = 5.0f;


// Prototipos.
void initGL();

void display();

void reshape(GLsizei width, GLsizei height);

void keyboard(unsigned char key, int x, int y);

void keyboardSpecial(int key, int x, int y);

void calcularEnquadramentoModelo();


int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Computação Gráfica");

    // Inicializa o estado OpenGL.
    initGL();

    std::string mensagemErro;

    // Carrega a geometria do OBJ.
    if (!carregarOBJ(
        CAMINHO_MODELO,
        modelo,
        mensagemErro
    )) {
        std::cerr
            << "Erro ao carregar OBJ: "
            << mensagemErro
            << std::endl;

        return 1;
    }

    if (modelo.vertices.empty() ||
        modelo.faces.empty()) {

        std::cerr
            << "O arquivo OBJ nao possui "
            << "vertices ou faces para renderizar."
            << std::endl;

        return 1;
    }

    // Calcula o centro e a escala inicial
    // sem modificar os vertices originais.
    calcularEnquadramentoModelo();

    // Carrega a imagem BMP e cria a
    // textura correspondente no OpenGL.
    if (!texturaBMP.carregar(
        CAMINHO_TEXTURA,
        mensagemErro
    )) {
        std::cerr
            << "Erro ao carregar textura: "
            << mensagemErro
            << std::endl;

        return 1;
    }

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

    std::cout
        << "Textura BMP carregada: "
        << CAMINHO_TEXTURA
        << std::endl;

    if (modelo.texturas.empty()) {
        std::cout
            << "Aviso: OBJ sem coordenadas vt. "
            << "Textura nao sera aplicada."
            << std::endl;
    }

    std::cout
        << "Controles de iluminacao:"
        << std::endl;

    std::cout
        << "1 - Alternar GL_LIGHT0"
        << std::endl;

    std::cout
        << "2 - Alternar GL_LIGHT1"
        << std::endl;

    std::cout
        << "3 - Alternar GL_LIGHT2"
        << std::endl;

    // Registra os callbacks.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

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

    glClearDepth(1.0f);

    // Mantem o depth buffer.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Faces preenchidas.
    glPolygonMode(
        GL_FRONT_AND_BACK,
        GL_FILL
    );

    // A textura sera ativada somente
    // quando o modelo for desenhado.
    glDisable(GL_TEXTURE_2D);

    // Preserva as tres fontes luminosas
    // e o material ja implementados.
    inicializarIluminacao();
}


void display() {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera.
    glTranslatef(
        0.0f,
        0.0f,
        -5.0f
    );

    // Posiciona as luzes antes das
    // transformacoes do objeto.
    posicionarLuzes();

    glPushMatrix();

    // 1. Translacao.
    glTranslatef(
        posicaoX,
        posicaoY,
        posicaoZ
    );

    // 2. Rotacao nos tres eixos.
    glRotatef(
        rotacaoX,
        1.0f, 0.0f, 0.0f
    );

    glRotatef(
        rotacaoY,
        0.0f, 1.0f, 0.0f
    );

    glRotatef(
        rotacaoZ,
        0.0f, 0.0f, 1.0f
    );

    // 3. Escala controlada pelo teclado.
    glScalef(
        escalaTransformacao,
        escalaTransformacao,
        escalaTransformacao
    );

    // 4. Escala automatica.
    glScalef(
        escalaModelo,
        escalaModelo,
        escalaModelo
    );

    // 5. Centraliza o modelo.
    glTranslatef(
        -centroModeloX,
        -centroModeloY,
        -centroModeloZ
    );

    // Ativa a textura somente se
    // existir um mapeamento UV no modelo.
    if (!modelo.texturas.empty() &&
        texturaBMP.carregada()) {

        texturaBMP.ativar();
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    // Desenha as faces triangulares.
    // OBJRenderer envia normais, UVs
    // e posicoes dos vertices.
    desenharModeloOBJ(modelo);

    // Impede que a textura interfira
    // em outros desenhos futuros.
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    glutSwapBuffers();
}


void reshape(GLsizei width, GLsizei height) {
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Projecao em perspectiva.
    gluPerspective(
        45.0f,
        aspect,
        0.1f,
        100.0f
    );

    glMatrixMode(GL_MODELVIEW);
}


// Controles de escala, rotacao e iluminacao.
void keyboard(unsigned char key, int x, int y) {
    bool alterouCena = true;

    switch (key) {

        // Encerra o programa.
    case 27:
        texturaBMP.liberar();
        std::exit(0);
        break;

        // Aumenta a escala.
    case 'p':
    case 'P':
        escalaTransformacao = std::min(
            ESCALA_MAXIMA,
            escalaTransformacao * FATOR_AUMENTO
        );
        break;

        // Reduz a escala.
    case 'o':
    case 'O':
        escalaTransformacao = std::max(
            ESCALA_MINIMA,
            escalaTransformacao * FATOR_REDUCAO
        );
        break;

        // Rotacao positiva em X.
    case 'x':
        rotacaoX += PASSO_ROTACAO;
        break;

        // Rotacao negativa em X.
    case 'X':
        rotacaoX -= PASSO_ROTACAO;
        break;

        // Rotacao positiva em Y.
    case 'y':
        rotacaoY += PASSO_ROTACAO;
        break;

        // Rotacao negativa em Y.
    case 'Y':
        rotacaoY -= PASSO_ROTACAO;
        break;

        // Rotacao positiva em Z.
    case 'z':
        rotacaoZ += PASSO_ROTACAO;
        break;

        // Rotacao negativa em Z.
    case 'Z':
        rotacaoZ -= PASSO_ROTACAO;
        break;

        // Alterna a primeira luz.
    case '1':
        alternarLuz(0);
        break;

        // Alterna a segunda luz.
    case '2':
        alternarLuz(1);
        break;

        // Alterna a terceira luz.
    case '3':
        alternarLuz(2);
        break;

    default:
        alterouCena = false;
        break;
    }

    if (alterouCena) {
        glutPostRedisplay();
    }
}


// Translacao nos tres eixos.
void keyboardSpecial(int key, int x, int y) {
    bool alterouTransformacao = true;

    switch (key) {

    case GLUT_KEY_LEFT:
        posicaoX -= PASSO_MOVIMENTO;
        break;

    case GLUT_KEY_RIGHT:
        posicaoX += PASSO_MOVIMENTO;
        break;

    case GLUT_KEY_UP:
        posicaoY += PASSO_MOVIMENTO;
        break;

    case GLUT_KEY_DOWN:
        posicaoY -= PASSO_MOVIMENTO;
        break;

    case GLUT_KEY_PAGE_UP:
        posicaoZ += PASSO_MOVIMENTO;
        break;

    case GLUT_KEY_PAGE_DOWN:
        posicaoZ -= PASSO_MOVIMENTO;
        break;

    default:
        alterouTransformacao = false;
        break;
    }

    if (alterouTransformacao) {
        glutPostRedisplay();
    }
}


// Calcula o enquadramento inicial do OBJ.
// Mantem a logica existente na develop.
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

    // Ignora os 1% menores e maiores valores
    // para reduzir a influencia de pontos extremos.
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