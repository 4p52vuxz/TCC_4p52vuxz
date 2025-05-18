#include "Fluxo.h"
#include <cmath>

Fluxo::Fluxo(int numeroTotalMalhas) :
    metodo(0),
    numeroIteracaoExterna(0) {

    fluxo.resize(numeroTotalMalhas);
    fluxoAnterior.resize(numeroTotalMalhas);
    fonte.resize(numeroTotalMalhas);

    for (int i = 0; i < numeroTotalMalhas; i++) {
        fluxo[i].resize(2, 0.0);
        fluxoAnterior[i].resize(2, 0.0);
        fonte[i].resize(2, 0.0);
    }
}

void Fluxo::inicializar() {
    // Inicializar fluxo com valor 1.0
    for (size_t i = 0; i < fluxo.size(); i++) {
        fluxo[i][0] = 1.0;
        fluxo[i][1] = 1.0;
    }
}

void Fluxo::atualizarFluxoAnterior() {
    for (size_t i = 0; i < fluxo.size(); i++) {
        fluxoAnterior[i][0] = fluxo[i][0];
        fluxoAnterior[i][1] = fluxo[i][1];
    }
}

double Fluxo::calcularDesvioMaximo() const {
    double desvioMaximo = 0.0;

    for (size_t i = 0; i < fluxo.size(); i++) {
        for (int j = 0; j < 2; j++) {
            if (fluxo[i][j] != 0.0) {
                double desvio = std::abs(1.0 - fluxoAnterior[i][j] / fluxo[i][j]);
                if (desvio > desvioMaximo) {
                    desvioMaximo = desvio;
                }
            }
        }
    }

    return desvioMaximo;
}

int Fluxo::getMetodo() const {
    return metodo;
}

int Fluxo::getNumeroIteracaoExterna() const {
    return numeroIteracaoExterna;
}

double Fluxo::getFluxo(int malha, int grupo) const {
    return fluxo[malha][grupo - 1];
}

double Fluxo::getFonte(int malha, int grupo) const {
    return fonte[malha][grupo - 1];
}

void Fluxo::setMetodo(int valor) {
    metodo = valor;
}

void Fluxo::incrementarIteracaoExterna() {
    numeroIteracaoExterna++;
}

void Fluxo::setNumeroIteracaoExterna(int valor) {
    numeroIteracaoExterna = valor;
}

void Fluxo::setFluxo(int malha, int grupo, double valor) {
    fluxo[malha][grupo - 1] = valor;
}

void Fluxo::setFonte(int malha, int grupo, double valor) {
    fonte[malha][grupo - 1] = valor;
}