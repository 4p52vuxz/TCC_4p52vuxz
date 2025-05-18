#pragma once
#include <vector>
#include "DadosNucleares.h"
#include "Nucleo.h"


class Matriz {
private:
    std::vector<std::vector<double>> coefA;
    std::vector<std::vector<double>> coefE;
    std::vector<std::vector<double>> coefD;

public:
    Matriz(int numeroTotalMalhas);

    void montarMatriz(const DadosNucleares& dadosNucleares, const Nucleo& nucleo);

    double getCoefA(int malha, int grupo) const;
    double getCoefE(int malha, int grupo) const;
    double getCoefD(int malha, int grupo) const;
};