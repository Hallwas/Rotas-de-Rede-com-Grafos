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

// Encontrar menor caminho entre dois IPs
// Usa shortest_path() do grafo.cpp (BFS)
// Usa exportar_com_caminho() do graphviz.cpp
void opcao2(const string& nomeArquivo) {
    string ipOrigem, ipDestino;

    cout << "Digite o IP de Origem: ";
    cin >> ipOrigem;
    cout << "Digite o IP de Destino: ";
    cin >> ipDestino;

    vector<string> caminho = shortest_path(ipOrigem, ipDestino);

    if (caminho.empty()) {
        cout << "\nSem conectividade: nao existe caminho entre "
             << ipOrigem << " e " << ipDestino << "." << endl;
        return;
    }

    // numero de saltos = arestas percorridas = tamanho do vetor - 1
    int totalSaltos = (int)caminho.size() - 1;
    cout << "\nCaminho encontrado (" << totalSaltos << " saltos):" << endl;

    for (int i = 0; i < (int)caminho.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << caminho[i];
    }
    cout << endl;

    // gera o .dot com o caminho destacado e exporta no formato escolhido
    string dotFile = nomeArquivo + "_caminho.dot";
    exportar_com_caminho(dotFile, caminho);

    int formato = exibirSubMenu();
    gerar_saida_visual(dotFile, nomeArquivo, formato);
}

// Calcular diametro do grafo
// Usa diameter() do grafo.cpp
void opcao3() {
    cout << "\nCalculando diametro do grafo..." << endl;

    int d = diameter();

    cout << "Diametro do grafo: " << d << " saltos" << endl;
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
                opcao2(nomeArquivo);
                break;
            case 3:
                opcao3();
                break;
            case 4:
                opcao4(nomeArquivo);
                break;
            case 0:
                cout << "Encerrando o programa. Ate mais!" << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
    }

    return 0;
}