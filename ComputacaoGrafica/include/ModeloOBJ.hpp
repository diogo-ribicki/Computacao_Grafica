#pragma once

#include <vector>

struct VerticeOBJ {
    double x;
    double y;
    double z;
};

struct CoordenadaTexturaOBJ {
    double u;
    double v;
};

struct NormalOBJ {
    double x;
    double y;
    double z;
};

struct IndiceFaceOBJ {
    int vertice = -1;
    int textura = -1;
    int normal = -1;
};

struct FaceOBJ {
    std::vector<IndiceFaceOBJ> indices;
};

struct ModeloOBJ {
    std::vector<VerticeOBJ> vertices;
    std::vector<CoordenadaTexturaOBJ> texturas;
    std::vector<NormalOBJ> normais;
    std::vector<FaceOBJ> faces;
};