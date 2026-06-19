#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "grafo.cpp"
#include "graphviz.cpp"

using namespace std;

// Submenu para escolher o formato de saida do Graphviz
int exibirSubMenu() {
    int opcao;
    cout << "\nSelecione o formato de saida:" << endl;
    cout << "1. Tela"            << endl;
    cout << "2. Imagem (PNG)"    << endl;
    cout << "3. Documento (PDF)" << endl;
    cout << "Opcao: ";
    cin >> opcao;
    return opcao;
}

// Menu principal
void exibirMenu() {
    cout << "\n======================================================" << endl;
    cout << "1. Exibir Grafo Completo"                                 << endl;
    cout << "2. Encontrar Menor Caminho"                               << endl;
    cout << "3. Calcular o Diametro do Grafo"                         << endl;
    cout << "4. Identificar Roteadores Criticos"                      << endl;
    cout << "0. Sair"                                                  << endl;
    cout << "======================================================" << endl;
    cout << "Escolha uma opcao: ";
}

// Exibir grafo completo
void opcao1(const string& nomeArquivo) {
    string dotFile = nomeArquivo + ".dot";
    exportar_grafo_completo(dotFile);

    int formato = exibirSubMenu();
    gerar_saida_visual(dotFile, nomeArquivo, formato);
}

// Identificar roteadores criticos (top 5 in-degree)
void opcao4(const string& nomeArquivo) {
    vector<router> top5 = critical_routers();

    if (top5.empty()) {
        cout << "Nenhum vertice encontrado." << endl;
        return;
    }

    cout << "\nTop 5 roteadores com maior grau de entrada (in-degree):" << endl;
    for (int i = 0; i < (int)top5.size(); i++) {
        cout << (i + 1) << ". " << top5[i].ip
             << "  (in-degree: " << top5[i].indegree << ")" << endl;
    }

    string dotFile = nomeArquivo + "_criticos.dot";
    exportar_roteadores_criticos_dot(dotFile, top5);

    int formato = exibirSubMenu();
    gerar_saida_visual(dotFile, nomeArquivo, formato);
}

// main
int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo.log>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];

    vector<int> resultado = builder(nomeArquivo);

    cout << "Grafo de roteamento inicializado!" << endl;
    cout << "Vertices unicos (IPs): " << resultado[0]
         << " | Arestas: "            << resultado[1] << endl;

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
            case 1:
                opcao1(nomeArquivo);
                break;
            case 2:
                cout << "[opcao 2 ainda nao implementada]" << endl;
                break;
            case 3:
                cout << "[opcao 3 ainda nao implementada]" << endl;
                break;
            case 4:
                opcao4(nomeArquivo);
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