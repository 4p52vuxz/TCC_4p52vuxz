#include "Reator.h"
#include "GerenciadorIO.h"
#include "SolucionadorJacobi.h"
#include "SolucionadorGauss.h"
#include "SolucionadorAnaliticoFundamental.h"
#include <stdexcept>
#include <chrono>
#include <iostream>

Reator::Reator() :
    fluxo(nullptr),
    matriz(nullptr),
    fonte(nullptr),
    solucionador(nullptr) {
}

Reator::~Reator() {
    delete fluxo;
    delete matriz;
    delete fonte;
    delete solucionador;
}

void Reator::inicializarComponentes() {
    int numeroTotalMalhas = nucleo.getNumeroTotalMalhas();

    fluxo = new Fluxo(numeroTotalMalhas);
    matriz = new Matriz(numeroTotalMalhas);
    fonte = new Fonte(numeroTotalMalhas);

    dadosNucleares.redimensionarMalhas(numeroTotalMalhas);

    int malhaAtual = 0;
    for (int regiao = 1; regiao <= nucleo.getNumeroRegiao(); regiao++) {
        int tipo = nucleo.getTipoRegiao(regiao);
        for (int p = 0; p < nucleo.getNumeroParticao(regiao); p++) {
            dadosNucleares.atribuirDadosAMalha(malhaAtual, tipo);
            malhaAtual++;
        }
    }

    fluxo->inicializar();
    fatorMultiplicacao.inicializar();

    matriz->montarMatriz(dadosNucleares, nucleo);
}

void Reator::carregarDados(const std::string& arquivoCriterios, const std::string& arquivoDadosNucleares, const std::string& arquivoDadosNucleo) {
    GerenciadorIO::carregarCriterios(criterios, arquivoCriterios);
    GerenciadorIO::carregarDadosNucleares(dadosNucleares, arquivoDadosNucleares);
    GerenciadorIO::carregarDadosNucleo(nucleo, arquivoDadosNucleo);

    inicializarComponentes();
}

void Reator::prepararCalculo(int metodo) {
    fluxo->setMetodo(metodo);

    if (solucionador != nullptr) {
        delete solucionador;
    }

    if (metodo == 1) {
        solucionador = new SolucionadorJacobi();
    }
    else if (metodo == 2) {
        solucionador = new SolucionadorGauss();
    }
    else if (metodo == 3) {
        if (solucionador != nullptr) { 
            delete solucionador;
            solucionador = nullptr;
        }
        solucionador = new SolucionadorAnaliticoFundamental();
    }

    if (metodo == 1 || metodo == 2 || metodo == 3) { 
        GerenciadorIO::gravarKeffMetodo(fatorMultiplicacao, 0, metodo);
    }
    else {
        GerenciadorIO::gravarKeff(fatorMultiplicacao, 0, "Keff_Desconhecido.txt"); // Fallback
    }
    fonte->calcularFonteFissao(dadosNucleares, *fluxo, nucleo);
    double somaFonteFissao = fonte->somarTermoFonteFissao(dadosNucleares, nucleo);
    fatorMultiplicacao.setSomaFonteFissao(somaFonteFissao);
    fatorMultiplicacao.setSomaFonteFissaoAnterior(somaFonteFissao);

    GerenciadorIO::gravarKeff(fatorMultiplicacao, 0, "Keff.txt");
}

void Reator::executarCalculo() {
    // Loop de iterações externas
    int metodo = fluxo->getMetodo();
    fluxo->setNumeroIteracaoExterna(0);
    bool convergido = false;
    if (metodo == 1 || metodo == 2) { 
        do {
            // Incrementar contador de iterações
            fluxo->incrementarIteracaoExterna();

            // Salvar valores do passo anterior
            fatorMultiplicacao.setSomaFonteFissaoAnterior(fatorMultiplicacao.getSomaFonteFissao());
            fatorMultiplicacao.setKeffAnterior(fatorMultiplicacao.getKeff());
            fluxo->atualizarFluxoAnterior();

            // Calcular fonte para a iteração atual
            fonte->calcularFonte(*fluxo, fatorMultiplicacao);

            // Resolver sistema
            solucionador->resolver(*fluxo, *matriz, dadosNucleares, criterios);

            // Calcular nova fonte de fissão
            fonte->calcularFonteFissao(dadosNucleares, *fluxo, nucleo);
            fatorMultiplicacao.setSomaFonteFissao(fonte->somarTermoFonteFissao(dadosNucleares, nucleo));

            // Calcular Keff
            fatorMultiplicacao.calcularKeff();

            // Gravar Keff atual
            if (metodo == 1) {
                GerenciadorIO::gravarKeff(fatorMultiplicacao, fluxo->getNumeroIteracaoExterna(), "Jacobi_Keff.txt");
            }
            else {
                GerenciadorIO::gravarKeff(fatorMultiplicacao, fluxo->getNumeroIteracaoExterna(), "Gauss_Keff.txt");
            }
            

            // Verificar se excedeu o número máximo de iterações externas
            if (fluxo->getNumeroIteracaoExterna() > criterios.getNumeroMaximoExterna()) {
                throw std::runtime_error("Excedeu o numero maximo de iteracoes externas");
            }

            // Teste de convergência do Keff
            double desvioKeff = fatorMultiplicacao.getDesvioKeff();
            if (desvioKeff <= criterios.getToleranciaKeff()) {
                // Teste de convergência do fluxo
                double desvioFluxo = fluxo->calcularDesvioMaximo();
                if (desvioFluxo <= criterios.getToleranciaFluxoExterna()) {
                    convergido = true;
                }
            }

        } while (!convergido);
    }

    else if (metodo == 3) {
        fluxo->setNumeroIteracaoExterna(1); 

        SolucionadorAnaliticoFundamental* saf_solver = static_cast<SolucionadorAnaliticoFundamental*>(solucionador);
        saf_solver->setContextoNucleo(nucleo);

        saf_solver->resolver(*fluxo, *matriz, dadosNucleares, criterios);

        std::cout << "INFO: Calculo analitico fundamental concluido. Perfil de fluxo gerado." << std::endl;

    }
    else {
        throw std::runtime_error("Metodo desconhecido em Reator::executarCalculo.");
    }
    
}




void Reator::gravarResultados(const std::string& prefixo) {
    std::string nomeMapaNucleo = prefixo + "MapaNucleo.csv";
    std::string nomeFluxo = prefixo + "Fluxo_de_Neutrons.csv";

    // Gravar mapa do núcleo
    GerenciadorIO::gravarMapaNucleo(nucleo, nomeMapaNucleo);

    // Gravar fluxo final
    GerenciadorIO::gravarFluxo(*fluxo, fatorMultiplicacao, nucleo, nomeFluxo);

}

void Reator::gravarResultadosMetodo(int metodo) {
    // Gravar mapa do núcleo (apenas uma vez é suficiente, independente do método)
    if (metodo == 1) {
        GerenciadorIO::gravarMapaNucleo(nucleo, "MapaNucleo.csv");
    }

    // Gravar fluxo final com identificador do método
    GerenciadorIO::gravarFluxoMetodo(*fluxo, fatorMultiplicacao, nucleo, metodo);
}