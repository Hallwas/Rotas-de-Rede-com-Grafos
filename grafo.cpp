#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

struct node{
    string value; // valor do vértice
    vector<node*> links; // lista de ponteiros para os vértices(nodos) vizinhos
};

// Lista de Adjacências - tabela hash para armazenar as conexões entre os vértices(arestas) 
// chave = o valor do vértice
// valor = o vértice por completo(usamos para acessar os vizinhos)
unordered_map<string, node> nodes;

// método para inserir um vértice ao grafo com o valor 'S'
void insert_node(const string& s){
    // variável auxiliar para criar o vértice e o inserir
    node aux;
    aux.value = s;

    // adicionamos o vértice à tabela hash(lista de adjacências)
    nodes[s] = aux;
}

// função que busca um vértice pelo seu valor 's' e retorna o endereço dele
node* find(const string& s){
    auto it = nodes.find(s);
    if (it == nodes.end()){ // não existe o vértice com valor 's'
        return nullptr;
    } else{ // existe
        return &it->second; // retorna um ponteiro para o nodo inteiro(valor do hash)
    }
}

// função para ligar um vértice(origem) a outro(destino) - criar uma aresta dirigida
bool insert_link(const string& from, const string& to){
    // usando a função 'find()' para verificar se a origem existe
    auto pfrom = find(from);
    if (pfrom == nullptr){
        return false; // o vértice de origem da aresta não existe
    }
    // fazemos a mesma verificação para o vértice de destino
    auto pto = find(to);
    if (pto == nullptr){
        return false;
    }

    // acessamos a lista de vizinhos do vértice de origem e adicionamos o de destino
    pfrom->links.push_back(pto);
    return true;

}

// função para verificar se uma aresta existe
bool link_exist(const string& from, const string& to){
    // usando a função 'find()' para verificar se a origem existe
    auto pfrom = find(from);
    if (pfrom == nullptr){
        return false; // o vértice de origem da aresta não existe
    }
    // fazemos a mesma verificação para o vértice de destino
    auto pto = find(to);
    if (pto == nullptr){
        return false;
    }

    auto it = std::find(pfrom->links.begin(), pfrom->links.end(), pto);

    if (it != pfrom->links.end())
    {
        return true;
    }
    return false;

}

// vector = exibir uma mensagem de sucesso informando o número total de vértices únicos e arestas inseridas
vector<int> builder(string fileName){

    int linkCount = 0;

    ifstream file(fileName);
    if (!file.is_open()) 
    {
        cerr << "Erro: não foi possivel abrir o arquivo. \"" << fileName << "\"." << endl;
        exit(1);
    }

    // "descartar" o cabeçalho do arquivo
    string trash;
    getline(file, trash);
    
    while (getline(file, trash, ',')) // lê a 1ª coluna e entra no while apontando para a 2ª coluna
    { 
        string from, to;

        getline(file, trash, ','); // descarta a 2° coluna
        getline(file, trash, ','); // descarta a 3° coluna
        getline(file, trash, ','); // descarta a 4° coluna
        getline(file, from, ',');
        getline(file, to, ',');
        getline(file, trash); // descarta a 7° coluna

        // verificação de timeout de rede ou estão em branco
        if (to == "*" || from == "" || to == "") 
        {
            continue;
        }
        
        // verificação se o vértice de origem já existe
        auto pfrom = find(from);
        if (pfrom == nullptr){
            insert_node(from); // o vértice de origem da aresta não existe - adicionamos
        }
        // fazemos a mesma verificação para o vértice de destino
        auto pto = find(to);
        if (pto == nullptr){
            insert_node(to);
        }

        if(link_exist(from, to) == false){ // verificamos se a aresta entre eles ainda não existe - criamos
            insert_link(from, to);
            linkCount++;
        }

    }

    file.close();

    vector<int> response;

    response.push_back(nodes.size());
    response.push_back(linkCount);

    return response;
}
