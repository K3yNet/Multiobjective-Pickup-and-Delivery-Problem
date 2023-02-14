#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <utility>
#include <sstream>
using namespace std;
const int INF = 1000000;
const int MAX_N = 101;

vector<int> ciclo_inicial = {0, 50, 100};

std::vector<std::vector<int>> matriz_arquivo() {
    std::ifstream arquivo("entrada.txt");
    std::vector<std::vector<int>> matriz;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        std::istringstream linha_stream(linha);
        int num;
        std::vector<int> linha_matriz;

        while (linha_stream >> num) {
            linha_matriz.push_back(num);
        }

        matriz.push_back(linha_matriz);
    }

    return matriz;
}

std::vector<std::vector<int>> matriz = matriz_arquivo();

int custo_total(const vector<int> &ciclo) {
    int custo = 0;
    int vertice_atual, proximo_vertice;
    for (int i = 0; i < ciclo.size() - 1; i++) {
        vertice_atual = ciclo[i];
        proximo_vertice = ciclo[i + 1];
        custo += matriz[vertice_atual][proximo_vertice];
    }
    return custo;
}

vector<pair<int, int>> pega_pares(vector<int> lista){
    vector<pair<int, int>> pares;
    for (int i = 0; i < lista.size() - 1; i++) {
        pares.push_back(make_pair(ciclo_inicial[i], ciclo_inicial[i + 1]));
    }
    pares.push_back(make_pair(ciclo_inicial[lista.size() - 1], ciclo_inicial[0]));
    
    return pares;

}

vector<int> mais_proximo(vector<int> ciclo_inicial) {

    while (ciclo_inicial.size() < MAX_N) {
        int distancia_vertice = INF;
        int custo = INF;
        int vertice = -1;

        for (int linha = 0; linha < MAX_N; linha++) {
            if (find(ciclo_inicial.begin(), ciclo_inicial.end(), linha) == ciclo_inicial.end()) {
                for (int coluna = 0; coluna < MAX_N; coluna++) {
                    if (find(ciclo_inicial.begin(), ciclo_inicial.end(), coluna) != ciclo_inicial.end()) {
                        if (matriz[linha][coluna] < distancia_vertice) {
                            distancia_vertice = matriz[linha][coluna];
                            vertice = linha;
                        }
                    }
                }
            }
        }

        cout << "Vertice mais distante: " << vertice << endl;

        vector<pair<int, int>> pares = pega_pares(ciclo_inicial);
        vector<int> novo_ciclo;

        for (pair<int, int> aresta : pares) {
            int novo_custo = matriz[vertice][aresta.first] + matriz[vertice][aresta.second] - matriz[aresta.first][aresta.second];
            cout << "Novo custo = " << novo_custo << endl;
            cout << "Matriz 1 = " << matriz[vertice][aresta.first] << endl;
            cout << "Matriz 2 = " << matriz[vertice][aresta.second] << endl;
            cout << "Matriz 3 = " << matriz[aresta.first][aresta.second] << endl;
            if (novo_custo < custo) {
                novo_ciclo = ciclo_inicial;
                custo = novo_custo;
                cout << "Custo = " << custo << endl;
                novo_ciclo.insert(novo_ciclo.begin() + (aresta.first + 1), vertice);
            }
            // for (int i = 0; i < novo_ciclo.size(); i++)
            // {
            //     cout << novo_ciclo[i] << " ";
            // }
            // cout << endl;
        }
        
        ciclo_inicial = vector<int>(novo_ciclo);

        for (int i = 0; i < ciclo_inicial.size(); i++){
            cout << ciclo_inicial[i] << " ";
        }
        cout << endl;

    }

    ciclo_inicial.push_back(0);

    return ciclo_inicial;

}

int main() {

    // for (int i = 0; i < MAX_N; i++)
    // {
    //     for (int j = 0; j < MAX_N; j++)
    //     {
    //         cout << matriz[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    
    ciclo_inicial = mais_proximo(ciclo_inicial);

    cout << endl;

    cout << custo_total(ciclo_inicial) << endl;
    
    return 0;
}
