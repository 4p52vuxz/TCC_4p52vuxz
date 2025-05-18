#pragma once
#include <vector>
#include "DadosNucleares.h"
#include "Fluxo.h"
#include "Nucleo.h"
#include "FatorMultiplicacao.h"


class Fonte {
private:
    std::vector<std::vector<double>> fonteFissao;

public:
    Fonte(int numeroTotalMalhas);

    void calcularFonteFissao(const DadosNucleares& dadosNucleares, const Fluxo& fluxo, const Nucleo& nucleo);
    void calcularFonte(Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao);
    double somarTermoFonteFissao(const DadosNucleares& dadosNucleares, const Nucleo& nucleo);

    double getFonteFissao(int malha, int grupo) const;
};