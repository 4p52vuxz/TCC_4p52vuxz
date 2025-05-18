#include "SolucionadorAnaliticoFundamental.h"
#include <cmath>        
#include <stdexcept>   
#include <vector>
#include <iostream>  
#include <iomanip> 

SolucionadorAnaliticoFundamental::SolucionadorAnaliticoFundamental() : pNucleoConfigurado(nullptr) {}

void SolucionadorAnaliticoFundamental::setContextoNucleo(const Nucleo& nucleo_obj) {
    pNucleoConfigurado = &nucleo_obj;
}

void SolucionadorAnaliticoFundamental::resolver(Fluxo& fluxo,
    const Matriz& matriz_ignorada,
    const DadosNucleares& dadosNucleares,
    const Criterios& criterios_ignorados) {
    if (!pNucleoConfigurado) {
        throw std::runtime_error("Contexto (nucleo) nao foi configurado para SolucionadorAnaliticoFundamental. Chame setContextoNucleo primeiro.");
    }
    if (pNucleoConfigurado->getNumeroRegiao() != 1) {
        throw std::runtime_error("SolucionadorAnaliticoFundamental implementado apenas para 1 regiao homogenea.");
    }


    double L_slab = pNucleoConfigurado->getTamanhoRegiao(1); 
    if (L_slab <= 0) {
        throw std::runtime_error("Largura do slab (L) deve ser positiva para SolucionadorAnaliticoFundamental.");
    }


    double D1 = dadosNucleares.getCoefDifusaoMalha(0, 1);
    double Sigma_a1 = dadosNucleares.getSecaoChoqueAbsorcaoMalha(0, 1);
    double Sigma_s12 = dadosNucleares.getSecaoChoqueEspalhamentoMalha(0, 2);

    double D2 = dadosNucleares.getCoefDifusaoMalha(0, 2);      
    double Sigma_a2 = dadosNucleares.getSecaoChoqueAbsorcaoMalha(0, 2);

    double B_g_sq = (M_PI / L_slab) * (M_PI / L_slab);

    double denominador_R_amp = D2 * B_g_sq + Sigma_a2;
    double R_amplitude_ratio;

    if (std::abs(denominador_R_amp) < 1e-12) { 
        std::cerr << "AVISO (SolucionadorAnaliticoFundamental): Denominador para a razao de amplitude R_amp eh proximo de zero ("
            << denominador_R_amp << "). Isso pode indicar ressonância ou um caso especial." << std::endl;
        std::cerr << "   Detalhes: D2=" << D2 << ", B_g_sq=" << B_g_sq << ", Sigma_a2=" << Sigma_a2 << ", Sigma_s12=" << Sigma_s12 << std::endl;

        if (std::abs(Sigma_s12) < 1e-12) {
            R_amplitude_ratio = 0.0;
            std::cerr << "   Sigma_s12 tambem proximo de zero. Setando R_amplitude_ratio = 0." << std::endl;
        }
        else {
           
            R_amplitude_ratio = 1e12; 
            std::cerr << "   Numerador Sigma_s12 nao eh zero. Setando R_amplitude_ratio para valor grande." << std::endl;
        }
    }
    else {
        R_amplitude_ratio = Sigma_s12 / denominador_R_amp;
    }


    int num_malhas = 500;
    if (num_malhas == 0) {
        throw std::runtime_error("Objeto Fluxo (SolucionadorAnaliticoFundamental) nao tem malhas definidas (getNumeroDeMalhas() == 0).");
    }
    double dx_malha = L_slab / static_cast<double>(num_malhas);


    for (int m = 0; m < num_malhas; ++m) {

        double x_centro_malha = (static_cast<double>(m) + 0.5) * dx_malha;


        double phi1_norm_x = std::sin(M_PI * x_centro_malha / L_slab);


        double phi2_norm_x = R_amplitude_ratio * phi1_norm_x;

        fluxo.setFluxo(m, 1, phi1_norm_x);
        fluxo.setFluxo(m, 2, phi2_norm_x);
    }

}