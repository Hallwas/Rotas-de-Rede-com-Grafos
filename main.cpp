#include <iostream>
#include <fstream>
#include <string>

#include "grafo.cpp"
#include "graphviz.cpp"

using namespace std;

void exibirMenu() {
    cout << "\n--------" << endl;
    cout << "1. Exibir Grafo Completo"           << endl;
    cout << "2. Encontrar Menor Caminho"         << endl;
    cout << "3. Calcular o Diametro do Grafo"    << endl;
    cout << "4. Identificar Roteadores Criticos" << endl;
    cout << "0. Sair"                            << endl;
    cout << "--------"                           << endl;
    cout << "Escolha uma opcao: ";
}

int main(int argc, char* argv[]) {

    // Verifica se o usuario passou o nome do arquivo como argumento
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.log>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];

    // Tenta abrir o arquivo pra ver se ele existe e da pra ler
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir o arquivo \""
             << nomeArquivo << "\"." << endl;
        return 1;
    }
    arquivo.close();

    cout << "Grafo de roteamento inicializado!" << endl;

    // Loop principal do menu
    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "[opcao 1]" << endl; // FAZER!!!
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
                cout << "Encerrando o programa. Ate mais!" << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
    }

    return 0;
}