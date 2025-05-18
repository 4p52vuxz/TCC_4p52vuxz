#include "Matriz.h"
#include <cmath>

Matriz::Matriz(int numeroTotalMalhas) {
    coefA.resize(numeroTotalMalhas);
    coefE.resize(numeroTotalMalhas);
    coefD.resize(numeroTotalMalhas);

    for (int i = 0; i < numeroTotalMalhas; i++) {
        coefA[i].resize(2, 0.0);
        coefE[i].resize(2, 0.0);
        coefD[i].resize(2, 0.0);
    }
}

void Matriz::montarMatriz(const DadosNucleares& dadosNucleares, const Nucleo& nucleo) {
    int numeroTotalMalhas = nucleo.getNumeroTotalMalhas();
    double alfa, beta;

    // Primeira malha (contorno à esquerda)
    if (nucleo.getContornoEsquerda() == 0) {
        alfa = 3.0;
    }
    else {
        alfa = 1.0;
    }

    coefD[0][0] = -dadosNucleares.getCoefDifusaoMalha(0, 1) / std::pow(nucleo.getDeltaX(0), 2);
    coefD[0][1] = -dadosNucleares.getCoefDifusaoMalha(0, 2) / std::pow(nucleo.getDeltaX(0), 2);

    coefA[0][0] = dadosNucleares.getSecaoChoqueRemocaoMalha(0, 1) - alfa * coefD[0][0];
    coefA[0][1] = dadosNucleares.getSecaoChoqueRemocaoMalha(0, 2) - alfa * coefD[0][1];

    // Segunda à penúltima malhas
    for (int m = 1; m < numeroTotalMalhas - 1; m++) {
        std::vector<double> denominadorMenos(2);
        std::vector<double> denominadorMais(2);

        denominadorMenos[0] = nucleo.getDeltaX(m) * (dadosNucleares.getCoefDifusaoMalha(m, 1) * nucleo.getDeltaX(m - 1) +
            dadosNucleares.getCoefDifusaoMalha(m - 1, 1) * nucleo.getDeltaX(m));
        denominadorMenos[1] = nucleo.getDeltaX(m) * (dadosNucleares.getCoefDifusaoMalha(m, 2) * nucleo.getDeltaX(m - 1) +
            dadosNucleares.getCoefDifusaoMalha(m - 1, 2) * nucleo.getDeltaX(m));

        denominadorMais[0] = nucleo.getDeltaX(m) * (dadosNucleares.getCoefDifusaoMalha(m, 1) * nucleo.getDeltaX(m + 1) +
            dadosNucleares.getCoefDifusaoMalha(m + 1, 1) * nucleo.getDeltaX(m));
        denominadorMais[1] = nucleo.getDeltaX(m) * (dadosNucleares.getCoefDifusaoMalha(m, 2) * nucleo.getDeltaX(m + 1) +
            dadosNucleares.getCoefDifusaoMalha(m + 1, 2) * nucleo.getDeltaX(m));

        coefE[m][0] = (-2.0 * dadosNucleares.getCoefDifusaoMalha(m, 1) * dadosNucleares.getCoefDifusaoMalha(m - 1, 1)) / denominadorMenos[0];
        coefE[m][1] = (-2.0 * dadosNucleares.getCoefDifusaoMalha(m, 2) * dadosNucleares.getCoefDifusaoMalha(m - 1, 2)) / denominadorMenos[1];

        coefD[m][0] = (-2.0 * dadosNucleares.getCoefDifusaoMalha(m, 1) * dadosNucleares.getCoefDifusaoMalha(m + 1, 1)) / denominadorMais[0];
        coefD[m][1] = (-2.0 * dadosNucleares.getCoefDifusaoMalha(m, 2) * dadosNucleares.getCoefDifusaoMalha(m + 1, 2)) / denominadorMais[1];

        coefA[m][0] = dadosNucleares.getSecaoChoqueRemocaoMalha(m, 1) - coefE[m][0] - coefD[m][0];
        coefA[m][1] = dadosNucleares.getSecaoChoqueRemocaoMalha(m, 2) - coefE[m][1] - coefD[m][1];
    }

    // Última malha (contorno à direita)
    if (nucleo.getContornoDireita() == 0) {
        beta = 3.0;
    }
    else {
        beta = 1.0;
    }

    int ultimaMalha = numeroTotalMalhas - 1;

    coefE[ultimaMalha][0] = -dadosNucleares.getCoefDifusaoMalha(ultimaMalha, 1) / std::pow(nucleo.getDeltaX(ultimaMalha), 2);
    coefE[ultimaMalha][1] = -dadosNucleares.getCoefDifusaoMalha(ultimaMalha, 2) / std::pow(nucleo.getDeltaX(ultimaMalha), 2);

    coefA[ultimaMalha][0] = dadosNucleares.getSecaoChoqueRemocaoMalha(ultimaMalha, 1) - beta * coefE[ultimaMalha][0];
    coefA[ultimaMalha][1] = dadosNucleares.getSecaoChoqueRemocaoMalha(ultimaMalha, 2) - beta * coefE[ultimaMalha][1];
}

double Matriz::getCoefA(int malha, int grupo) const {
    return coefA[malha][grupo - 1];
}

double Matriz::getCoefE(int malha, int grupo) const {
    return coefE[malha][grupo - 1];
}

double Matriz::getCoefD(int malha, int grupo) const {
    return coefD[malha][grupo - 1];
}