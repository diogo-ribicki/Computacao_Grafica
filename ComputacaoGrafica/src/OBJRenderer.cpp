#include "OBJRenderer.hpp"

#include <cstddef>
#include <GL/glut.h>

namespace {

    // Envia para o OpenGL todos os atributos
    // associados a um vertice de uma face OBJ.
    void enviarVertice(
        const ModeloOBJ& modelo,
        const IndiceFaceOBJ& indice
    ) {
        // Posicao do vertice.
        const VerticeOBJ& vertice =
            modelo.vertices[indice.vertice];


        // Normal opcional.
        //
        // O loader utiliza -1 quando a face
        // nao possui indice de normal.
        if (indice.normal >= 0) {

            const NormalOBJ& normal =
                modelo.normais[indice.normal];

            glNormal3f(
                static_cast<GLfloat>(normal.x),
                static_cast<GLfloat>(normal.y),
                static_cast<GLfloat>(normal.z)
            );
        }


        // Coordenada de textura opcional.
        //
        // Ainda nao carregamos nenhuma imagem
        // de textura nesta etapa.
        //
        // Mesmo assim, deixamos a coordenada
        // associada ao vertice preparada.
        if (indice.textura >= 0) {

            const CoordenadaTexturaOBJ& textura =
                modelo.texturas[indice.textura];

            glTexCoord2f(
                static_cast<GLfloat>(textura.u),
                static_cast<GLfloat>(textura.v)
            );
        }


        // A posicao deve ser enviada depois
        // dos demais atributos do vertice.
        glVertex3f(
            static_cast<GLfloat>(vertice.x),
            static_cast<GLfloat>(vertice.y),
            static_cast<GLfloat>(vertice.z)
        );
    }

}


void desenharModeloOBJ(
    const ModeloOBJ& modelo
) {
    glBegin(GL_TRIANGLES);

    for (const FaceOBJ& face : modelo.faces) {

        // O loader ja rejeita faces com menos
        // de tres vertices.
        //
        // Esta verificacao apenas protege a
        // funcao caso um ModeloOBJ seja criado
        // manualmente.
        if (face.indices.size() < 3) {
            continue;
        }

        // Triangulacao em fan.
        //
        // Face:
        //
        // 0 1 2 3 4
        //
        // Triangulos:
        //
        // 0 1 2
        // 0 2 3
        // 0 3 4
        for (
            std::size_t i = 1;
            i + 1 < face.indices.size();
            i++
            ) {
            enviarVertice(
                modelo,
                face.indices[0]
            );

            enviarVertice(
                modelo,
                face.indices[i]
            );

            enviarVertice(
                modelo,
                face.indices[i + 1]
            );
        }
    }

    glEnd();
}