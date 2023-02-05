/*
MacroEntrega 1: Leitura de Dados, Estruturas e verificação de restrição
A solução aleatória foi gerada a partir da instancia poa-n100-6
Enzo Velo : 202120910
Lucas Gomes Colombo : 202120795
Rafael Brunini : 202120488
*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include "funcoes.h"

using namespace std;


int main(){

    Truck caminhao;

    string posicaoAtualArquivo = "nyc-n200-3.txt";

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];

    leituraGrafo(posicaoAtualArquivo, instancia.tamanho_instancia, MA, totalPontos);
    
    cout << quantidadeTrucks(totalPontos, MA, instancia, caminhao);

    return 0;
}