#include <GL/glut.h>

#include "Cubo.hpp"

Cubo cubo;

// Valores utilizados pelos controles
const double PASSO_TRANSLACAO = 0.2;
const double FATOR_AUMENTO = 1.1;
const double FATOR_REDUCAO = 0.9;
const double PASSO_ROTACAO = 5.0;

void initGL();
void display();
void reshape(GLsizei width, GLsizei height);

void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);

int main(int argc, char** argv) {

    cubo = criar_cubo(
        0.0,
        0.0,
        -6.0, // Posiciona o cubo na frente da camers
        2.0
    );

    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_DEPTH
    );

    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("3D Shapes");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Eventos de teclado
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);

    initGL();

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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void display() {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    desenhar_cubo(cubo);

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {

    if (height == 0) {
        height = 1;
    }

    GLfloat aspect =
        (GLfloat)width / (GLfloat)height;

    glViewport(
        0,
        0,
        width,
        height
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        45.0f,
        aspect,
        0.1f,
        100.0f
    );
}

// Teclas comuns
void keyboard(unsigned char key, int x, int y) {

    switch (key) {

        // ESC
    case 27:
        exit(0);
        break;


        // Escala

    case 'o':
    case 'O':
        escalar_cubo(
            cubo,
            FATOR_REDUCAO,
            FATOR_REDUCAO,
            FATOR_REDUCAO
        );
        break;

    case 'p':
    case 'P':
        escalar_cubo(
            cubo,
            FATOR_AUMENTO,
            FATOR_AUMENTO,
            FATOR_AUMENTO
        );
        break;


        // Rotacao no eixo X

    case 'x':
        rotacionar_cubo_x(
            cubo,
            PASSO_ROTACAO
        );
        break;

    case 'X':
        rotacionar_cubo_x(
            cubo,
            -PASSO_ROTACAO
        );
        break;


        // Rotacao no eixo Y

    case 'y':
        rotacionar_cubo_y(
            cubo,
            PASSO_ROTACAO
        );
        break;

    case 'Y':
        rotacionar_cubo_y(
            cubo,
            -PASSO_ROTACAO
        );
        break;


        // Rotacao no eixo Z

    case 'z':
        rotacionar_cubo_z(
            cubo,
            PASSO_ROTACAO
        );
        break;

    case 'Z':
        rotacionar_cubo_z(
            cubo,
            -PASSO_ROTACAO
        );
        break;


    default:
        return;
    }

    // Solicita o redesenho depois da transformacao
    glutPostRedisplay();
}

// Teclas especiais
void keyboard_special(int key, int x, int y) {

    switch (key) {

        // Eixo X

    case GLUT_KEY_LEFT:
        movimentar_cubo(
            cubo,
            -PASSO_TRANSLACAO,
            0.0,
            0.0
        );
        break;

    case GLUT_KEY_RIGHT:
        movimentar_cubo(
            cubo,
            PASSO_TRANSLACAO,
            0.0,
            0.0
        );
        break;


        // Eixo Y

    case GLUT_KEY_UP:
        movimentar_cubo(
            cubo,
            0.0,
            PASSO_TRANSLACAO,
            0.0
        );
        break;

    case GLUT_KEY_DOWN:
        movimentar_cubo(
            cubo,
            0.0,
            -PASSO_TRANSLACAO,
            0.0
        );
        break;


        // Eixo Z

    case GLUT_KEY_PAGE_UP:
        movimentar_cubo(
            cubo,
            0.0,
            0.0,
            PASSO_TRANSLACAO
        );
        break;

    case GLUT_KEY_PAGE_DOWN:
        movimentar_cubo(
            cubo,
            0.0,
            0.0,
            -PASSO_TRANSLACAO
        );
        break;


    default:
        return;
    }

    // Solicita o redesenho depois da transformacao
    glutPostRedisplay();
}