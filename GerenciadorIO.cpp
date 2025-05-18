#include "GerenciadorIO.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void GerenciadorIO::carregarCriterios(Criterios& criterios, const std::string& nomeArquivo) {
    try {
        criterios.carregarDeArquivo(nomeArquivo);
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar criterios: " << e.what() << std::endl;
    }
}

void GerenciadorIO::carregarDadosNucleares(DadosNucleares& dadosNucleares, const std::string& nomeArquivo) {
    try {
        dadosNucleares.carregarDeArquivo(nomeArquivo);
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar dados nucleares: " << e.what() << std::endl;
    }
}

void GerenciadorIO::carregarDadosNucleo(Nucleo& nucleo, const std::string& nomeArquivo) {
    try {
        nucleo.carregarDeArquivo(nomeArquivo);
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar dados do nucleo: " << e.what() << std::endl;
    }
}

void GerenciadorIO::gravarMapaNucleo(const Nucleo& nucleo, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo " << nomeArquivo << " para escrita." << std::endl;
        return;
    }

    // Cabeçalho do CSV
    arquivo << "Malha,DeltaX,Posicao,CodigoEC" << std::endl;

    // Dados para cada malha
    for (int m = 0; m < nucleo.getNumeroTotalMalhas(); m++) {
        double posicao = (m + 1) * nucleo.getDeltaX(m);
        arquivo << m + 1 << ","
            << std::fixed << std::setprecision(4) << nucleo.getDeltaX(m) << ","
            << posicao << ","
            << "C" << std::endl;
    }

    arquivo.close();
}

void GerenciadorIO::gravarFluxo(const Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao,
    const Nucleo& nucleo, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo " << nomeArquivo << " para escrita." << std::endl;
        return;
    }

    // Cabeçalho do CSV
    arquivo << "NumeroIteracoes," << fluxo.getNumeroIteracaoExterna() << std::endl;
    arquivo << "Keff," << std::setprecision(12) << std::fixed << fatorMultiplicacao.getKeff() << std::endl;
    arquivo << std::endl; // Linha em branco para separar cabeçalho dos dados

    // Cabeçalho dos dados do fluxo
    arquivo << "Malha,Posicao,Fluxo_Grupo1,Fluxo_Grupo2" << std::endl;

    // Dados do fluxo para cada malha
    for (int m = 0; m < nucleo.getNumeroTotalMalhas(); m++) {
        double posicao = (m + 1) * nucleo.getDeltaX(m);
        arquivo << m + 1 << ","
            << std::fixed << std::setprecision(4) << posicao << ","
            << std::fixed << std::setprecision(9) << fluxo.getFluxo(m, 1) << ","
            << fluxo.getFluxo(m, 2) << std::endl;
    }

    arquivo.close();
}

void GerenciadorIO::gravarKeff(const FatorMultiplicacao& fatorMultiplicacao, int iteracao,
    const std::string& nomeArquivo) {
    std::ofstream arquivo;

    if (iteracao == 0) {
        arquivo.open(nomeArquivo); // Cria arquivo novo
        // Adicionar cabeçalho se for novo arquivo
        if (arquivo.is_open()) {
            arquivo << "Iteracao,Keff" << std::endl;
        }
    }
    else {
        arquivo.open(nomeArquivo, std::ios::app); // Append no arquivo existente
    }

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo " << nomeArquivo << " para escrita." << std::endl;
        return;
    }

    arquivo << iteracao << ","
        << std::fixed << std::setprecision(12) << fatorMultiplicacao.getKeff() << std::endl;

    arquivo.close();
}

void GerenciadorIO::gravarTempo(double tempo, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo " << nomeArquivo << " para escrita." << std::endl;
        return;
    }

    arquivo << "Metrica,Valor" << std::endl;
    arquivo << "Tempo_Simulacao_Segundos," << std::fixed << std::setprecision(6) << tempo << std::endl;

    arquivo.close();
}

std::string GerenciadorIO::obterNomeArquivoMetodo(const std::string& baseNome, int metodo) {
    //std::string prefixo = (metodo == 1) ? "Jacobi_" : "Gauss_";
    std::string prefixo;
    if (metodo == 1) {
        prefixo = "Jacobi_";
    }
    else if (metodo == 2) {
        prefixo = "Gauss_";
    }
    else if (metodo == 3) { 
        prefixo = "AnaliticoDireto_"; 
    }
    else {
        std::cerr << "AVISO GerenciadorIO::obterNomeArquivoMetodo: Metodo desconhecido (" << metodo << "), usando prefixo 'Desconhecido_'" << std::endl;
        prefixo = "Desconhecido_";
    }
    // Garantir que o arquivo tenha extensão .csv
    std::string nomeBase = baseNome;
    size_t pos = nomeBase.find_last_of('.');
    if (pos != std::string::npos) {
        nomeBase = nomeBase.substr(0, pos) + ".csv";
    }
    else {
        nomeBase += ".csv";
    }

    return prefixo + nomeBase;
}

void GerenciadorIO::gravarFluxoMetodo(const Fluxo& fluxo, const FatorMultiplicacao& fatorMultiplicacao,
    const Nucleo& nucleo, int metodo) {
    std::string nomeArquivo = obterNomeArquivoMetodo("Fluxo_de_Neutrons", metodo);
    gravarFluxo(fluxo, fatorMultiplicacao, nucleo, nomeArquivo);
}

void GerenciadorIO::gravarKeffMetodo(const FatorMultiplicacao& fatorMultiplicacao, int iteracao, int metodo) {
    std::string nomeArquivo = obterNomeArquivoMetodo("Keff", metodo);
    gravarKeff(fatorMultiplicacao, iteracao, nomeArquivo);
}

void GerenciadorIO::gravarTempoMetodo(double tempo, int metodo) {
    std::string nomeArquivo = obterNomeArquivoMetodo("Tempo", metodo);
    gravarTempo(tempo, nomeArquivo);
}

int GerenciadorIO::lerMetodo() {
    return 0;
}