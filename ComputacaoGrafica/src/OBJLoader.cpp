#include "OBJLoader.hpp"

#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>

namespace {

    bool converterInteiro(
        const std::string& texto,
        int& valor
    ) {
        if (texto.empty()) {
            return false;
        }

        try {
            std::size_t posicao = 0;

            int convertido =
                std::stoi(texto, &posicao);

            if (posicao != texto.size()) {
                return false;
            }

            valor = convertido;

            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool resolverIndiceOBJ(
        int indiceOBJ,
        std::size_t quantidade,
        int& indiceInterno
    ) {
        // O indice 0 nao existe no formato OBJ.
        if (indiceOBJ == 0) {
            return false;
        }

        long long indiceResolvido;

        // Indices positivos comecam em 1.
        if (indiceOBJ > 0) {
            indiceResolvido =
                static_cast<long long>(indiceOBJ) - 1;
        }

        // Indices negativos sao relativos ao final
        // da lista existente naquele momento.
        else {
            indiceResolvido =
                static_cast<long long>(quantidade) +
                indiceOBJ;
        }

        if (
            indiceResolvido < 0 ||
            indiceResolvido >=
            static_cast<long long>(quantidade)
            ) {
            return false;
        }

        indiceInterno =
            static_cast<int>(indiceResolvido);

        return true;
    }

    bool interpretarIndiceFace(
        const std::string& token,
        const ModeloOBJ& modelo,
        IndiceFaceOBJ& indice,
        std::string& erro
    ) {
        if (token.empty()) {
            erro = "indice de face vazio";
            return false;
        }

        const std::size_t primeiraBarra =
            token.find('/');

        std::size_t segundaBarra =
            std::string::npos;

        if (primeiraBarra != std::string::npos) {
            segundaBarra =
                token.find(
                    '/',
                    primeiraBarra + 1
                );

            // Aceitamos no maximo:
            //
            // v
            // v/vt
            // v//vn
            // v/vt/vn
            //
            // Portanto, uma terceira barra
            // significa um formato invalido.
            if (
                segundaBarra != std::string::npos &&
                token.find(
                    '/',
                    segundaBarra + 1
                ) != std::string::npos
                ) {
                erro =
                    "formato de face invalido: " +
                    token;

                return false;
            }
        }

        std::string textoVertice;
        std::string textoTextura;
        std::string textoNormal;

        // Formato:
        //
        // v
        if (primeiraBarra == std::string::npos) {
            textoVertice = token;
        }

        // Existe pelo menos uma barra.
        else {

            textoVertice =
                token.substr(
                    0,
                    primeiraBarra
                );

            // Formato:
            //
            // v/vt
            if (segundaBarra == std::string::npos) {

                textoTextura =
                    token.substr(
                        primeiraBarra + 1
                    );

                if (textoTextura.empty()) {
                    erro =
                        "formato de face invalido: " +
                        token;

                    return false;
                }
            }

            // Formatos:
            //
            // v//vn
            // v/vt/vn
            else {

                textoTextura =
                    token.substr(
                        primeiraBarra + 1,
                        segundaBarra -
                        primeiraBarra -
                        1
                    );

                textoNormal =
                    token.substr(
                        segundaBarra + 1
                    );

                if (textoNormal.empty()) {
                    erro =
                        "formato de face invalido: " +
                        token;

                    return false;
                }
            }
        }

        int indiceLido;

        // Vertice obrigatorio
        if (
            !converterInteiro(
                textoVertice,
                indiceLido
            )
            ) {
            erro =
                "indice de vertice invalido: " +
                token;

            return false;
        }

        if (
            !resolverIndiceOBJ(
                indiceLido,
                modelo.vertices.size(),
                indice.vertice
            )
            ) {
            erro =
                "indice de vertice fora do intervalo: " +
                token;

            return false;
        }

        // Textura opcional
        if (!textoTextura.empty()) {

            if (
                !converterInteiro(
                    textoTextura,
                    indiceLido
                )
                ) {
                erro =
                    "indice de textura invalido: " +
                    token;

                return false;
            }

            if (
                !resolverIndiceOBJ(
                    indiceLido,
                    modelo.texturas.size(),
                    indice.textura
                )
                ) {
                erro =
                    "indice de textura fora do intervalo: " +
                    token;

                return false;
            }
        }

        // Normal opcional
        if (!textoNormal.empty()) {

            if (
                !converterInteiro(
                    textoNormal,
                    indiceLido
                )
                ) {
                erro =
                    "indice de normal invalido: " +
                    token;

                return false;
            }

            if (
                !resolverIndiceOBJ(
                    indiceLido,
                    modelo.normais.size(),
                    indice.normal
                )
                ) {
                erro =
                    "indice de normal fora do intervalo: " +
                    token;

                return false;
            }
        }

        return true;
    }

}

bool carregarOBJ(
    const std::string& caminho,
    ModeloOBJ& modelo,
    std::string& mensagemErro
) {
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        mensagemErro =
            "Nao foi possivel abrir o arquivo: " +
            caminho;

        return false;
    }

    // Garante que um novo carregamento nao misture
    // dados com um modelo carregado anteriormente.
    modelo = ModeloOBJ{};

    mensagemErro.clear();

    std::string linha;
    std::size_t numeroLinha = 0;

    while (std::getline(arquivo, linha)) {

        numeroLinha++;

        std::istringstream conteudoLinha(linha);

        std::string tipo;

        if (!(conteudoLinha >> tipo)) {
            continue;
        }

        // Comentario
        if (
            !tipo.empty() &&
            tipo[0] == '#'
            ) {
            continue;
        }

        // Vertice
        //
        // v x y z
        if (tipo == "v") {

            VerticeOBJ vertice{};

            if (
                !(conteudoLinha
                    >> vertice.x
                    >> vertice.y
                    >> vertice.z)
                ) {
                mensagemErro =
                    "Linha " +
                    std::to_string(numeroLinha) +
                    ": vertice invalido.";

                return false;
            }

            modelo.vertices.push_back(
                vertice
            );

            continue;
        }

        // Coordenada de textura
        //
        // vt u v
        if (tipo == "vt") {

            CoordenadaTexturaOBJ textura{};

            if (
                !(conteudoLinha
                    >> textura.u
                    >> textura.v)
                ) {
                mensagemErro =
                    "Linha " +
                    std::to_string(numeroLinha) +
                    ": coordenada de textura invalida.";

                return false;
            }

            modelo.texturas.push_back(
                textura
            );

            continue;
        }

        // Normal
        //
        // vn x y z
        if (tipo == "vn") {

            NormalOBJ normal{};

            if (
                !(conteudoLinha
                    >> normal.x
                    >> normal.y
                    >> normal.z)
                ) {
                mensagemErro =
                    "Linha " +
                    std::to_string(numeroLinha) +
                    ": normal invalida.";

                return false;
            }

            modelo.normais.push_back(
                normal
            );

            continue;
        }

        // Face
        //
        // Formatos reconhecidos:
        //
        // f v v v
        // f v/vt v/vt v/vt
        // f v//vn v//vn v//vn
        // f v/vt/vn v/vt/vn v/vt/vn
        //
        // A quantidade de vertices nao e limitada
        // a tres.
        if (tipo == "f") {

            FaceOBJ face;

            std::string token;

            while (conteudoLinha >> token) {

                // Permite comentario depois da face:
                //
                // f 1 2 3 # comentario
                if (
                    !token.empty() &&
                    token[0] == '#'
                    ) {
                    break;
                }

                IndiceFaceOBJ indice;
                std::string erroIndice;

                if (
                    !interpretarIndiceFace(
                        token,
                        modelo,
                        indice,
                        erroIndice
                    )
                    ) {
                    mensagemErro =
                        "Linha " +
                        std::to_string(numeroLinha) +
                        ": " +
                        erroIndice;

                    return false;
                }

                face.indices.push_back(
                    indice
                );
            }

            if (face.indices.size() < 3) {

                mensagemErro =
                    "Linha " +
                    std::to_string(numeroLinha) +
                    ": face com menos de 3 vertices.";

                return false;
            }

            modelo.faces.push_back(
                face
            );

            continue;
        }

        // Demais comandos OBJ nao fazem parte
        // desta etapa.
        //
        // Exemplos:
        //
        // o
        // g
        // s
        // mtllib
        // usemtl
    }

    return true;
}