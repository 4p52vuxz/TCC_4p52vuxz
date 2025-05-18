#pragma once
#include <vector>
#include <string>


class DadosNucleares {
private:
    int numeroTipo;
    std::vector<std::vector<double>> coefDifusao;
    std::vector<std::vector<double>> secaoChoqueAbsorcao;
    std::vector<std::vector<double>> secaoChoqueRemocao;
    std::vector<std::vector<double>> nuSecaoChoqueFissao;
    std::vector<std::vector<double>> secaoChoqueEspalhamento;

    
    std::vector<std::vector<double>> coefDifusaoMalha;
    std::vector<std::vector<double>> secaoChoqueRemocaoMalha;
    std::vector<std::vector<double>> nuSecaoChoqueFissaoMalha;
    std::vector<std::vector<double>> secaoChoqueEspalhamentoMalha;
    std::vector<char> codigoEC;

public:
    DadosNucleares();

    void carregarDeArquivo(const std::string& nomeArquivo);
    void atribuirDadosAMalha(int malha, int tipo);
    void redimensionarMalhas(int numeroTotalMalhas);

    int getNumeroTipo() const;
    char getCodigoEC(int malha) const;
    double getCoefDifusaoMalha(int malha, int grupo) const;
    double getSecaoChoqueRemocaoMalha(int malha, int grupo) const;
    double getNuSecaoChoqueFissaoMalha(int malha, int grupo) const;
    double getSecaoChoqueEspalhamentoMalha(int malha, int grupo) const;
    double getSecaoChoqueAbsorcaoMalha(int malha, int grupo) const;

    void setCodigoEC(int malha, char codigo);
};