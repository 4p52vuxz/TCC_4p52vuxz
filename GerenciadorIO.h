#pragma once
#include <string>
#include "Criterios.h"
#include "DadosNucleares.h"
#include "Nucleo.h"
#include "Fluxo.h"
#include "FatorMultiplicacao.h"


class GerenciadorIO {
public:
    static void carregarCriterios(Criterios& criterios, const std::string& nomeArquivo);
    static void carregarDadosNucleares(DadosNucleares& dadosNucleares, const std::string& nomeArquivo);
    static void carregarDadosNucleo(Nucleo& nucleo, const std::string& nomeArquivo);


    static void gravarMapaNucleo(const Nucleo& nucleo, const std::string& nomeArquivo);
    static void gravarFluxo(const Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao,
        const Nucleo& nucleo, const std::string& nomeArquivo);
    static void gravarKeff(const FatorMultiplicacao& fatorMultiplicacao, int iteracao,
        const std::string& nomeArquivo);
    static void gravarTempo(double tempo, const std::string& nomeArquivo);


    static std::string obterNomeArquivoMetodo(const std::string& baseNome, int metodo);
    static void gravarFluxoMetodo(const Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao,
        const Nucleo& nucleo, int metodo);
    static void gravarKeffMetodo(const FatorMultiplicacao& fatorMultiplicacao, int iteracao, int metodo);
    static void gravarTempoMetodo(double tempo, int metodo);

    static int lerMetodo();
};