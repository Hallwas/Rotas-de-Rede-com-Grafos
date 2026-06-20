#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <queue>
#include <unordered_set>

using namespace std;

struct node{
    string value; // valor do vértice
    vector<node*> links; // lista de ponteiros para os vértices(nodos) vizinhos
};

struct router{
    string ip;
    int indegree;
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

    auto it = find(pfrom->links.begin(), pfrom->links.end(), pto);

    if (it != pfrom->links.end())
    {
        return true;
    }
    return false;

}

// numero de arestas que chegam em um vertice
int indegree(const string& vertice){
    node* grafo = find(vertice);
    if (grafo == nullptr)
    {
        return 0; // o grafo não existe
    }

    int result = 0;

    // percorrendo o unordered_map - lista de adjacencia(contem todos os vértices)
    for(auto nodo : nodes){
        /* percorrendo o vetor de vizinhos de cada vértice(se algum vértice 
        tem o que estamos buscando como vizinho, é uma aresta chegando a ele)*/
        for(auto vizinho : nodo.second.links){
        // se algum aponta para o vértice a ser procurado, +1
        if (vizinho->value == vertice)
        {
            result++;
        }
        }
    }

    return result;

}

// vector = exibir uma mensagem de sucesso informando o número total de vértices únicos e arestas inseridas
vector<int> builder(const string fileName){

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

// função específica para ordenar a struct - primeiro considera o grau, e, em caso de empate, o ip
bool order(const router& a, const router& b){ 
    if (a.indegree != b.indegree) {
        return a.indegree > b.indegree; 
    }
    // empate no grau - testamos pelo nome(ip)
    return a.ip < b.ip;
};

vector<router> critical_routers(){

    vector<router> routers;

    // percorremos o unordered_map chamando a função para calcular o indegree de cada vértice
    for (const auto& vertex : nodes) {
        string value = vertex.first;
        int degree = indegree(value);

        // criamos no formato da struct
        router r = {value, degree};

        // inserimos o resultado no vetor
        routers.push_back(r);
    }

    // ordenamos o vetor usando uma função específica para a struct
    sort(routers.begin(), routers.end(), order);

    // verifica se o grafo tem + de 5 vértices - evitar um erro ao reduzir o vetor
    int limit = min(5, (int)routers.size());

    // mantém apenas o top 5(ou menos de acordo com a verificação anterior)
    routers.resize(limit);

    // devolve o vetor já "formatado"
    return routers;
}

vector<string> shortest_path(const string from, const string to){

    vector<string> path;

    // verificação se o nodo de origem existe
    auto pfrom = find(from);
    if (!pfrom) return path; // retorna vazio caso não exista

    // mesma verificação para o de destino
    auto pto = find(to);
    if (!pto) return path;

    // usamos o set para organizar a lista de nodos já visitados
    unordered_set<node*> visited;

    // conteiner do c++ para FILA - organiza a expansão BFS
    queue<node*> explorationQueue;

    // mapa para armazenar de onde aquele nodo foi descoberto - formato "filho = pai"
    unordered_map<string, string> cameFrom;

    // adicionamos o nodo de origem na fila
    explorationQueue.push(pfrom);

    // marcamos o nodo origem como já visitado
    visited.insert(pfrom);

    // laço de repetição - segue até a fila ficar vazia
    while (!explorationQueue.empty())
    {
        // pega o nodo em primeiro na fila
        auto current = explorationQueue.front(); 
        // tira ele da fila
        explorationQueue.pop();

        // chegamos no destino
        if (current->value == to){
            break;
        }

        // laço para percorrer os vizinhos do nodo atual(1° da fila)
        for(auto& vizinho : current->links){
        /* verificação se o vizinho em questão já não 
        foi visitado(estaria na fila de controle de visitados)*/
        if (visited.count(vizinho) == 0){
            // se não foi visitado, é adicionado para ser
            explorationQueue.push(vizinho); // adicionamos o vizinho à fila
            visited.insert(vizinho); // marca ele como já visitado
            cameFrom[vizinho->value] = current->value; // salvamos no mapa o "filho = pai"
        }
        
        }
    }

    if(visited.count(pto) == 0){ // o destino não foi visitado
        return path;
    }

    // lógica para retornar o vetor de strings com o menor caminho

    // começamos pelo destino
    string currentIp = to;

    // até chegar na origem
    while(currentIp != from){
        // adicionamos o atual no vetor de resposta
        path.push_back(currentIp);

        // atualizamos o atual para passar ao pai dele - é um passo pra trás
        currentIp = cameFrom[currentIp];
    }

    // adicionamos o vetor de origem - cai fora do loop antes de fazer
    path.push_back(from);

    // como é armazenado no formato "filho = pai"(ao contrário), precisamos inverter
    reverse(path.begin(), path.end());
    
    return path;
}