#include "Fonte.h"

Fonte::Fonte(int numeroTotalMalhas) {
    fonteFissao.resize(numeroTotalMalhas);

    for (int i = 0; i < numeroTotalMalhas; i++) {
        fonteFissao[i].resize(2, 0.0);
    }
}

void Fonte::calcularFonteFissao(const DadosNucleares& dadosNucleares, const Fluxo& fluxo, const Nucleo& nucleo) {
    // Inicializa a fonte de fissão
    for (size_t m = 0; m < fonteFissao.size(); m++) {
        fonteFissao[m][0] = 0.0;
        fonteFissao[m][1] = 0.0;
    }

    // Calcula a fonte de fissão de nêutrons
    for (int m = 0; m < nucleo.getNumeroTotalMalhas(); m++) {
        if (dadosNucleares.getCodigoEC(m) == 'C') {
            fonteFissao[m][0] = dadosNucleares.getNuSecaoChoqueFissaoMalha(m, 1) * fluxo.getFluxo(m, 1) +
                dadosNucleares.getNuSecaoChoqueFissaoMalha(m, 2) * fluxo.getFluxo(m, 2);
        }
    }
}

void Fonte::calcularFonte(Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao) {
    for (size_t m = 0; m < fonteFissao.size(); m++) {
        fluxo.setFonte(m, 1, fonteFissao[m][0] / fatorMultiplicacao.getKeff());
    }
}

double Fonte::somarTermoFonteFissao(const DadosNucleares& dadosNucleares, const Nucleo& nucleo) {
    double somaFonteFissao = 0.0;

    for (int m = 0; m < nucleo.getNumeroTotalMalhas(); m++) {
        if (dadosNucleares.getCodigoEC(m) == 'C') {
            somaFonteFissao += fonteFissao[m][0] * nucleo.getDeltaX(m);
        }
    }

    return somaFonteFissao;
}

double Fonte::getFonteFissao(int malha, int grupo) const {
    return fonteFissao[malha][grupo - 1];
}