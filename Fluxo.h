#pragma once
#include <vector>


class Fluxo {
private:
    int metodo;
    int numeroIteracaoExterna;
    std::vector<std::vector<double>> fluxo;
    std::vector<std::vector<double>> fluxoAnterior;
    std::vector<std::vector<double>> fonte;

public:
    Fluxo(int numeroTotalMalhas);

    void inicializar();
    void atualizarFluxoAnterior();
    double calcularDesvioMaximo() const;

    int getMetodo() const;
    int getNumeroIteracaoExterna() const;
    double getFluxo(int malha, int grupo) const;
    double getFonte(int malha, int grupo) const;

    void setMetodo(int valor);
    void incrementarIteracaoExterna();
    void setNumeroIteracaoExterna(int valor);
    void setFluxo(int malha, int grupo, double valor);
    void setFonte(int malha, int grupo, double valor);


    friend class SolucionadorJacobi;
    friend class SolucionadorGauss;
    friend class SolucionadorAnaliticoFundamental;
};