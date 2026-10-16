#pragma once

#include <string>
#include <GL/glut.h>

class TexturaBMP {
public:
    // Le o BMP e envia seus pixels ao OpenGL.
    // Deve ser chamada depois da criacao da janela.
    bool carregar(
        const std::string& caminho,
        std::string& mensagemErro
    );

    // Ativa e vincula a textura para o desenho.
    void ativar() const;

    // Libera o identificador da textura OpenGL.
    void liberar();

    // Informa se existe uma textura carregada.
    bool carregada() const;

    // Impede a copia do identificador OpenGL.
    TexturaBMP() = default;
    TexturaBMP(const TexturaBMP&) = delete;
    TexturaBMP& operator=(const TexturaBMP&) = delete;

private:
    GLuint identificador = 0;
};