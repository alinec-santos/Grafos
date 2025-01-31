#include <stdio.h>
#include <stdlib.h>
#include "../headers/menu.h"
#include "../headers/biblioteca.h"

void exibir_menu_principal() {
    printf("╔════════════════════════════════════╗\n");
    printf("║        GRAFO - MENU PRINCIPAL      ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║  1. Carregar grafo de um arquivo   ║\n");
    printf("║  2. Imprimir grafo                 ║\n");
    printf("║  3. Ordem do grafo                 ║\n");
    printf("║  4. Tamanho do grafo               ║\n");
    printf("║  5. Densidade ε(G) do grafo        ║\n");
    printf("║  6. Vizinhos de um vértice         ║\n");
    printf("║  7. Grau de um vértice             ║\n");
    printf("║  8. O grafo possui articulação?    ║\n");
    printf("║  9. Busca em largura               ║\n");
    printf("║ 10. Nº componentes conexos         ║\n");
    printf("║ 11. O grafo possui ciclo?          ║\n");
    printf("║ 12. Distância e caminho mínimo     ║\n");
    printf("║ 13. Gerar grafo aleatório          ║\n");
    printf("║ 14. Sair                           ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("Escolha uma opção: ");
}

void pausar() {
    printf("\nPressione Enter para voltar ao menu...");
    getchar(); getchar();
}

void exibir_menu() {
    Grafo g = {0}; 
    int opcao, opcaoso;
    
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ Para que sua experiência na nossa biblioteca ║\n");
    printf("║ seja ainda mais completa, nos diga:          ║\n");
    printf("║ Qual é o seu sistema operacional?            ║\n");
    printf("║ 1- Linux                                     ║\n");
    printf("║ 2- Windows                                   ║\n");
    printf("║ 3- Outros                                    ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcaoso);

    char *clear_cmd = (opcaoso == 1) ? "clear" : "cls";

    while (1) {
        system(clear_cmd);  
        exibir_menu_principal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:{
                char nomeArquivo[100];
                printf("Digite o nome do arquivo: ");
                scanf("%s",&nomeArquivo);
                ler_grafo_de_arquivo(&g, nomeArquivo);
                printf("Grafo gerado com sucesso!");
                pausar();
                break;
            }
            case 2:
                if (g.n_vertices > 0) {
                    imprimir_grafo(&g);
                } else {
                    printf("O grafo não foi carregado ainda. Por favor, carregue-o primeiro.\n");
                }
                pausar();
                break;

            case 3:
                if (g.n_vertices > 0) {
                    printf("Ordem do grafo: %d\n", ordem_grafo(&g));
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 4:
                if (g.n_vertices > 0) {
                    printf("Tamanho do grafo: %d\n", tamanho_grafo(&g));
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 5:
                if (g.n_vertices > 0) {
                    exibir_densidade(&g);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 6:
                if (g.n_vertices > 0) {
                    int vertice_vizinho;
                    printf("Consultar vizinho(s) do vértice: ");
                    scanf("%d", &vertice_vizinho);
                    consultar_vizinhos(&g, vertice_vizinho);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 7:
                if (g.n_vertices > 0) {
                    int vertice_grau;
                    printf("Consultar grau do vértice: ");
                    scanf("%d", &vertice_grau);
                    consultar_grau(&g, vertice_grau);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 8:
                if (g.n_vertices > 0) {
                    int vertice_articulacao;
                    printf("Consultar se o vértice é articulação: ");
                    scanf("%d", &vertice_articulacao);
                    verificar_articulacao(&g, vertice_articulacao);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 9:
                if (g.n_vertices > 0) {
                    int vertice_bfs;
                    printf("Digite o vértice inicial para a busca em largura: ");
                    scanf("%d", &vertice_bfs);
                    bfs(&g, vertice_bfs);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 10:
                if (g.n_vertices > 0) {
                    componentes_conexas(&g);
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 11:
                if (g.n_vertices > 0) {
                    if (possui_ciclo(&g)) {
                        printf("O grafo possui um ciclo.\n");
                    } else {
                        printf("O grafo não possui ciclos.\n");
                    }
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar();
                break;

            case 12:
                if (g.n_vertices > 0) {
                    int vertice_inicial;
                    printf("Digite o número do vértice de origem: ");
                    scanf("%d", &vertice_inicial);
                    if (vertice_inicial < 1 || vertice_inicial > g.n_vertices) {
                        printf("Vértice inválido. O número do vértice deve estar entre 1 e %d.\n", g.n_vertices);
                    } else {
                        bellman_ford(&g, vertice_inicial); 
                    }
                } else {
                    printf("O grafo não foi carregado ainda.\n");
                }
                pausar(); 
                break;
            case 13: 
            {
                int n_vertices, n_arestas, tipo_peso;
                char nome_arquivo[100];

                printf("Digite o número de vértices: ");
                scanf("%d", &n_vertices);
                printf("Digite o número de arestas: ");
                scanf("%d", &n_arestas);
                printf("Escolha o tipo de peso:\n");
                printf("1. Apenas positivo\n");
                printf("2. Apenas negativo\n");
                printf("3. Ambos (positivo e negativo)\n");
                printf("Digite sua opção: ");
                scanf("%d", &tipo_peso);
                printf("Digite o nome do arquivo para salvar o grafo (ex: grafo.txt): ");
                scanf("%s", nome_arquivo);

                gerar_grafo_aleatorio(n_vertices, n_arestas, nome_arquivo, tipo_peso);
                pausar();
            }
            break;

            case 14:
                printf("Saindo do programa. Até logo!\n");
                return;

            default:
                printf("Opção inválida. Tente novamente.\n");
                pausar();
        }
    }
}
