#pragma once
#include "Fluxo.h"
#include "Matriz.h"
#include "DadosNucleares.h"
#include "Criterios.h"


class Solucionador {
public:
    virtual void resolver(Fluxo& fluxo, const Matriz& matriz, const DadosNucleares& dadosNucleares, const Criterios& criterios) = 0;
    virtual ~Solucionador() {}
};