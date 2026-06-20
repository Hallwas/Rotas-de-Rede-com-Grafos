#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void exportar_grafo_completo(string nomeArquivo) {

    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo \"" << nomeArquivo << "\"" << endl;
        return;
    }

    //Cabecalho do formato DOT para grafos direcionados
    arquivo << "digraph G {" << endl;
    arquivo << "  rankdir=LR;" << endl;                        //layout da esquerda para a direita
    arquivo << "  node [shape=box, style=rounded];" << endl;   //estilo dos nos
    arquivo << endl;

    //Percorre todos os nos da lista de adjacencias
    for (auto& par : nodes) {
        node& nodo = par.second; // nodo atual

        //Para cada vizinho do nodo atual, escrevemos a aresta
        for (int i = 0; i < (int)nodo.links.size(); i++) {
            node* vizinho = nodo.links[i];
            arquivo << "  \"" << nodo.value << "\" -> \"" << vizinho->value << "\";" << endl;
        }
    }

    arquivo << "}" << endl;
    arquivo.close();

    cout << "Arquivo DOT gerado: " << nomeArquivo << endl;
}

// Gera um arquivo .dot igual ao grafo completo, mas destaca em azul
// os nos do caminho e em vermelho as arestas que fazem parte dele.
void exportar_com_caminho(string nomeArquivo, vector<string> caminho) {

    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo \"" << nomeArquivo << "\"" << endl;
        return;
    }

    arquivo << "digraph G {" << endl;
    arquivo << "  rankdir=LR;" << endl;
    arquivo << "  node [shape=box, style=rounded];" << endl;
    arquivo << endl;

    //Destaca os nos que fazem parte do caminho em azul claro
    arquivo << "  // Nos do caminho destacados em azul" << endl;
    for (int i = 0; i < (int)caminho.size(); i++) {
        arquivo << "  \"" << caminho[i] << "\" [style=\"filled,rounded\", fillcolor=lightblue];" << endl;
    }

    //Destaca o no de origem em verde e o de destino em vermelho
    if (caminho.size() >= 1) {
        arquivo << "  \"" << caminho[0] << "\" [style=\"filled,rounded\", fillcolor=lightgreen];" << endl;
    }
    if (caminho.size() >= 2) {
        arquivo << "  \"" << caminho[caminho.size() - 1] << "\" [style=\"filled,rounded\", fillcolor=lightsalmon];" << endl;
    }

    arquivo << endl;

    //Escreve todas as arestas, destacando as que estao no caminho
    for (auto& par : nodes) {
        node& nodo = par.second;

        for (int i = 0; i < (int)nodo.links.size(); i++) {
            node* vizinho = nodo.links[i];

            //Verifica se essa aresta (nodo -> vizinho) faz parte do caminho
            bool aresta_no_caminho = false;
            for (int j = 0; j < (int)caminho.size() - 1; j++) {
                if (caminho[j] == nodo.value && caminho[j + 1] == vizinho->value) {
                    aresta_no_caminho = true;
                    break;
                }
            }

            if (aresta_no_caminho) {
                // Aresta do caminho: vermelha e mais grossa
                arquivo << "  \"" << nodo.value << "\" -> \"" << vizinho->value
                        << "\" [color=red, penwidth=2.5];" << endl;
            } else {
                // Aresta normal: cinza claro para nao poluir
                arquivo << "  \"" << nodo.value << "\" -> \"" << vizinho->value
                        << "\" [color=gray];" << endl;
            }
        }
    }

    arquivo << "}" << endl;
    arquivo.close();

    cout << "Arquivo DOT com caminho destacado gerado: " << nomeArquivo << endl;
}

void exportar_com_caminho(string nomeArquivo, const vector<string>& caminho) {
    ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) return;

    arquivo << "digraph G {" << endl;
    arquivo << "  rankdir=LR;" << endl;
    
    // Criamos um set para consulta rápida se um IP faz parte do caminho
    unordered_set<string> setCaminho(caminho.begin(), caminho.end());

    for (const auto& par : nodes) {
        string u = par.first;
        // Pinta nós do caminho de vermelho
        if (setCaminho.count(u)) {
            arquivo << "  \"" << u << "\" [style=filled, fillcolor=red, fontcolor=white];" << endl;
        }

        for (node* vizinho : par.second.links) {
            string v = vizinho->value;
            bool arestaNoCaminho = false;
            // Verifica se a aresta (u->v) faz parte do caminho
            for(size_t i = 0; i < caminho.size() - 1; ++i) {
                if(caminho[i] == u && caminho[i+1] == v) arestaNoCaminho = true;
            }
            
            if (arestaNoCaminho)
                arquivo << "  \"" << u << "\" -> \"" << v << "\" [color=red, penwidth=2.0];" << endl;
            else
                arquivo << "  \"" << u << "\" -> \"" << v << "\";" << endl;
        }
    }
    arquivo << "}" << endl;
    arquivo.close();
}

// Gera um arquivo .dot igual ao grafo completo, mas destaca em laranja
// os roteadores criticos (com maior indegree), mostrando o valor do indegree como rotulo extra no no.
void exportar_roteadores_criticos_dot(string nomeArquivo, vector<router> criticos) {

    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel criar o arquivo \"" << nomeArquivo << "\"" << endl;
        return;
    }

    arquivo << "digraph G {" << endl;
    arquivo << "  rankdir=LR;" << endl;
    arquivo << "  node [shape=box, style=rounded];" << endl;
    arquivo << endl;

    //Destaca os roteadores criticos em laranja com o indegree no rotulo
    arquivo << "  // Roteadores criticos destacados em laranja" << endl;
    for (int i = 0; i < (int)criticos.size(); i++) {
        // \\n dentro da string gera uma quebra de linha no rotulo do no no DOT
        arquivo << "  \"" << criticos[i].ip
                << "\" [style=\"filled,rounded\", fillcolor=orange, "
                << "label=\"" << criticos[i].ip << "\\n(in: " << criticos[i].indegree << ")\"];"
                << endl;
    }

    arquivo << endl;

    //Escreve todas as arestas normalmente
    for (auto& par : nodes) {
        node& nodo = par.second;

        for (int i = 0; i < (int)nodo.links.size(); i++) {
            node* vizinho = nodo.links[i];
            arquivo << "  \"" << nodo.value << "\" -> \"" << vizinho->value << "\";" << endl;
        }
    }

    arquivo << "}" << endl;
    arquivo.close();

    cout << "Arquivo DOT com roteadores criticos gerado: " << nomeArquivo << endl;
}


//Chama o programa 'dot' do Graphviz paraconverter um arquivo .dot em uma imagem PNG.
void gerar_saida_visual(string arquivoDot, string arquivoSaidaBase, int formato) {
    string comando;
    string arquivoFinal;

    if (formato == 2 || formato == 1) { 
        // 2 = PNG, 1 = Tela (gera PNG e tenta abrir)
        arquivoFinal = arquivoSaidaBase + ".png";
        comando = "dot -Tpng \"" + arquivoDot + "\" -o \"" + arquivoFinal + "\"";
    } else if (formato == 3) { 
        // 3 = PDF
        arquivoFinal = arquivoSaidaBase + ".pdf";
        comando = "dot -Tpdf \"" + arquivoDot + "\" -o \"" + arquivoFinal + "\"";
    } else {
        cout << "Formato invalido!" << endl;
        return;
    }

    cout << "Gerando visualizacao via Graphviz..." << endl;
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        cout << "Arquivo gerado com sucesso: " << arquivoFinal << endl;
        
        // Funcionalidade de abrir na Tela (Opcao 1)
        if (formato == 1) {
            #ifdef _WIN32
                string comandoAbrir = "start \"\" \"" + arquivoFinal + "\""; // Windows
            #elif __APPLE__
                string comandoAbrir = "open \"" + arquivoFinal + "\""; // Mac
            #else
                string comandoAbrir = "xdg-open \"" + arquivoFinal + "\""; // Linux
            #endif
            system(comandoAbrir.c_str());
        }
    } else {
        cout << "AVISO: Falha ao executar o Graphviz." << endl;
        cout << "Verifique se ele esta instalado e adicionado ao PATH do sistema." << endl;
    }
}