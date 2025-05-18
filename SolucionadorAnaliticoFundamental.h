#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "Solucionador.h"
#include "Fluxo.h"
#include "DadosNucleares.h"
#include "Criterios.h"
#include "Nucleo.h"



class SolucionadorAnaliticoFundamental : public Solucionador {
private:
    const Nucleo* pNucleoConfigurado;

public:
    SolucionadorAnaliticoFundamental();


    void setContextoNucleo(const Nucleo& nucleo_obj);


    void resolver(Fluxo& fluxo,
        const Matriz& matriz_ignorada,
        const DadosNucleares& dadosNucleares,
        const Criterios& criterios_ignorados) override;
};

