
#pragma once

// Configura o modelo de iluminacao,
// os materiais e as tres fontes de luz.
void inicializarIluminacao();

// Posiciona as fontes luminosas usando
// a matriz ModelView atual.
//
// Deve ser chamada depois da camera
// e antes das transformacoes do objeto.
void posicionarLuzes();

// Alterna uma luz individualmente.
//
// 0 = GL_LIGHT0
// 1 = GL_LIGHT1
// 2 = GL_LIGHT2
void alternarLuz(int numeroLuz);
