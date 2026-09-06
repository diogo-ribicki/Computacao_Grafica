#include <GL/glut.h>

#include "Cubo.hpp"

Cubo cubo;

void initGL();
void display();
void reshape(GLsizei width, GLsizei height);

int main(int argc, char** argv) {

    cubo = criar_cubo(
        0.0,
        0.0,
        -6.0,
        2.0
    );

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("3D Shapes");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

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