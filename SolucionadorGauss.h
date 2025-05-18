#pragma once
#include "Solucionador.h"


class SolucionadorGauss : public Solucionador {
public:
    void resolver(Fluxo& fluxo, const Matriz& matriz, const DadosNucleares& dadosNucleares, const Criterios& criterios) override;
};