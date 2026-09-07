# Computação Gráfica - Controles do Cubo 3D

Este projeto utiliza **OpenGL com GLUT/freeglut** para desenhar e transformar manualmente um cubo 3D.

As transformações são realizadas matematicamente sobre os vértices do cubo

## Controles de teclado

| Tecla | Ação |
|---|---|
| `←` | Mover para a esquerda no eixo X |
| `→` | Mover para a direita no eixo X |
| `↑` | Mover para cima no eixo Y |
| `↓` | Mover para baixo no eixo Y |
| `Page Up` | Mover no sentido positivo do eixo Z |
| `Page Down` | Mover no sentido negativo do eixo Z |
| `P` | Aumentar a escala do cubo |
| `O` | Reduzir a escala do cubo |
| `X` | Rotacionar +5° no eixo X |
| `Shift + X` | Rotacionar -5° no eixo X |
| `Y` | Rotacionar +5° no eixo Y |
| `Shift + Y` | Rotacionar -5° no eixo Y |
| `Z` | Rotacionar +5° no eixo Z |
| `Shift + Z` | Rotacionar -5° no eixo Z |
| `ESC` | Fechar o programa |

## Transformações implementadas

- Translação nos eixos X, Y e Z
- Escala em relação ao centro do cubo
- Rotação em torno dos eixos X, Y e Z
- Visualização 3D em perspectiva
- Desenho wireframe utilizando `GL_LINES`