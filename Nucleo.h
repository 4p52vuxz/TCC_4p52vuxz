#pragma once
#include <vector>
#include <string>


class Nucleo {
private:
    int numeroRegiao;
    int numeroTotalMalhas;
    int contornoEsquerda;
    int contornoDireita;
    std::vector<double> tamanhoRegiao;
    std::vector<int> tipoRegiao;
    std::vector<int> numeroParticao;
    std::vector<double> deltaX;

public:
    Nucleo();

    void carregarDeArquivo(const std::string& nomeArquivo);
    void calcularMalhas();

    int getNumeroRegiao() const;
    int getNumeroTotalMalhas() const;
    int getContornoEsquerda() const;
    int getContornoDireita() const;
    double getTamanhoRegiao(int regiao) const;
    int getTipoRegiao(int regiao) const;
    int getNumeroParticao(int regiao) const;
    double getDeltaX(int malha) const;
};