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

int ref = 1;

vector<int> geraRotas(Instancia inst, Pontos *totalPontos, int index, Truck caminhao){
    vector<int> rota;
    rota.push_back(0);
    rota.push_back(index);
    rota.push_back(totalPontos[index].pEntrega);
    rota.push_back(0);
    ref++;
    for (int i = 0; i < rota.size(); i++){
        caminhao.tempoGasto += totalPontos[rota[i]].duracaoServico;
        caminhao.ocupacao += totalPontos[rota[i]].demanda;
    }
    return rota;
}

int main(){

    // nyc-n100-4.txt
    // poa-n100-6.txt
    // bar-n100-1.txt
    // poa-n5000-3.txt
    string posicaoAtualArquivo = "poa-n100-6.txt ";

    Instancia instancia;

    LerInstancia(posicaoAtualArquivo,instancia);

    vector<Truck> caminhoes;

    Pontos *totalPontos = new Pontos[instancia.tamanho_instancia];
    int **MA = new int*[instancia.tamanho_instancia];

    leituraGrafo(posicaoAtualArquivo, instancia.tamanho_instancia, MA, totalPontos);

    Pontos *coletas = new Pontos[(instancia.tamanho_instancia - 1 ) / 2];
    for(int i = 0; i < (instancia.tamanho_instancia - 1 ) / 2; i++){
        coletas[i] = totalPontos[i];
        cout << coletas[i].id << " ";
    }
    cout << endl;

    selectionSort(coletas, instancia);

    for(int i = 0; i < (instancia.tamanho_instancia - 1 ) / 2; i++)
        cout << coletas[i].id << " ";
    cout << endl;

    vector<int> rota;
    int finish = 0;
    
    while (finish != 1){
        Truck caminhao;
        int index = coletas[0 + ref].id;

        label1:
        if (!coletas[0 + ref].visitado)
            rota = geraRotas(instancia, totalPontos, index, caminhao);
        else{
            ref++;
            goto label1;
        }
        caminhao.rota = mais_proximo(caminhao, rota, MA, instancia, totalPontos, finish);
        // caminhao.tempoGasto += custo_total(caminhao.rota, MA);
        cout << "Melhor rota: " << endl;
        for (int i = 0; i < caminhao.rota.size(); i++){
            cout << caminhao.rota[i] << " ";
        }
        cout << endl;
        caminhoes.push_back(caminhao);
    }

    int custo = INF;

    for (int i = 0; i < caminhoes.size(); i++){
        if (verificaRestricao(totalPontos, instancia, caminhoes[i].rota.insert(caminhoes[i].rota.begin + 1, caminhoes[i + 1].rota))){
            caminhoes[i].rota.insert(caminhoes[i].rota.begin + 1, caminhoes[i + 1].rota);
            caminhoes[i + 1]
        }
        
    }
    

    cout << "Total de caminhoes: " << caminhoes.size() << endl;
    int custo = 0;
    for (int i = 0; i < caminhoes.size(); i++){
        custo += custo_total(caminhoes[i].rota, MA);
    }
    cout << "Custo total: " << custo << endl;

    return 0;
}
