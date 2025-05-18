#include "Criterios.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

Criterios::Criterios() :
    numeroMaximoExterna(0),
    numeroMaximoInterna(0),
    toleranciaKeff(0.0),
    toleranciaFluxoExterna(0.0),
    toleranciaFluxoInterna(0.0) {
}

void Criterios::setNumeroMaximoExterna(int valor) {
    numeroMaximoExterna = valor;
}

void Criterios::setNumeroMaximoInterna(int valor) {
    numeroMaximoInterna = valor;
}

void Criterios::setToleranciaKeff(double valor) {
    toleranciaKeff = valor;
}

void Criterios::setToleranciaFluxoExterna(double valor) {
    toleranciaFluxoExterna = valor;
}

void Criterios::setToleranciaFluxoInterna(double valor) {
    toleranciaFluxoInterna = valor;
}

int Criterios::getNumeroMaximoExterna() const {
    return numeroMaximoExterna;
}

int Criterios::getNumeroMaximoInterna() const {
    return numeroMaximoInterna;
}

double Criterios::getToleranciaKeff() const {
    return toleranciaKeff;
}

double Criterios::getToleranciaFluxoExterna() const {
    return toleranciaFluxoExterna;
}

double Criterios::getToleranciaFluxoInterna() const {
    return toleranciaFluxoInterna;
}

double Criterios::getkEffReferenciaAnalitico() const {
    return kEffReferenciaAnalitico;
}

void Criterios::carregarDeArquivo(const std::string& nomeArquivo) {
    try {
        // Verifica se o arquivo é JSON
        if (nomeArquivo.substr(nomeArquivo.size() - 5) == ".json") {
            std::ifstream arquivo(nomeArquivo);
            nlohmann::json j;
            arquivo >> j;

            numeroMaximoExterna = j["numeroMaximoExterna"];
            numeroMaximoInterna = j["numeroMaximoInterna"];
            toleranciaKeff = j["toleranciaKeff"];
            toleranciaFluxoExterna = j["toleranciaFluxoExterna"];
            toleranciaFluxoInterna = j["toleranciaFluxoInterna"];
            kEffReferenciaAnalitico = j["kEffReferenciaAnalitico"];
        }
        else {
            // Formato antigo de arquivo .txt
            std::ifstream arquivo(nomeArquivo);
            arquivo >> numeroMaximoExterna;
            arquivo >> numeroMaximoInterna;
            arquivo >> toleranciaKeff;
            arquivo >> toleranciaFluxoExterna;
            arquivo >> toleranciaFluxoInterna;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao carregar criterios: " << e.what() << std::endl;
    }
}