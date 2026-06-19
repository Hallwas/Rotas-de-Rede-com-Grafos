#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "grafo.cpp"
#include "graphviz.cpp"

using namespace std;

void exibirMenu() {
    cout << "\n--------------------------------------" << endl;
    cout << "1. Opção"    << endl;
    cout << "2. Opção"    << endl;
    cout << "3. Opção"    << endl;
    cout << "4. Opção"    << endl;
    cout << "0. Sair"     << endl;
    cout << "--------------------------------------" << endl;
    cout << "Escolha uma opcao: ";
}

int main(int argc, char* argv[]) {

    // Verifica se o usuario passou o nome do arquivo como argumento
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << "<arquivo.log>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];

    // Chama o builder do grafo.cpp que le o arquivo, monta o grafo e retorna [qtd_vertices, qtd_arestas]
    vector<int> resultado = builder(nomeArquivo);

    // Se o arquivo nao abriu, o builder ja exibe o erro e chama exit(1)
    cout << "Grafo de roteamento inicializado!" << endl;
    cout << "Vertices unicos (IPs): " << resultado[0] << " | Arestas: " << resultado[1] << endl;

    // Loop principal do menu
    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "[opcao 1]" << endl;
                break;
            case 2:
                cout << "[opcao 2]" << endl;
                break;
            case 3:
                cout << "[opcao 3]" << endl;
                break;
            case 4:
                cout << "[opcao 4]" << endl;
                break;
            case 0:
                cout << "Encerrando o programa." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
    }

    return 0;
}