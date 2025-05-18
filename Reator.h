#pragma once
#include "Criterios.h"
#include "DadosNucleares.h"
#include "Nucleo.h"
#include "Fluxo.h"
#include "Matriz.h"
#include "FatorMultiplicacao.h"
#include "Fonte.h"
#include "Solucionador.h"


class Reator {
private:
    Criterios criterios;
    DadosNucleares dadosNucleares;
    Nucleo nucleo;
    Fluxo* fluxo;
    Matriz* matriz;
    FatorMultiplicacao fatorMultiplicacao;
    Fonte* fonte;
    Solucionador* solucionador;

    void inicializarComponentes();

public:
    Reator();
    ~Reator();

    void carregarDados(const std::string& arquivoCriterios,
        const std::string& arquivoDadosNucleares,
        const std::string& arquivoDadosNucleo);
    void prepararCalculo(int metodo);
    void executarCalculo();


    void gravarResultados(const std::string& prefixo = "");
    void gravarResultadosMetodo(int metodo);
};