#pragma once
#include <string>


class Criterios {
private:
    int numeroMaximoExterna;
    int numeroMaximoInterna;
    double toleranciaKeff;
    double toleranciaFluxoExterna;
    double toleranciaFluxoInterna;
    double kEffReferenciaAnalitico;

public:
    Criterios();

    void setNumeroMaximoExterna(int valor);
    void setNumeroMaximoInterna(int valor);
    void setToleranciaKeff(double valor);
    void setToleranciaFluxoExterna(double valor);
    void setToleranciaFluxoInterna(double valor);

    int getNumeroMaximoExterna() const;
    int getNumeroMaximoInterna() const;
    double getToleranciaKeff() const;
    double getToleranciaFluxoExterna() const;
    double getToleranciaFluxoInterna() const;
    double getkEffReferenciaAnalitico() const;

    void carregarDeArquivo(const std::string& nomeArquivo);
};