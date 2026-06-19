#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unordered_map>

using namespace std;

// nodes vem do grafo.cpp
extern unordered_map<string, node> nodes;

void gerarDOT(string nomeArquivo = "grafo_completo.dot")
{
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro ao criar arquivo DOT." << endl;
        return;
    }

    arquivo << "digraph Rede {\n";
    arquivo << "    rankdir=LR;\n";
    arquivo << "    node [shape=circle];\n\n";

    for (auto& vertice : nodes)
    {
        string origem = vertice.first;

        for (auto vizinho : vertice.second.links)
        {
            arquivo << "    \""
                    << origem
                    << "\" -> \""
                    << vizinho->value
                    << "\";\n";
        }
    }

    arquivo << "}\n";

    arquivo.close();

    cout << "Arquivo DOT gerado: "
         << nomeArquivo << endl;
}

void gerarImagem()
{
    gerarDOT();

    int resultado = system(
        "dot -Tpng grafo_completo.dot -o grafo_completo.png"
    );

    if(resultado == 0)
    {
        cout << "Imagem gerada: grafo_completo.png" << endl;
    }
    else
    {
        cout << "Graphviz nao encontrado no sistema." << endl;
    }
}

