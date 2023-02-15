#ifndef funcoes
#define funcoes

#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

const int INF = 1000000;

struct Instancia{
    int tamanho_instancia;
    int capacidade_veiculo;
    int roterizacao;
    int janela_de_tempo;
};

struct Pontos{
    unsigned int id;
    float latitude;
    float longitude;
    int demanda;
    int tempoAbertura;
    int tempoLimite;
    int duracaoServico;
    int pColeta;
    int pEntrega;
    int visitado = 0;
};

struct Truck{
    unsigned int id;
    int tempoGasto = 0;
    int ocupacao = 0;
     vector <int> rota;
};

void LerInstancia(string namefile, Instancia &instancia){
    char delimitador = ' ';
    ifstream entrada(namefile);
    string campoAtual;
    for(int i = 1; i <= 10; i++){
        getline(entrada,campoAtual,delimitador);
        getline(entrada,campoAtual);
        switch (i)
        {
        case 5:
            instancia.tamanho_instancia = stoi(campoAtual);
            break;

        case 8:
            instancia.roterizacao = stoi(campoAtual);
            break;
        case 9:
            instancia.janela_de_tempo = stoi(campoAtual);
            break;
        case 10:
            instancia.capacidade_veiculo = stoi(campoAtual);
            break;
        default:
            break;
        }
    }
    entrada.close();
}

void leituraGrafo(string nameFile,int tamanhoGrafo,int **MA,Pontos *totalPontos){
    char delimitador = ' ';
    ifstream entrada(nameFile);
    string posicaoAtual;

    for(int i = 0; i < tamanhoGrafo; i++){
        MA[i] = new int[tamanhoGrafo];
    }

    for(int i = 0; i < 10;i++) // Descarta as 10 primeiras linhas que ja foram lidas pois caracterizam a instancia
        getline(entrada,posicaoAtual);

    getline(entrada,posicaoAtual);
    for(int i = 0; i < tamanhoGrafo;i++){
        for(int j = 1; j <= 9; j++ ){
            getline(entrada,posicaoAtual,delimitador);
            switch (j){
                case 1:
                    totalPontos[i].id = stoi(posicaoAtual);
                    break;
                
                case 2:
                    totalPontos[i].latitude = stof(posicaoAtual);
                    break;
                case 3:
                    totalPontos[i].longitude = stof(posicaoAtual);
                    break;

                case 4:
                    totalPontos[i].demanda = stoi(posicaoAtual);
                    break;
                
                case 5:
                    totalPontos[i].tempoAbertura = stoi(posicaoAtual);
                    break;
                
                case 6:
                    totalPontos[i].tempoLimite =  stoi(posicaoAtual);
                    break;
                
                case 7:
                    totalPontos[i].duracaoServico =  stoi(posicaoAtual);
                    break;
                
                case 8:
                    totalPontos[i].pColeta = stoi(posicaoAtual);
                    delimitador = '\n';
                    break;
                
                case 9:
                    totalPontos[i].pEntrega = stoi(posicaoAtual);
                    delimitador = ' ';
                    break;
                
                default:
                    break;
            }
        }
    }

    getline(entrada,posicaoAtual);
    for (int i = 0; i < tamanhoGrafo; i++){
        delimitador = ' ';
        for(int j = 0; j < tamanhoGrafo;j++){
            getline(entrada,posicaoAtual,delimitador);
            MA[i][j] = stoi(posicaoAtual);
            if(j == tamanhoGrafo-2)
                delimitador = '\n';
        }

    }
}

bool verificaRestricao(Pontos *totalPontos, Instancia inst, Truck caminhao){
    
    int visitaEntrega, visitaColeta;
    Pontos coleta;
    Pontos entrega;
    Pontos atual;
    bool restrito = false;

    //precedencia de coleta
    for(int i = 0; i < int(caminhao.rota.size()); i++){
        restrito = false;
        if(totalPontos[caminhao.rota[i]].pColeta > 0){
            for(int j = 0; j < i ;j++){
                if(totalPontos[caminhao.rota[j]].id == totalPontos[caminhao.rota[i]].pColeta)
                    restrito = true;
            }
            if(restrito == false)
                return false;
            
        }
    }
    cout << "Prescedencia de coleta" << endl;

    //Janela de Tempo
    for(int i = 1; i < int(caminhao.rota.size()); i++){
        atual = totalPontos[caminhao.rota[i]];
        if(caminhao.tempoGasto < atual.tempoAbertura){
            caminhao.tempoGasto += atual.tempoAbertura - caminhao.tempoGasto;
            cout << "Tempo gasto: " << caminhao.tempoGasto << endl;
        }
        else if(caminhao.tempoGasto > atual.tempoLimite)
            return false;
    }
    cout << "Janela de tempo" << endl;

    // Obrigatoriedade e Exclusividade de visita
    for(int i = 0; i < int(caminhao.rota.size()); i++){
        visitaEntrega = 0;
        visitaColeta = 0;
        if(totalPontos[caminhao.rota[i]].pColeta == 0 and totalPontos[caminhao.rota[i]].id != 0){
            coleta = totalPontos[caminhao.rota[i]];
            entrega = totalPontos[totalPontos[caminhao.rota[i]].pEntrega-1];
        }
        else if(totalPontos[caminhao.rota[i]].pEntrega == 0 and totalPontos[caminhao.rota[i]].id != 0){
            entrega = totalPontos[caminhao.rota[i]];
            coleta = totalPontos[totalPontos[caminhao.rota[i]].pColeta-1];
        }

        for(int j = 0; j < caminhao.rota.size();j++){
            if(totalPontos[caminhao.rota[j]].id == coleta.id)
                visitaColeta++;

            if(totalPontos[caminhao.rota[j]].id == entrega.id)
                visitaEntrega++;
        }
        if(visitaColeta > 1 or visitaEntrega > 1)
            return false;
    }
    cout << "Exclusividade de visita" << endl;

    // Verifica inicio e fim e se não estoura o tempo de roterização
    if(caminhao.rota[0] != 0 or caminhao.rota[caminhao.rota.size()-1] != 0 or caminhao.tempoGasto > inst.roterizacao)
        return false;
    cout << "Inicio e fim" << endl;
        
    //Capacidade do veiculo
    if(caminhao.ocupacao > inst.capacidade_veiculo)
        return false;
    cout << "Capacidade" << endl;

    return true; // caso passe por todas as restrições retorna verdadeiro
}

vector<pair<int, int>> pega_pares(vector<int> lista){
    vector<pair<int, int>> pares;
    for (int i = 0; i < lista.size() - 1; i++) {
        pares.push_back(make_pair(lista[i], lista[i + 1]));
    }
    pares.push_back(make_pair(lista[lista.size() - 1], lista[0]));
    
    return pares;
}

int custo_total(const vector<int> &ciclo, int** MA) {
    int custo = 0;
    vector<pair<int, int>>pares = pega_pares(ciclo);
    for (int i = 0; i < pares.size() - 1; i++)
    {
        int a = pares[i].first;
        int b = pares[i].second;
        custo = MA[a][b] + custo;
        cout << "custo = +" << MA[a][b] << " - " << custo << endl;
    }
    cout << endl;

    return custo;
}

vector<int> mais_proximo(vector<int> ciclo_inicial, int** MA, Instancia inst, Pontos *totalPontos, vector<Truck> caminhoes) {
    // inst.tamanho_instancia
    bool chave = true;
    while (ciclo_inicial.size() < inst.tamanho_instancia) {
        int distancia_vertice = INF;
        int custo = INF;
        int vertice = -1;

        for (int linha = 0; linha < (inst.tamanho_instancia - 1) / 2; linha++) {
            if (find(ciclo_inicial.begin(), ciclo_inicial.end(), linha) == ciclo_inicial.end()) {
                for (int coluna = 0; coluna < inst.tamanho_instancia; coluna++) {
                    if (find(ciclo_inicial.begin(), ciclo_inicial.end(), coluna) != ciclo_inicial.end()) {
                        if (MA[linha][coluna] < distancia_vertice) {
                            distancia_vertice = MA[linha][coluna];
                            vertice = linha;
                        }
                    }
                }
            }
        }

        cout << "Vertice mais distante: " << vertice << endl;

        vector<pair<int, int>> pares = pega_pares(ciclo_inicial);

        for (pair<int, int> aresta : pares) {
            int novo_custo = MA[vertice][aresta.first] + MA[vertice][aresta.second] - MA[aresta.first][aresta.second];
            if (novo_custo < custo) {
                caminhoes[0].rota = ciclo_inicial;
                if (chave){
                    caminhoes[0].rota.push_back(0);
                    chave = false;
                }
                
                custo = novo_custo;
                
                auto it = find(caminhoes[0].rota.begin(), caminhoes[0].rota.end(), aresta.first);
                caminhoes[0].rota.insert((caminhoes[0].rota.begin() + int(it - caminhoes[0].rota.begin() + 1)), vertice);
                caminhoes[0].ocupacao += totalPontos[vertice].demanda;
                caminhoes[0].tempoGasto += totalPontos[vertice].duracaoServico;
                if (vertice < (inst.tamanho_instancia - 1) / 2){
                    cout << "Verice de coleta" << endl;
                    it = find(caminhoes[0].rota.begin(), caminhoes[0].rota.end(), vertice);
                    caminhoes[0].rota.insert((caminhoes[0].rota.begin() + int(it - caminhoes[0].rota.begin() + 1)), totalPontos[vertice].pEntrega);
                    caminhoes[0].ocupacao += totalPontos[totalPontos[vertice].pEntrega].demanda;
                    caminhoes[0].tempoGasto += totalPontos[totalPontos[vertice].pEntrega].duracaoServico;
                    for (int i = 0; i < caminhoes[0].rota.size(); i++)
                        cout << caminhoes[0].rota[i] << " ";
                    cout << endl;
                }
                else{
                    cout << "Verice de entrega" << endl;
                    it = find(caminhoes[0].rota.begin(), caminhoes[0].rota.end(), vertice);
                    caminhoes[0].rota.insert((caminhoes[0].rota.begin() + int(it - caminhoes[0].rota.begin())), totalPontos[vertice].pColeta);
                    caminhoes[0].ocupacao += totalPontos[totalPontos[vertice].pColeta].demanda;
                    caminhoes[0].tempoGasto += totalPontos[totalPontos[vertice].pColeta].duracaoServico;
                    for (int i = 0; i < caminhoes[0].rota.size(); i++)
                        cout << caminhoes[0].rota[i] << " ";
                    cout << endl;
                }   
            }
        }
        
        ciclo_inicial = vector<int>(caminhoes[0].rota);
        caminhoes[0].tempoGasto += custo_total(ciclo_inicial, MA);
        cout << caminhoes[0].tempoGasto << endl;
        if(verificaRestricao(totalPontos, inst, caminhoes[0]))
            continue;
        else{
            cout << "Travou" << endl;
            return ciclo_inicial;
        }
    }

    for (int i = 0; i < ciclo_inicial.size(); i++)
    {
        cout << ciclo_inicial[i] << " ";
    }
    

    return ciclo_inicial;

}

void sequentialInsertion(Pontos *totalPontos, Instancia inst, vector <Truck> &caminhoes, int** MA){
    
    Truck new_truck;
    caminhoes.push_back(new_truck);
    int nCaminhoes = 0;
    caminhoes[0].rota.push_back(0);
    caminhoes[0].rota.push_back((inst.tamanho_instancia - 1) / 2);
    caminhoes[0].rota.push_back(inst.tamanho_instancia - 1);

    // while(true){
    //     if (verificaRestricao(totalPontos, inst, caminhoes[nCaminhoes])){
    //         continue;
    //     }
    //     else{
    //         nCaminhoes++;
    //         caminhoes.push_back(new_truck);
    //     }
    // }
}

#endif
