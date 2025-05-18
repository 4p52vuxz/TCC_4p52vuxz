#include "FatorMultiplicacao.h"
#include <cmath>

FatorMultiplicacao::FatorMultiplicacao() :
    keff(0.0),
    keffAnterior(0.0),
    somaFonteFissao(0.0),
    somaFonteFissaoAnterior(0.0) {
}

void FatorMultiplicacao::inicializar() {
    keff = 1.0;
    keffAnterior = 1.0;
    somaFonteFissao = 0.0;
    somaFonteFissaoAnterior = 0.0;
}

void FatorMultiplicacao::calcularKeff() {
    keff = keffAnterior * (somaFonteFissao / somaFonteFissaoAnterior);
}

double FatorMultiplicacao::getDesvioKeff() const {
    return std::abs(1.0 - keffAnterior / keff);
}

double FatorMultiplicacao::getKeff() const {
    return keff;
}

double FatorMultiplicacao::getSomaFonteFissao() const {
    return somaFonteFissao;
}

void FatorMultiplicacao::setKeff(double valor) {
    keff = valor;
}

void FatorMultiplicacao::setKeffAnterior(double valor) {
    keffAnterior = valor;
}

void FatorMultiplicacao::setSomaFonteFissao(double valor) {
    somaFonteFissao = valor;
}

void FatorMultiplicacao::setSomaFonteFissaoAnterior(double valor) {
    somaFonteFissaoAnterior = valor;
}