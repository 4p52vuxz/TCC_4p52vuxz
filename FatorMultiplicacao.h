#pragma once


class FatorMultiplicacao {
private:
    double keff;
    double keffAnterior;
    double somaFonteFissao;
    double somaFonteFissaoAnterior;

public:
    FatorMultiplicacao();

    void inicializar();
    void calcularKeff();
    double getDesvioKeff() const;

    double getKeff() const;
    double getSomaFonteFissao() const;

    void setKeff(double valor);
    void setKeffAnterior(double valor);
    void setSomaFonteFissao(double valor);
    void setSomaFonteFissaoAnterior(double valor);
};