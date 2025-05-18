#include "SolucionadorGauss.h"
#include <cmath>
#include <stdexcept>
void SolucionadorGauss::resolver(Fluxo& fluxo, const Matriz& matriz, const DadosNucleares& dadosNucleares, const Criterios& criterios) {
    int numeroIteracaoInterna = 0;
    double desvioFluxo;

    do {
        numeroIteracaoInterna++;
        fluxo.atualizarFluxoAnterior();

        // Primeira malha
        std::vector<double> s(2);
        s[0] = fluxo.getFonte(0, 1);

        fluxo.fluxo[0][0] = (1.0 / matriz.getCoefA(0, 1)) *
            (s[0] - matriz.getCoefD(0, 1) * fluxo.fluxoAnterior[1][0]);

        s[1] = dadosNucleares.getSecaoChoqueEspalhamentoMalha(0, 2) * fluxo.fluxo[0][0];

        fluxo.fluxo[0][1] = (1.0 / matriz.getCoefA(0, 2)) *
            (s[1] - matriz.getCoefD(0, 2) * fluxo.fluxoAnterior[1][1]);

        // Segunda à penúltima malhas
        for (size_t m = 1; m < fluxo.fluxo.size() - 1; m++) {
            s[0] = fluxo.getFonte(m, 1);

            fluxo.fluxo[m][0] = (1.0 / matriz.getCoefA(static_cast<int>(m), 1)) *
                (s[0] - matriz.getCoefE(m, 1) * fluxo.fluxo[m - 1][0] -
                    matriz.getCoefD(m, 1) * fluxo.fluxoAnterior[m + 1][0]);

            s[1] = dadosNucleares.getSecaoChoqueEspalhamentoMalha(m, 2) * fluxo.fluxoAnterior[m][0];

            fluxo.fluxo[m][1] = (1.0 / matriz.getCoefA(m, 2)) *
                (s[1] - matriz.getCoefE(m, 2) * fluxo.fluxo[m - 1][1] -
                    matriz.getCoefD(m, 2) * fluxo.fluxoAnterior[m + 1][1]);
        }

        // Última malha
        int ultimaMalha = fluxo.fluxo.size() - 1;
        s[0] = fluxo.getFonte(ultimaMalha, 1);

        fluxo.fluxo[ultimaMalha][0] = (1.0 / matriz.getCoefA(ultimaMalha, 1)) *
            (s[0] - matriz.getCoefE(ultimaMalha, 1) * fluxo.fluxo[ultimaMalha - 1][0]);

        s[1] = dadosNucleares.getSecaoChoqueEspalhamentoMalha(ultimaMalha, 2) * fluxo.fluxoAnterior[ultimaMalha][0];

        fluxo.fluxo[ultimaMalha][1] = (1.0 / matriz.getCoefA(ultimaMalha, 2)) *
            (s[1] - matriz.getCoefE(ultimaMalha, 2) * fluxo.fluxo[ultimaMalha - 1][1]);

        // Verificar se excedeu o número máximo de iterações internas
        if (numeroIteracaoInterna > criterios.getNumeroMaximoInterna()) {
            throw std::runtime_error("Excedeu o numero maximo de iteracoes internas");
        }

        // Teste de convergência do fluxo de nêutrons
        desvioFluxo = fluxo.calcularDesvioMaximo();

    } while (desvioFluxo > criterios.getToleranciaFluxoInterna());
}