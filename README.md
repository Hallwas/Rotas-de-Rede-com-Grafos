# Analisador de Rotas de Rede com Grafos

Este projeto é a implementação do trabalho final da disciplina de Estrutura de Dados II do curso de Ciência da Computação da UPF(Universidade de Passo Fundo), desenvolvido pelos alunos: André Bohm Hallwass, João Gabriel Dal'Forno Casagrande e Lucca Lampert De Conto. 

O projeto é uma aplicação em C++ focada na análise de topologia e diagnóstico de rotas em redes de computadores a partir de dados reais extraídos de ferramentas como o traceroute. O sistema usa esses dados, recebidos através de um arquivo de log como argumento de linha de comando, para construir um grafo direcionado não ponderado em memória, com uma estrutura de lista de adjacência. Nessa estrutura, cada roteador com um IP válido é tratado como um vértice e cada salto de comunicação atua como uma aresta. O objetivo é aplicar algoritmos clássicos sobre essa topologia para extrair métricas da rede, além de exibir o grafo utilizando o pacote Graphviz.
