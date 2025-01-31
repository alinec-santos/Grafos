#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../headers/biblioteca.h"
#include <float.h>

// Definições de cores
#define RESET "\033[0m"
#define RED "\033[31m"

// Funções para criação, leitura e impressão do grafo
void inicializar_grafo(Grafo *g, int n_vertices)
{
    g->n_vertices = n_vertices;
    // Inicializa a matriz de pesos com 0 (sem arestas)
    for (int i = 0; i < n_vertices; i++)
    {
        for (int j = 0; j < n_vertices; j++)
        {
            g->matriz_pesos[i][j] = 0.0; // Inicializa com 0, representando a ausência de arestas
            // printf("matriz inicializada\n");
        }
    }
}

void ler_grafo_de_arquivo(Grafo *g, const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    int n_vertices;
    fscanf(arquivo, "%d", &n_vertices);
    // printf("Número de vértices: %d\n", n_vertices);  // Debug

    // Inicializa o grafo apenas se n_vertices for diferente do atual
    if (g->n_vertices == 0)
    { // Verifica se o grafo ainda não foi inicializado
        inicializar_grafo(g, n_vertices);
    }
    else if (g->n_vertices != n_vertices)
    { // Se o número de vértices mudou
        printf("Mudança no número de vértices não suportada.\n");
        fclose(arquivo);
        return;
    }

    int v1, v2;
    float peso;
    while (fscanf(arquivo, "%d %d %f", &v1, &v2, &peso) == 3)
    {
        // printf("Aresta: %d-%d com peso %.2f\n", v1, v2, peso);  // Debug
        g->matriz_pesos[v1 - 1][v2 - 1] = peso; // Atribui o peso na posição correspondente
        g->matriz_pesos[v2 - 1][v1 - 1] = peso; // Para grafos não direcionados
    }

    fclose(arquivo);
}

void imprimir_grafo(Grafo *g)
{
    if (g->n_vertices == 0)
    {
        printf("O grafo não foi carregado.\n");
        return;
    }

    printf("   "); // Espaço inicial para alinhar o cabeçalho das colunas
    for (int i = 0; i < g->n_vertices; i++)
    {
        printf("%4d ", i + 1); // Imprime o número do vértice da coluna
    }
    printf("\n");

    for (int i = 0; i < g->n_vertices; i++)
    {
        printf("%d ", i + 1); // Imprime o número do vértice da linha
        for (int j = 0; j < g->n_vertices; j++)
        {
            printf("%4.1f ", g->matriz_pesos[i][j]); // Imprime o peso com uma casa decimal
        }
        printf("\n");
    }
}

// Função para calcular a ordem do grafo(nº de vertices)
int ordem_grafo(Grafo *g)
{
    return g->n_vertices;
}

// Função para calcular o tamanho do grafo(nº de arestas)
int tamanho_grafo(Grafo *g)
{

    int tamanho = 0;
    for (int i = 0; i < g->n_vertices; i++)
    {
        for (int j = i + 1; j < g->n_vertices; j++)
        { // metade superior da matriz
            if (g->matriz_pesos[i][j] != 0.0)
            {
                tamanho++;
            }
        }
    }
    return tamanho;
}

// Função para calcular e exibir a densidade do grafo
float calcular_densidade(Grafo *g)
{
    int n_vertices = g->n_vertices;
    int n_arestas = tamanho_grafo(g);

    if (n_vertices < 2)
    {
        return 0.0; // Densidade é zero se o grafo tem menos de 2 vértices
    }

    return (2.0 * n_arestas) / (n_vertices * (n_vertices - 1));
}

void exibir_densidade(Grafo *g)
{
    float densidade = calcular_densidade(g);
    printf("Densidade do grafo: %.2f\n", densidade);
}

// Função para consultar vizinho de um determinado vértice escolhido
void consultar_vizinhos(Grafo *g, int vertice)
{
    if (vertice < 1 || vertice > g->n_vertices)
    {
        printf("Vértice inválido. O número do vértice deve estar entre 1 e %d.\n", g->n_vertices);
        return;
    }

    printf("Vizinhos do vértice %d: ", vertice);
    int encontrou_vizinhos = 0;

    // A matriz de adjacência é 0-indexada, então subtraímos 1 do vértice fornecido
    for (int i = 0; i < g->n_vertices; i++)
    {
        if (g->matriz_pesos[vertice - 1][i] != 0.0)
        {
            printf("%d ", i + 1); // Imprime os vértices vizinhos (convertendo para 1-indexado)
            encontrou_vizinhos = 1;
        }
    }

    if (!encontrou_vizinhos)
    {
        printf("Nenhum vizinho encontrado.");
    }

    printf("\n");
}

// Função para consultar o grau de um determinado vértice escolhido
int consultar_grau(Grafo *g, int vertice)
{
    if (vertice < 1 || vertice > g->n_vertices)
    {
        printf("Vértice inválido. O número do vértice deve estar entre 1 e %d.\n", g->n_vertices);
        return -1; // Retorna um valor negativo para indicar erro
    }

    int grau = 0;

    // A matriz de valores é 0-indexada, então subtraímos 1 do vértice fornecido
    for (int i = 0; i < g->n_vertices; i++)
    {
        // Verifica se há uma aresta, ou seja, se o valor é diferente de 0
        if (g->matriz_pesos[vertice - 1][i] != 0.0)
        {
            grau++; // Incrementa o grau para cada vizinho encontrado
        }
    }
    printf("Grau: %d", grau);
}

void encontrar_articulacoes_util(Grafo *g, int vertice, bool *visitado, int *descoberta, int *low, int *pai, int *articulacao, int *tempo)
{
    int filhos = 0;
    visitado[vertice] = true;
    descoberta[vertice] = low[vertice] = ++(*tempo);

    for (int i = 0; i < g->n_vertices; i++)
    {
        if (g->matriz_pesos[vertice][i] != 0.0)
        { // Se há uma aresta
            if (!visitado[i])
            {
                pai[i] = vertice;
                filhos++;
                encontrar_articulacoes_util(g, i, visitado, descoberta, low, pai, articulacao, tempo);

                low[vertice] = (low[vertice] < low[i]) ? low[vertice] : low[i];

                // Condição para vértice de articulação
                if (pai[vertice] == -1 && filhos > 1)
                {
                    articulacao[vertice] = 1; // Vértice raiz
                }
                if (pai[vertice] != -1 && low[i] >= descoberta[vertice])
                {
                    articulacao[vertice] = 1; // Não raiz
                }
            }
            else if (i != pai[vertice])
            {
                low[vertice] = (low[vertice] < descoberta[i]) ? low[vertice] : descoberta[i];
            }
        }
    }
}

void verificar_articulacao(Grafo *g, int vertice)
{
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));
    int *descoberta = (int *)malloc(g->n_vertices * sizeof(int));
    int *low = (int *)malloc(g->n_vertices * sizeof(int));
    int *pai = (int *)malloc(g->n_vertices * sizeof(int));
    int *articulacao = (int *)malloc(g->n_vertices * sizeof(int));
    int tempo = 0;

    for (int i = 0; i < g->n_vertices; i++)
    {
        visitado[i] = false;
        pai[i] = -1;
        articulacao[i] = 0; // Inicializa como não articulação
    }

    encontrar_articulacoes_util(g, vertice - 1, visitado, descoberta, low, pai, articulacao, &tempo); // -1 para ajustar o índice

    if (articulacao[vertice - 1])
    {
        printf("O vértice %d é uma articulação.\n", vertice);
    }
    else
    {
        printf("O vértice %d não é uma articulação.\n", vertice);
    }

    free(visitado);
    free(descoberta);
    free(low);
    free(pai);
    free(articulacao);
}

void bfs(Grafo *g, int vertice_inicial)
{
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));
    int *fila = (int *)malloc(g->n_vertices * sizeof(int));
    int frente = 0, tras = 0;
    int *pai = (int *)malloc(g->n_vertices * sizeof(int));

    // Inicializa as estruturas
    for (int i = 0; i < g->n_vertices; i++)
    {
        visitado[i] = false;
        pai[i] = -1; // Inicializa o pai como -1
    }

    // Marca o vértice inicial como visitado
    visitado[vertice_inicial - 1] = true;
    fila[tras++] = vertice_inicial - 1; // Adiciona o vértice inicial à fila

    printf("Sequência de vértices visitados na BFS: ");

    while (frente < tras)
    {
        int vertice_atual = fila[frente++];
        printf("%d ", vertice_atual + 1); // Imprime o vértice visitado (ajusta para 1-indexado)

        // Explora os vizinhos do vértice atual
        for (int i = 0; i < g->n_vertices; i++)
        {
            if (g->matriz_pesos[vertice_atual][i] != 0.0)
            { // Se há uma aresta
                if (!visitado[i])
                {
                    visitado[i] = true;     // Marca como visitado
                    fila[tras++] = i;       // Adiciona à fila
                    pai[i] = vertice_atual; // Define o pai
                }
            }
        }
    }

    // Agora, vamos verificar as arestas que não fazem parte da árvore de busca
    printf("\nArestas que não fazem parte da árvore de busca:\n");
    for (int i = 0; i < g->n_vertices; i++)
    {
        for (int j = 0; j < g->n_vertices; j++)
        {
            if (g->matriz_pesos[i][j] != 0.0)
            { // Se há uma aresta
                // Verifica se a aresta (i, j) não faz parte da árvore de busca
                if (pai[j] != i && pai[i] != j)
                {
                    printf("A aresta (%d, %d) não faz parte da árvore de busca.\n", i + 1, j + 1);
                }
            }
        }
    }

    // Libera a memória
    free(visitado);
    free(fila);
    free(pai);
}

void dfs_componente_conexa(Grafo *g, int vertice, bool *visitado, int *componente, int *tamanho)
{
    visitado[vertice] = true;
    componente[*tamanho] = vertice + 1; // Armazena o vértice (ajusta para 1-indexado)
    (*tamanho)++;

    for (int i = 0; i < g->n_vertices; i++)
    {
        if (g->matriz_pesos[vertice][i] != 0.0 && !visitado[i])
        { // Se há uma aresta e não foi visitado
            dfs_componente_conexa(g, i, visitado, componente, tamanho);
        }
    }
}

void componentes_conexas(Grafo *g)
{
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));
    int *componente = (int *)malloc(g->n_vertices * sizeof(int));
    int numero_componentes = 0;

    for (int i = 0; i < g->n_vertices; i++)
    {
        visitado[i] = false; // Inicializa todos os vértices como não visitados
    }

    printf("Componentes conexas:\n");

    for (int i = 0; i < g->n_vertices; i++)
    {
        if (!visitado[i])
        { // Se o vértice não foi visitado, é o início de uma nova componente
            int tamanho = 0;
            dfs_componente_conexa(g, i, visitado, componente, &tamanho);
            numero_componentes++;

            printf("Componente %d: ", numero_componentes);
            for (int j = 0; j < tamanho; j++)
            {
                printf("%d ", componente[j]);
            }
            printf("\n");
        }
    }

    printf("Número total de componentes conexas: %d\n", numero_componentes);

    // Libera a memória
    free(visitado);
    free(componente);
}

bool dfs_ciclo(Grafo *g, int vertice, bool *visitado, int pai)
{
    visitado[vertice] = true; // Marca o vértice como visitado

    for (int i = 0; i < g->n_vertices; i++)
    {
        if (g->matriz_pesos[vertice][i] != 0.0)
        { // Se há uma aresta
            if (!visitado[i])
            {
                // Se o vizinho não foi visitado, continue a DFS
                if (dfs_ciclo(g, i, visitado, vertice))
                {
                    return true; // Ciclo encontrado
                }
            }
            else if (i != pai)
            {
                // Se o vizinho já foi visitado e não é o pai, então há um ciclo
                return true;
            }
        }
    }
    return false; // Nenhum ciclo encontrado a partir deste vértice
}

void gerar_grafo_aleatorio(int n_vertices, int n_arestas, const char *nome_arquivo, int tipo_peso)
{
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo)
    {
        printf("Erro ao criar o arquivo: %s\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "%d\n", n_vertices); // Escreve o número de vértices

    // Para evitar arestas duplicadas, vamos usar um conjunto
    bool **arestas_existentes = (bool **)malloc(n_vertices * sizeof(bool *));
    for (int i = 0; i < n_vertices; i++)
    {
        arestas_existentes[i] = (bool *)calloc(n_vertices, sizeof(bool));
    }

    int arestas_criadas = 0;
    while (arestas_criadas < n_arestas)
    {
        int v1 = rand() % n_vertices; // Vértice 1
        int v2 = rand() % n_vertices; // Vértice 2

        // Evita arestas de um vértice para ele mesmo e arestas duplicadas
        if (v1 != v2 && !arestas_existentes[v1][v2])
        {
            float peso;
            if (tipo_peso == 1)
            {
                peso = ((float)rand() / RAND_MAX) * 10; // Peso positivo entre 0 e 10
            }
            else if (tipo_peso == 2)
            {
                peso = ((float)rand() / RAND_MAX) * 10 - 10; // Peso negativo entre -10 e 0
            }
            else
            {
                peso = ((float)rand() / RAND_MAX) * 20 - 10; // Peso entre -10 e 10
            }
            fprintf(arquivo, "%d %d %.2f\n", v1 + 1, v2 + 1, peso); // Escreve a aresta
            arestas_existentes[v1][v2] = true;                      // Marca aresta como existente
            arestas_existentes[v2][v1] = true;                      // Para grafos não direcionados
            arestas_criadas++;
        }
    }

    // Libera a memória
    for (int i = 0; i < n_vertices; i++)
    {
        free(arestas_existentes[i]);
    }
    free(arestas_existentes);
    fclose(arquivo);
    printf("Grafo gerado e salvo em %s\n", nome_arquivo);
}

bool possui_ciclo(Grafo *g)
{
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));

    // Inicializa todos os vértices como não visitados
    for (int i = 0; i < g->n_vertices; i++)
    {
        visitado[i] = false;
    }

    // Verifica cada componente do grafo
    for (int i = 0; i < g->n_vertices; i++)
    {
        if (!visitado[i])
        { // Se o vértice não foi visitado
            if (dfs_ciclo(g, i, visitado, -1))
            {
                free(visitado);
                return true; // Ciclo encontrado
            }
        }
    }

    free(visitado);
    return false; // Nenhum ciclo encontrado
}
// Função para imprimir o caminho de um vértice até o vértice inicial
void imprimir_caminho(int *pai, int v)
{
    if (v == -1)
    {
        printf("Nenhum caminho");
        return;
    }

    if (pai[v] == -1)
    {
        printf("%d", v + 1); // Ajuste para índice 1
        return;
    }

    imprimir_caminho(pai, pai[v]);
    printf(" -> %d", v + 1); // Ajuste para índice 1
}

void initialize_single_source(Grafo *g, int vertice_inicial, float *distancias, int *pai)
{
    for (int i = 0; i < g->n_vertices; i++)
    {
        distancias[i] = FLT_MAX; // Inicializa como infinito
        pai[i] = -1;             // Inicializa o pai como -1
    }
    distancias[vertice_inicial - 1] = 0; // Distância do vértice inicial é 0 (ajustado para índice 0)
}

void relax(int u, int v, float peso, float *distancias, int *pai)
{
    if (distancias[u] != FLT_MAX && distancias[u] + peso < distancias[v])
    {
        distancias[v] = distancias[u] + peso;
        pai[v] = u; // Atualiza o pai de v
    }
}

void bellman_ford(Grafo *g, int vertice_inicial)
{
    int n_vertices = g->n_vertices;
    float *distancias = (float *)malloc(n_vertices * sizeof(float));
    int *pai = (int *)malloc(n_vertices * sizeof(int));

    initialize_single_source(g, vertice_inicial, distancias, pai);

    // Relaxa as arestas (n_vertices - 1) vezes
    for (int i = 0; i < n_vertices - 1; i++)
    {
        for (int u = 0; u < n_vertices; u++)
        {
            for (int v = 0; v < n_vertices; v++)
            {
                if (g->matriz_pesos[u][v] != 0.0)
                { // Se há uma aresta entre u e v
                    relax(u, v, g->matriz_pesos[u][v], distancias, pai);
                }
            }
        }
    }

    // Verifica se há ciclos de peso negativo
    bool ciclo_negativo = false;
    for (int u = 0; u < n_vertices; u++)
    {
        for (int v = 0; v < n_vertices; v++)
        {
            if (g->matriz_pesos[u][v] != 0.0)
            { // Se há uma aresta
                if (distancias[u] != FLT_MAX && distancias[u] + g->matriz_pesos[u][v] < distancias[v])
                {
                    ciclo_negativo = true; // Ciclo negativo encontrado
                }
            }
        }
    }

    // Se houver um ciclo negativo, avise o usuário
    if (ciclo_negativo)
    {
        printf("\n" RED "Aviso: O grafo contém um ciclo de peso negativo. As distâncias podem não ser válidas.\n" RESET "\n");
    }

    // Imprime as distâncias e caminhos
    printf("Vértice\tDistância\tCaminho\n");
    for (int i = 0; i < n_vertices; i++)
    {
        printf("%d\t", i + 1); // Ajusta para 1-indexado
        if (distancias[i] == FLT_MAX)
        {
            printf("Infinito\t");
        }
        else
        {
            printf("%.2f\t\t", distancias[i]);
        }
        if (ciclo_negativo == false && distancias[i] != FLT_MAX)
        {
            imprimir_caminho(pai, i); // Chama a função para imprimir o caminho até o vértice
        }
        else
        {
            printf("Nenhum caminho");
        }
        printf("\n");
    }

    // Libera a memória
    free(distancias);
    free(pai);
}


void escrever_arvore_minima(const char *nome_arquivo, Aresta *arestas, int num_arestas, float peso_total) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao criar arquivo da árvore mínima\n");
        return;
    }

    fprintf(arquivo, "%d\n", num_arestas + 1); // número de vértices
    for (int i = 0; i < num_arestas; i++) {
        fprintf(arquivo, "%d %d %.2f\n", arestas[i].origem + 1, arestas[i].destino + 1, arestas[i].peso);
    }
    fprintf(arquivo, "\nPeso total da árvore: %.2f\n", peso_total);
    fclose(arquivo);
}

void prim(Grafo *g, const char *nome_arquivo) {
    int n = g->n_vertices;
    float *chave = (float *)malloc(n * sizeof(float));
    int *pai = (int *)malloc(n * sizeof(int));
    bool *na_agm = (bool *)malloc(n * sizeof(bool));
    Aresta *arestas = (Aresta *)malloc((n-1) * sizeof(Aresta));
    float peso_total = 0;

    for (int i = 0; i < n; i++) {
        chave[i] = FLT_MAX;
        na_agm[i] = false;
    }

    chave[0] = 0;
    pai[0] = -1;
    int arestas_count = 0;

    for (int count = 0; count < n; count++) {
        float min = FLT_MAX;
        int u = -1;

        for (int v = 0; v < n; v++) {
            if (!na_agm[v] && chave[v] < min) {
                min = chave[v];
                u = v;
            }
        }

        if (u == -1) break;
        na_agm[u] = true;

        if (pai[u] != -1) {
            arestas[arestas_count].origem = pai[u];
            arestas[arestas_count].destino = u;
            arestas[arestas_count].peso = g->matriz_pesos[pai[u]][u];
            peso_total += arestas[arestas_count].peso;
            arestas_count++;
        }

        for (int v = 0; v < n; v++) {
            if (g->matriz_pesos[u][v] && !na_agm[v] && g->matriz_pesos[u][v] < chave[v]) {
                pai[v] = u;
                chave[v] = g->matriz_pesos[u][v];
            }
        }
    }

    escrever_arvore_minima(nome_arquivo, arestas, arestas_count, peso_total);
    printf("Árvore geradora mínima salva em %s\n", nome_arquivo);
    printf("Peso total da árvore: %.2f\n", peso_total);

    free(chave);
    free(pai);
    free(na_agm);
    free(arestas);
}
// Implementação de uma heurística gulosa para cobertura mínima de vértices
void cobertura_minima_vertices(Grafo *g) {


    bool *na_cobertura = (bool *)malloc(g->n_vertices * sizeof(bool));
    bool **aresta_coberta = (bool **)malloc(g->n_vertices * sizeof(bool *));
    
    for(int i = 0; i < g->n_vertices; i++) {
        aresta_coberta[i] = (bool *)calloc(g->n_vertices, sizeof(bool));
        na_cobertura[i] = false;
    }

    int *graus = (int *)malloc(g->n_vertices * sizeof(int));
    for(int i = 0; i < g->n_vertices; i++) {
        graus[i] = 0;
        for(int j = 0; j < g->n_vertices; j++) {
            if(g->matriz_pesos[i][j] != 0) {
                graus[i]++;
            }
        }
    }

    bool tem_aresta_nao_coberta = true;
    while(tem_aresta_nao_coberta) {
        int max_grau = -1;
        int vertice_max_grau = -1;
        
        for(int i = 0; i < g->n_vertices; i++) {
            if(!na_cobertura[i] && graus[i] > max_grau) {
                max_grau = graus[i];
                vertice_max_grau = i;
            }
        }

        if(vertice_max_grau == -1 || max_grau == 0) {
            break;
        }

        na_cobertura[vertice_max_grau] = true;

        for(int i = 0; i < g->n_vertices; i++) {
            if(g->matriz_pesos[vertice_max_grau][i] != 0 && !aresta_coberta[vertice_max_grau][i]) {
                aresta_coberta[vertice_max_grau][i] = true;
                aresta_coberta[i][vertice_max_grau] = true;
                graus[i]--;
            }
        }
        graus[vertice_max_grau] = 0;

        tem_aresta_nao_coberta = false;
        for(int i = 0; i < g->n_vertices && !tem_aresta_nao_coberta; i++) {
            for(int j = 0; j < g->n_vertices; j++) {
                if(g->matriz_pesos[i][j] != 0 && !aresta_coberta[i][j]) {
                    tem_aresta_nao_coberta = true;
                    break;
                }
            }
        }
    }

    printf("Vértices na cobertura mínima: ");
    int tamanho_cobertura = 0;
    for(int i = 0; i < g->n_vertices; i++) {
        if(na_cobertura[i]) {
            printf("%d ", i + 1);
            tamanho_cobertura++;
        }
    }
    printf("\nTamanho da cobertura: %d\n", tamanho_cobertura);

    for(int i = 0; i < g->n_vertices; i++) {
        free(aresta_coberta[i]);
    }
    free(aresta_coberta);
    free(na_cobertura);
    free(graus);
}

// Implementação do algoritmo de emparelhamento máximo
bool bpm(Grafo *g, int u, bool *visitado, int *emparelhamento) {
    for (int v = 0; v < g->n_vertices; v++) {
        if (g->matriz_pesos[u][v] != 0 && !visitado[v]) {
            visitado[v] = true;
            
            if (emparelhamento[v] == -1 || bpm(g, emparelhamento[v], visitado, emparelhamento)) {
                emparelhamento[v] = u;
                return true;
            }
        }
    }
    return false;
}

void emparelhamento_maximo(Grafo *g) {
    int *emparelhamento = (int *)malloc(g->n_vertices * sizeof(int));
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));
    
    for (int i = 0; i < g->n_vertices; i++) {
        emparelhamento[i] = -1;
    }

    int resultado = 0;
    for (int u = 0; u < g->n_vertices; u++) {
        memset(visitado, false, g->n_vertices * sizeof(bool));
        if (bpm(g, u, visitado, emparelhamento)) {
            resultado++;
        }
    }

    printf("Tamanho do emparelhamento máximo: %d\n", resultado);
    printf("Arestas no emparelhamento:\n");
    for (int i = 0; i < g->n_vertices; i++) {
        if (emparelhamento[i] != -1) {
            printf("%d - %d\n", emparelhamento[i] + 1, i + 1);
        }
    }

    free(emparelhamento);
    free(visitado);
}

// Cálculo da centralidade de proximidade
float calcular_distancia(Grafo *g, int origem, int destino) {
    float *distancias = (float *)malloc(g->n_vertices * sizeof(float));
    bool *visitado = (bool *)malloc(g->n_vertices * sizeof(bool));
    
    for (int i = 0; i < g->n_vertices; i++) {
        distancias[i] = FLT_MAX;
        visitado[i] = false;
    }
    
    distancias[origem] = 0;
    
    for (int count = 0; count < g->n_vertices - 1; count++) {
        float min = FLT_MAX;
        int u = -1;
        
        for (int v = 0; v < g->n_vertices; v++) {
            if (!visitado[v] && distancias[v] < min) {
                min = distancias[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        visitado[u] = true;
        
        for (int v = 0; v < g->n_vertices; v++) {
            if (!visitado[v] && g->matriz_pesos[u][v] != 0 && 
                distancias[u] != FLT_MAX && 
                distancias[u] + g->matriz_pesos[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + g->matriz_pesos[u][v];
            }
        }
    }
    
    float dist = distancias[destino];
    free(distancias);
    free(visitado);
    return dist;
}

void centralidade_proximidade(Grafo *g, int vertice) {
    if (vertice < 1 || vertice > g->n_vertices) {
        printf("Vértice inválido!\n");
        return;
    }
    
    float soma_distancias = 0;
    vertice--; // Ajusta para índice 0
    
    for (int i = 0; i < g->n_vertices; i++) {
        if (i != vertice) {
            float dist = calcular_distancia(g, vertice, i);
            if (dist == FLT_MAX) {
                printf("Grafo não é conexo. Centralidade não pode ser calculada.\n");
                return;
            }
            soma_distancias += dist;
        }
    }
    
    float centralidade = (g->n_vertices - 1) / soma_distancias;
    printf("Centralidade de proximidade do vértice %d: %.4f\n", vertice + 1, centralidade);
}