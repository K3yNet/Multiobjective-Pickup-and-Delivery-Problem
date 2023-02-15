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
#include <stdlib.h>
#include <cstdlib>
#include "funcoes.h"

using namespace std;

int main(){

    Truck caminhao;

    string posicaoAtualArquivo = "poa-n100-6.txt";

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);


    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];

    leituraGrafo(posicaoAtualArquivo, instancia.tamanho_instancia, MA, totalPontos);
    caminhao.rota.push_back(0);
    caminhao.rota.push_back((instancia.tamanho_instancia - 1) / 2);
    caminhao.rota.push_back(instancia.tamanho_instancia - 1);
    vector<Truck> caminhoes;
    caminhoes.push_back(caminhao);
    
    Pontos *coletas = new Pontos[(instancia.tamanho_instancia -1 )/2];
    for(int i = 0; i < (instancia.tamanho_instancia -1 )/2; i++)
        coletas[i] = totalPontos[i];

    quickSort(coletas,0,(instancia.tamanho_instancia-1)/2);

    for(int i = 0; i < (instancia.tamanho_instancia -1 )/2;i++)
        cout << coletas[i].id << " ";

    
    mais_proximo(caminhoes[0].rota, MA, instancia, totalPontos, caminhoes);

    return 0;
}
