# Computação Gráfica

Projeto desenvolvido em C++ utilizando OpenGL e GLUT/freeglut para a disciplina de Computação Gráfica.

O projeto contempla as funcionalidades desenvolvidas durante as avaliações M1 e M2.

## M2 - 3D e Arquivos OBJ

A aplicação permite carregar e visualizar modelos tridimensionais a partir de arquivos `.obj`, aplicando transformações, texturas e iluminação.

### Funcionalidades

- Leitura de vértices (`v`), coordenadas de textura (`vt`), normais (`vn`) e faces (`f`).
- Tratamento de índices OBJ, incluindo índices negativos.
- Renderização de faces triangulares utilizando `GL_TRIANGLES`.
- Triangulação de faces com mais de três vértices.
- Carregamento de imagens BMP para texturização.
- Aplicação de coordenadas UV e normais durante a renderização.
- Translação, rotação e escala tridimensional.
- Visualização em perspectiva.
- Três fontes de luz com componentes ambiente, difusa e especular.
- Ativação e desativação individual das fontes de luz.

### Controles de teclado

| Tecla | Ação |
|---|---|
| `←` | Mover no eixo X negativo |
| `→` | Mover no eixo X positivo |
| `↑` | Mover no eixo Y positivo |
| `↓` | Mover no eixo Y negativo |
| `Page Up` | Mover no eixo Z positivo |
| `Page Down` | Mover no eixo Z negativo |
| `P` | Aumentar a escala |
| `O` | Reduzir a escala |
| `x` | Rotacionar +5° no eixo X |
| `Shift + X` | Rotacionar -5° no eixo X |
| `y` | Rotacionar +5° no eixo Y |
| `Shift + Y` | Rotacionar -5° no eixo Y |
| `z` | Rotacionar +5° no eixo Z |
| `Shift + Z` | Rotacionar -5° no eixo Z |
| `1` | Ativar/desativar a luz 0 |
| `2` | Ativar/desativar a luz 1 |
| `3` | Ativar/desativar a luz 2 |
| `ESC` | Fechar a aplicação |

### Iluminação

A cena possui três fontes luminosas independentes:

- **Luz 0 (`GL_LIGHT0`):** luz branca, posicionada acima e à esquerda.
- **Luz 1 (`GL_LIGHT1`):** luz azulada, posicionada à direita.
- **Luz 2 (`GL_LIGHT2`):** luz quente, posicionada abaixo e à frente.

Cada fonte possui componentes ambiente, difusa e especular.

As três luzes iniciam ativadas e podem ser alternadas individualmente pelas teclas `1`, `2` e `3`.

### Modelos e texturas

O modelo inicial utilizado pela aplicação é `data/radar.obj`, com a textura `Textures/launch.bmp`.

Os arquivos estão localizados nas pastas `data` e `Textures`, respectivamente.

O modelo e a textura inicial podem ser alterados pelas constantes `CAMINHO_MODELO` e `CAMINHO_TEXTURA` no arquivo `main.cpp`.

A aplicação utiliza uma textura BMP configurada separadamente dos materiais originais dos arquivos OBJ.

## M1 - Funções Básicas

Na primeira avaliação foi desenvolvido um cubo tridimensional utilizando operações matemáticas aplicadas diretamente aos vértices.

Funcionalidades implementadas:

- Cubo com 8 vértices e 12 arestas.
- Desenho wireframe utilizando `GL_LINES`.
- Visualização em perspectiva.
- Translação nos eixos X, Y e Z.
- Escala em relação ao centro do cubo.
- Rotação nos eixos X, Y e Z.
- Transformações matemáticas sem utilizar `glTranslate`, `glRotate` ou `glScale`.

O código da M1 permanece preservado no projeto como histórico do desenvolvimento. A aplicação principal executa atualmente as funcionalidades da M2.

## Execução

O projeto utiliza Visual Studio com C++, OpenGL e freeglut.

1. Abrir `ComputacaoGrafica.sln` no Visual Studio.
2. Selecionar a plataforma `x64`.
3. Selecionar a configuração `Debug` ou `Release`.
4. Compilar e executar a aplicação.

Também é possível utilizar `ComputacaoGrafica/build_and_run.bat` para compilar e executar a configuração Debug x64 em um ambiente compatível.

Os caminhos relativos dos arquivos OBJ e BMP consideram a pasta `ComputacaoGrafica` como diretório de trabalho.