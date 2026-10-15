#pragma once

#include <string>

#include "ModeloOBJ.hpp"

bool carregarOBJ(
    const std::string& caminho,
    ModeloOBJ& modelo,
    std::string& mensagemErro
);