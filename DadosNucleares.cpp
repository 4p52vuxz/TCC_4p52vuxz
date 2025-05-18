#include "DadosNucleares.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

DadosNucleares::DadosNucleares() : numeroTipo(0) {
}

void DadosNucleares::carregarDeArquivo(const std::string& nomeArquivo) {
    try {
        // Verifica se o arquivo é JSON
        if (nomeArquivo.substr(nomeArquivo.size() - 5) == ".json") {
            std::ifstream arquivo(nomeArquivo);
            nlohmann::json j;
            arquivo >> j;

            numeroTipo = j["numeroTipo"];

            coefDifusao.resize(numeroTipo);
            secaoChoqueAbsorcao.resize(numeroTipo);
            secaoChoqueRemocao.resize(numeroTipo);
            nuSecaoChoqueFissao.resize(numeroTipo);
            secaoChoqueEspalhamento.resize(numeroTipo);

            for (int i = 0; i < numeroTipo; i++) {
                coefDifusao[i].resize(2);
                secaoChoqueAbsorcao[i].resize(2);
                secaoChoqueRemocao[i].resize(2);
                nuSecaoChoqueFissao[i].resize(2);
                secaoChoqueEspalhamento[i].resize(2);

                coefDifusao[i][0] = j["tipos"][i]["coefDifusao"][0];
                coefDifusao[i][1] = j["tipos"][i]["coefDifusao"][1];

                secaoChoqueAbsorcao[i][0] = j["tipos"][i]["secaoChoqueAbsorcao"][0];
                secaoChoqueAbsorcao[i][1] = j["tipos"][i]["secaoChoqueAbsorcao"][1];

                secaoChoqueEspalhamento[i][0] = j["tipos"][i]["secaoChoqueEspalhamento"][0];
                secaoChoqueEspalhamento[i][1] = j["tipos"][i]["secaoChoqueEspalhamento"][1];

                nuSecaoChoqueFissao[i][0] = j["tipos"][i]["nuSecaoChoqueFissao"][0];
                nuSecaoChoqueFissao[i][1] = j["tipos"][i]["nuSecaoChoqueFissao"][1];

                // Cálculo da seção de choque de remoção
                secaoChoqueRemocao[i][0] = secaoChoqueAbsorcao[i][0];
                secaoChoqueRemocao[i][1] = secaoChoqueAbsorcao[i][1];
            }
        }
        else {
            // Formato antigo de arquivo .txt
            std::ifstream arquivo(nomeArquivo);
            arquivo >> numeroTipo;

            coefDifusao.resize(numeroTipo);
            secaoChoqueAbsorcao.resize(numeroTipo);
            secaoChoqueRemocao.resize(numeroTipo);
            nuSecaoChoqueFissao.resize(numeroTipo);
            secaoChoqueEspalhamento.resize(numeroTipo);

            for (int i = 0; i < numeroTipo; i++) {
                coefDifusao[i].resize(2);
                secaoChoqueAbsorcao[i].resize(2);
                secaoChoqueRemocao[i].resize(2);
                nuSecaoChoqueFissao[i].resize(2);
                secaoChoqueEspalhamento[i].resize(2);

                std::string nomeTipo;
                arquivo >> nomeTipo; // Ler "TIPO X"

                arquivo >> coefDifusao[i][0] >> coefDifusao[i][1];
                arquivo >> secaoChoqueAbsorcao[i][0] >> secaoChoqueAbsorcao[i][1];
                arquivo >> secaoChoqueEspalhamento[i][0] >> secaoChoqueEspalhamento[i][1];
                arquivo >> nuSecaoChoqueFissao[i][0] >> nuSecaoChoqueFissao[i][1];

                // Cálculo da seção de choque de remoção
                secaoChoqueRemocao[i][0] = secaoChoqueAbsorcao[i][0];
                secaoChoqueRemocao[i][1] = secaoChoqueAbsorcao[i][1];
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar dados nucleares: " << e.what() << std::endl;
    }
}

void DadosNucleares::atribuirDadosAMalha(int malha, int tipo) {
    // Ajustar índice para base-0
    tipo--;

    coefDifusaoMalha[malha][0] = coefDifusao[tipo][0];
    coefDifusaoMalha[malha][1] = coefDifusao[tipo][1];

    secaoChoqueRemocaoMalha[malha][0] = secaoChoqueRemocao[tipo][0];
    secaoChoqueRemocaoMalha[malha][1] = secaoChoqueRemocao[tipo][1];

    nuSecaoChoqueFissaoMalha[malha][0] = nuSecaoChoqueFissao[tipo][0];
    nuSecaoChoqueFissaoMalha[malha][1] = nuSecaoChoqueFissao[tipo][1];

    secaoChoqueEspalhamentoMalha[malha][0] = secaoChoqueEspalhamento[tipo][0];
    secaoChoqueEspalhamentoMalha[malha][1] = secaoChoqueEspalhamento[tipo][1];

    // Verifica se é região combustível
    if (nuSecaoChoqueFissao[tipo][1] > 0.0) {
        codigoEC[malha] = 'C';
    }
    else {
        codigoEC[malha] = 'R';
    }
}

void DadosNucleares::redimensionarMalhas(int numeroTotalMalhas) {
    coefDifusaoMalha.resize(numeroTotalMalhas);
    secaoChoqueRemocaoMalha.resize(numeroTotalMalhas);
    nuSecaoChoqueFissaoMalha.resize(numeroTotalMalhas);
    secaoChoqueEspalhamentoMalha.resize(numeroTotalMalhas);
    codigoEC.resize(numeroTotalMalhas);

    for (int i = 0; i < numeroTotalMalhas; i++) {
        coefDifusaoMalha[i].resize(2);
        secaoChoqueRemocaoMalha[i].resize(2);
        nuSecaoChoqueFissaoMalha[i].resize(2);
        secaoChoqueEspalhamentoMalha[i].resize(2);
    }
}

int DadosNucleares::getNumeroTipo() const {
    return numeroTipo;
}

char DadosNucleares::getCodigoEC(int malha) const {
    return codigoEC[malha];
}

double DadosNucleares::getCoefDifusaoMalha(int malha, int grupo) const {
    return coefDifusaoMalha[malha][grupo - 1];
}

double DadosNucleares::getSecaoChoqueRemocaoMalha(int malha, int grupo) const {
    return secaoChoqueRemocaoMalha[malha][grupo - 1];
}

double DadosNucleares::getNuSecaoChoqueFissaoMalha(int malha, int grupo) const {
    return nuSecaoChoqueFissaoMalha[malha][grupo - 1];
}

double DadosNucleares::getSecaoChoqueEspalhamentoMalha(int malha, int grupo) const {
    return secaoChoqueEspalhamentoMalha[malha][grupo - 1];
}

double DadosNucleares::getSecaoChoqueAbsorcaoMalha(int malha, int grupo) const {
    return secaoChoqueAbsorcao[malha][grupo - 1];
}
void DadosNucleares::setCodigoEC(int malha, char codigo) {
    codigoEC[malha] = codigo;
}