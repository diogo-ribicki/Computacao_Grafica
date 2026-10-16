#include "TexturaBMP.hpp"

#include <cstdint>
#include <fstream>
#include <vector>

namespace {

    // Le um inteiro de 16 bits em little-endian.
    bool ler16(
        std::ifstream& arquivo,
        std::uint16_t& valor
    ) {
        unsigned char bytes[2];

        if (!arquivo.read(
            reinterpret_cast<char*>(bytes),
            2
        )) {
            return false;
        }

        valor =
            static_cast<std::uint16_t>(bytes[0]) |
            (static_cast<std::uint16_t>(bytes[1]) << 8);

        return true;
    }

    // Le um inteiro de 32 bits em little-endian.
    bool ler32(
        std::ifstream& arquivo,
        std::uint32_t& valor
    ) {
        unsigned char bytes[4];

        if (!arquivo.read(
            reinterpret_cast<char*>(bytes),
            4
        )) {
            return false;
        }

        valor =
            static_cast<std::uint32_t>(bytes[0]) |
            (static_cast<std::uint32_t>(bytes[1]) << 8) |
            (static_cast<std::uint32_t>(bytes[2]) << 16) |
            (static_cast<std::uint32_t>(bytes[3]) << 24);

        return true;
    }

    // Estrutura temporaria usada durante a leitura.
    struct ImagemBMP {
        GLsizei largura = 0;
        GLsizei altura = 0;
        std::vector<unsigned char> pixels;
    };

    // Carrega os pixels em formato RGB.
    bool lerImagemBMP(
        const std::string& caminho,
        ImagemBMP& imagem,
        std::string& mensagemErro
    ) {
        std::ifstream arquivo(
            caminho,
            std::ios::binary
        );

        if (!arquivo.is_open()) {
            mensagemErro =
                "Nao foi possivel abrir o BMP: " +
                caminho;

            return false;
        }

        // Verifica a assinatura BM.
        char assinatura[2];

        if (!arquivo.read(assinatura, 2) ||
            assinatura[0] != 'B' ||
            assinatura[1] != 'M') {

            mensagemErro =
                "Arquivo nao e um BMP valido.";

            return false;
        }

        std::uint32_t offsetPixels = 0;
        std::uint32_t tamanhoCabecalho = 0;
        std::uint32_t larguraBruta = 0;
        std::uint32_t alturaBruta = 0;
        std::uint32_t compressao = 0;

        std::uint16_t planos = 0;
        std::uint16_t bitsPorPixel = 0;

        // Offset dos pixels: byte 10.
        arquivo.seekg(10, std::ios::beg);

        if (!ler32(arquivo, offsetPixels) ||
            !ler32(arquivo, tamanhoCabecalho)) {

            mensagemErro =
                "Erro ao ler cabecalho do BMP.";

            return false;
        }

        // Esta implementacao aceita o cabecalho
        // BITMAPINFOHEADER ou compativel.
        if (tamanhoCabecalho < 40) {
            mensagemErro =
                "Cabecalho BMP nao suportado.";

            return false;
        }

        // Largura e altura comecam no byte 18.
        arquivo.seekg(18, std::ios::beg);

        if (!ler32(arquivo, larguraBruta) ||
            !ler32(arquivo, alturaBruta) ||
            !ler16(arquivo, planos) ||
            !ler16(arquivo, bitsPorPixel) ||
            !ler32(arquivo, compressao)) {

            mensagemErro =
                "Erro ao ler dimensoes do BMP.";

            return false;
        }

        // Limita a leitura a BMP RGB de 24 bits,
        // sem compactacao.
        if (planos != 1 ||
            bitsPorPixel != 24 ||
            compressao != 0) {

            mensagemErro =
                "Utilize BMP de 24 bits sem compressao.";

            return false;
        }

        // Altura negativa significa que o BMP
        // armazena as linhas de cima para baixo.
        const bool origemSuperior =
            (alturaBruta & 0x80000000u) != 0;

        const std::uint32_t alturaReal =
            origemSuperior
            ? (0u - alturaBruta)
            : alturaBruta;

        if (larguraBruta == 0 || alturaReal == 0) {
            mensagemErro =
                "Dimensoes invalidas no BMP.";

            return false;
        }

        if (static_cast<std::uint64_t>(offsetPixels) <
            14ull + tamanhoCabecalho) {

            mensagemErro =
                "Offset de pixels invalido no BMP.";

            return false;
        }

        // Evita carregar uma imagem maior que
        // o limite permitido pelo OpenGL.
        GLint tamanhoMaximo = 0;

        glGetIntegerv(
            GL_MAX_TEXTURE_SIZE,
            &tamanhoMaximo
        );

        if (tamanhoMaximo <= 0 ||
            larguraBruta >
            static_cast<std::uint32_t>(tamanhoMaximo) ||
            alturaReal >
            static_cast<std::uint32_t>(tamanhoMaximo)) {

            mensagemErro =
                "Dimensoes do BMP excedem o limite OpenGL.";

            return false;
        }

        imagem.largura =
            static_cast<GLsizei>(larguraBruta);

        imagem.altura =
            static_cast<GLsizei>(alturaReal);

        // Cada pixel ocupa tres bytes: B, G, R.
        const std::size_t bytesPorLinha =
            static_cast<std::size_t>(imagem.largura) * 3;

        // Linhas BMP sao alinhadas em multiplos
        // de quatro bytes.
        const std::size_t bytesLinhaBMP =
            (bytesPorLinha + 3u) & ~std::size_t(3u);

        const std::size_t totalBytes =
            bytesPorLinha *
            static_cast<std::size_t>(imagem.altura);

        imagem.pixels.resize(totalBytes);

        std::vector<unsigned char> linhaBMP(
            bytesLinhaBMP
        );

        arquivo.seekg(
            offsetPixels,
            std::ios::beg
        );

        if (!arquivo) {
            mensagemErro =
                "Offset de pixels fora do arquivo.";

            return false;
        }

        for (GLsizei y = 0; y < imagem.altura; y++) {

            if (!arquivo.read(
                reinterpret_cast<char*>(linhaBMP.data()),
                static_cast<std::streamsize>(bytesLinhaBMP)
            )) {
                mensagemErro =
                    "Dados de pixels incompletos no BMP.";

                return false;
            }

            // OpenGL espera primeiro a linha inferior.
            const GLsizei linhaDestino =
                origemSuperior
                ? imagem.altura - 1 - y
                : y;

            const std::size_t inicioDestino =
                static_cast<std::size_t>(linhaDestino) *
                bytesPorLinha;

            for (GLsizei x = 0; x < imagem.largura; x++) {

                const std::size_t origem =
                    static_cast<std::size_t>(x) * 3;

                const std::size_t destino =
                    inicioDestino + origem;

                // Conversao de BGR para RGB.
                imagem.pixels[destino] =
                    linhaBMP[origem + 2];

                imagem.pixels[destino + 1] =
                    linhaBMP[origem + 1];

                imagem.pixels[destino + 2] =
                    linhaBMP[origem];
            }
        }

        return true;
    }

}


bool TexturaBMP::carregar(
    const std::string& caminho,
    std::string& mensagemErro
) {
    mensagemErro.clear();

    // Libera uma textura anterior, se existir.
    liberar();

    ImagemBMP imagem;

    if (!lerImagemBMP(
        caminho,
        imagem,
        mensagemErro
    )) {
        return false;
    }

    // Gera um identificador OpenGL.
    glGenTextures(
        1,
        &identificador
    );

    if (identificador == 0) {
        mensagemErro =
            "Falha ao gerar textura OpenGL.";

        return false;
    }

    // Seleciona a textura para configuracao.
    glBindTexture(
        GL_TEXTURE_2D,
        identificador
    );

    // Wrapping horizontal e vertical.
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    // Filtros iguais aos do exemplo do professor.
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    );

    // Combina a textura com o resultado
    // do sistema de iluminacao existente.
    glTexEnvi(
        GL_TEXTURE_ENV,
        GL_TEXTURE_ENV_MODE,
        GL_MODULATE
    );

    // Preserva o alinhamento anterior de pixels.
    GLint alinhamentoAnterior = 4;

    glGetIntegerv(
        GL_UNPACK_ALIGNMENT,
        &alinhamentoAnterior
    );

    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        1
    );

    // Transfere os dados RGB para o OpenGL.
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        imagem.largura,
        imagem.altura,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        imagem.pixels.data()
    );

    // Restaura o alinhamento anterior.
    glPixelStorei(
        GL_UNPACK_ALIGNMENT,
        alinhamentoAnterior
    );

    const GLenum erroOpenGL = glGetError();

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );

    if (erroOpenGL != GL_NO_ERROR) {
        liberar();

        mensagemErro =
            "Erro ao enviar textura BMP ao OpenGL.";

        return false;
    }

    // O OpenGL possui sua propria copia dos
    // pixels; o vetor local pode ser liberado.
    return true;
}


void TexturaBMP::ativar() const {
    if (identificador == 0) {
        glDisable(GL_TEXTURE_2D);
        return;
    }

    glEnable(GL_TEXTURE_2D);

    glBindTexture(
        GL_TEXTURE_2D,
        identificador
    );
}


void TexturaBMP::liberar() {
    if (identificador != 0) {
        glDeleteTextures(
            1,
            &identificador
        );

        identificador = 0;
    }
}


bool TexturaBMP::carregada() const {
    return identificador != 0;
}