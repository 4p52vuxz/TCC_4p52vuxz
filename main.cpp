#include "GerenciadorIO.h"
#include "Reator.h"
#include <iostream>
#include <chrono>
#include <string>


int main() {
    try {
        std::string arquivoCriterios = "criterios.json";
        std::string arquivoDadosNucleares = "dados_nucleares.json";
        std::string arquivoDadosNucleo = "dados_nucleo.json";

        {
            std::cout << "------------------------------" << std::endl;
            std::cout << "INICIANDO COM METODO DE JACOBI" << std::endl;
            std::cout << "------------------------------" << std::endl;

            int metodo = 1;

            Reator reator;
            reator.carregarDados(arquivoCriterios, arquivoDadosNucleares, arquivoDadosNucleo);
            reator.prepararCalculo(metodo);

            std::cout << "Executando calculo com metodo de Jacobi..." << std::endl;
            auto inicioTempo = std::chrono::high_resolution_clock::now();

            reator.executarCalculo();

            auto fimTempo = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> tempoDecorrido = fimTempo - inicioTempo;

            reator.gravarResultadosMetodo(metodo);
            GerenciadorIO::gravarTempoMetodo(tempoDecorrido.count(), metodo);

            std::cout << "Simulacao com metodo de Jacobi concluida!" << std::endl;
            std::cout << "Tempo de execucao: " << tempoDecorrido.count() << " segundos" << std::endl;
            std::cout << "Resultados salvos em arquivos CSV com prefixo 'Jacobi_'" << std::endl;
            std::cout << std::endl;
        }

        {
            std::cout << "------------------------------------" << std::endl;
            std::cout << "INICIANDO COM METODO DE GAUSS-SEIDEL" << std::endl;
            std::cout << "------------------------------------" << std::endl;

            int metodo = 2;

            Reator reator;
            reator.carregarDados(arquivoCriterios, arquivoDadosNucleares, arquivoDadosNucleo);
            reator.prepararCalculo(metodo);

            std::cout << "Executando calculo com metodo de Gauss-Seidel..." << std::endl;
            auto inicioTempo = std::chrono::high_resolution_clock::now();

            reator.executarCalculo();

            auto fimTempo = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> tempoDecorrido = fimTempo - inicioTempo;

            reator.gravarResultadosMetodo(metodo);
            GerenciadorIO::gravarTempoMetodo(tempoDecorrido.count(), metodo);

            std::cout << "Simulação com metodo de Gauss-Seidel concluida!" << std::endl;
            std::cout << "Tempo de execucao: " << tempoDecorrido.count() << " segundos" << std::endl;
            std::cout << "Resultados salvos em arquivos CSV com prefixo 'Gauss_'" << std::endl;
            std::cout << std::endl;
        }

        {
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "INICIANDO COM METODO ANALITICO FUNDAMENTAL (FORMA SENOIDAL)" << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;

            int metodo = 3;

            Reator reator_af;


            reator_af.carregarDados(arquivoCriterios, arquivoDadosNucleares, arquivoDadosNucleo);
            reator_af.prepararCalculo(metodo);

            std::cout << "Executando calculo com metodo Analitico Fundamental..." << std::endl;
            auto inicioTempo = std::chrono::high_resolution_clock::now();

            reator_af.executarCalculo();

            auto fimTempo = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> tempoDecorrido = fimTempo - inicioTempo;


            reator_af.gravarResultadosMetodo(metodo);
            GerenciadorIO::gravarTempoMetodo(tempoDecorrido.count(), metodo);

            std::cout << "Simulacao com metodo Analitico Fundamental concluida!" << std::endl;
            std::cout << "Tempo de execucao: " << tempoDecorrido.count() << " segundos" << std::endl;
            std::cout << "Resultados salvos em arquivos CSV com prefixo 'AnaliticoFundamental_'" << std::endl;
            std::cout << std::endl;
        }
       
        std::cout << "===============================================" << std::endl;
        std::cout << "  TODAS AS SIMULACOES CONCLUIDAS COM SUCESSO   " << std::endl;
        std::cout << "===============================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Arquivos gerados:" << std::endl;
        std::cout << "- MapaNucleo.csv" << std::endl;
        std::cout << "- Jacobi_Fluxo_de_Neutrons.csv" << std::endl;
        std::cout << "- Jacobi_Keff.csv" << std::endl;
        std::cout << "- Jacobi_Tempo.csv" << std::endl;
        std::cout << "- Gauss_Fluxo_de_Neutrons.csv" << std::endl;
        std::cout << "- Gauss_Keff.csv" << std::endl;
        std::cout << "- Gauss_Tempo.csv" << std::endl;
        std::cout << "- AnaliticoFundamental_Fluxo_de_Neutrons.csv" << std::endl;
        std::cout << "- AnaliticoFundamental_Tempo.csv" << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERRO: " << e.what() << std::endl;
        std::cerr << "A simulacao foi interrompida devido a um erro." << std::endl;
        return 1;
    }
}