#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
#include <stdbool.h>
#define MAX_VERTICES 100 // Defina um tamanho máximo para a matriz

typedef struct
{
    int n_vertices;
    float matriz_pesos[MAX_VERTICES][MAX_VERTICES]; // Matriz de pesos
} Grafo;

typedef struct
{
    int origem;
    int destino;
    float peso;
} Aresta;


// Funções para manipular o grafo
void inicializar_grafo(Grafo *g, int n_vertices);
void ler_grafo_de_arquivo(Grafo *g, const char *nome_arquivo);
void imprimir_grafo(Grafo *g);
int ordem_grafo(Grafo *g);
int tamanho_grafo(Grafo *g);
float calcular_densidade(Grafo *g);
void exibir_densidade(Grafo *g);
void consultar_vizinhos(Grafo *g, int vertice);
int consultar_grau(Grafo *g, int vertice);
void encontrar_articulacoes_util(Grafo *g, int vertice, bool *visitado, int *descoberta, int *low, int *pai, int *articulacao, int *tempo);
void verificar_articulacao(Grafo *g, int vertice);
void bfs(Grafo *g, int vertice_inicial);
void dfs_componente_conexa(Grafo *g, int vertice, bool *visitado, int *componente, int *tamanho);
void componentes_conexas(Grafo *g);
bool possui_ciclo(Grafo *g);
bool dfs_ciclo(Grafo *g, int vertice, bool *visitado, int pai);
void bellman_ford(Grafo *g, int vertice_inicial);
void imprimir_caminho(int *pai, int vertice);
void gerar_grafo_aleatorio(int n_vertices, int n_arestas, const char *nome_arquivo, int tipo_peso);
void initialize_single_source(Grafo *g, int vertice_inicial, float *distancias, int *pai);
void relax(int u, int v, float peso, float *distancias, int *pai);
void prim(Grafo *g, const char *nome_arquivo) ;
void escrever_arvore_minima(const char *nome_arquivo, Aresta *arestas, int num_arestas, float peso_total);
void cobertura_minima_vertices(Grafo *g) ;
bool bpm(Grafo *g, int u, bool *visitado, int *emparelhamento) ;
void emparelhamento_maximo(Grafo *g) ;
float calcular_distancia(Grafo *g, int origem, int destino) ;
void centralidade_proximidade(Grafo *g, int vertice);
#endif
