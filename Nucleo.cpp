#include "Nucleo.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Nucleo::Nucleo() :
    numeroRegiao(0),
    numeroTotalMalhas(0),
    contornoEsquerda(0),
    contornoDireita(0) {
}

void Nucleo::carregarDeArquivo(const std::string& nomeArquivo) {
    try {
        // Verifica se o arquivo é JSON
        if (nomeArquivo.substr(nomeArquivo.size() - 5) == ".json") {
            std::ifstream arquivo(nomeArquivo);
            nlohmann::json j;
            arquivo >> j;

            contornoEsquerda = j["contornoEsquerda"];
            contornoDireita = j["contornoDireita"];
            numeroRegiao = j["numeroRegiao"];

            tamanhoRegiao.resize(numeroRegiao);
            tipoRegiao.resize(numeroRegiao);
            numeroParticao.resize(numeroRegiao);

            for (int i = 0; i < numeroRegiao; i++) {
                tipoRegiao[i] = j["tipoRegiao"][i];
                tamanhoRegiao[i] = j["tamanhoRegiao"][i];
                numeroParticao[i] = j["numeroParticao"][i];
            }
        }
        else {
            // Formato antigo de arquivo .txt
            std::ifstream arquivo(nomeArquivo);
            arquivo >> contornoEsquerda >> contornoDireita;
            arquivo >> numeroRegiao;

            tamanhoRegiao.resize(numeroRegiao);
            tipoRegiao.resize(numeroRegiao);
            numeroParticao.resize(numeroRegiao);

            for (int i = 0; i < numeroRegiao; i++) {
                arquivo >> tipoRegiao[i];
            }

            for (int i = 0; i < numeroRegiao; i++) {
                arquivo >> tamanhoRegiao[i];
            }

            for (int i = 0; i < numeroRegiao; i++) {
                arquivo >> numeroParticao[i];
            }
        }

        // Calcular o número total de malhas
        calcularMalhas();
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar dados do nucleo: " << e.what() << std::endl;
    }
}

void Nucleo::calcularMalhas() {
    // Calcular número total de malhas
    numeroTotalMalhas = 0;
    for (int i = 0; i < numeroRegiao; i++) {
        numeroTotalMalhas += numeroParticao[i];
    }

    // Redimensionar o vetor de delta X
    deltaX.resize(numeroTotalMalhas);

    // Calcular delta X para cada malha
    int malhaAtual = 0;
    for (int i = 0; i < numeroRegiao; i++) {
        double deltaParcial = tamanhoRegiao[i] / numeroParticao[i];
        for (int j = 0; j < numeroParticao[i]; j++) {
            deltaX[malhaAtual] = deltaParcial;
            malhaAtual++;
        }
    }
}

int Nucleo::getNumeroRegiao() const {
    return numeroRegiao;
}

int Nucleo::getNumeroTotalMalhas() const {
    return numeroTotalMalhas;
}

int Nucleo::getContornoEsquerda() const {
    return contornoEsquerda;
}

int Nucleo::getContornoDireita() const {
    return contornoDireita;
}

double Nucleo::getTamanhoRegiao(int regiao) const {
    return tamanhoRegiao[regiao - 1];
}

int Nucleo::getTipoRegiao(int regiao) const {
    return tipoRegiao[regiao - 1];
}

int Nucleo::getNumeroParticao(int regiao) const {
    return numeroParticao[regiao - 1];
}

double Nucleo::getDeltaX(int malha) const {
    return deltaX[malha];
}