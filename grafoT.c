#include <stdlib.h>
#include <stdio.h>
#include "grafoT.h"
#include <time.h>

struct grafo{
    int num_vertices;
    int grau_max;
    int eh_ponderado;
    int** arestas;
    float** pesos;
    int* grau;
};

Grafo* cria_grafo(int grau_max, int eh_ponderado){
    Grafo *g = (Grafo*)malloc(sizeof(Grafo));
    if(g == NULL){
        return NULL;
    }
    g->num_vertices= 0;
    g->grau_max = grau_max;
    g->eh_ponderado = (eh_ponderado != 0)? 1 : 0;

    g->grau = NULL;
    g->arestas = NULL;
    g->pesos = NULL;

    return g;
}
void libera_grafo(Grafo* f){
    if(f == NULL) return;
    for(int i=0; i<f->num_vertices; i++){
        free(f->arestas[i]);
        if(f->eh_ponderado){
            free(f->pesos[i]);
        }
    }
    free(f->arestas);
    if(f->eh_ponderado){
        free(f->pesos);
    }
    free(f->grau);
    free(f);
}

int adicionaVertice(Grafo* g) {
    if (g == NULL) return -1;
    g->num_vertices++;
    
    g->grau = (int*)realloc(g->grau, g->num_vertices * sizeof(int));
    g->grau[g->num_vertices - 1] = 0;

    g->arestas = (int**)realloc(g->arestas, g->num_vertices * sizeof(int*));
    g->arestas[g->num_vertices - 1] = (int*)malloc(g->grau_max * sizeof(int));
    
    if (g->eh_ponderado) {
        g->pesos = (float**)realloc(g->pesos, g->num_vertices * sizeof(float*));
        g->pesos[g->num_vertices - 1] = (float*)malloc(g->grau_max * sizeof(float));
    }
    return 0;
}

int insereAresta(Grafo* g, int orig, int dest, int eh_digrafo, float peso){
    if(g == NULL || orig < 0 || orig >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        return -1; // Erro
    }
    if(g->grau[orig] >= g->grau_max) {
        return -2; // Grau máximo atingido
    }
    // Verifica se a aresta já existe
    for(int i = 0; i < g->grau[orig]; i++) {
        if(g->arestas[orig][i] == dest) {
            printf("Aresta já existe entre %d e %d.\n", orig, dest);
            return -3; // Aresta já existe
        }
    }

    g->arestas[orig] = (int*)realloc(g->arestas[orig], (g->grau[orig] + 1) * sizeof(int));
    if (g->eh_ponderado) {
        g->pesos[orig] = (float*)realloc(g->pesos[orig], (g->grau[orig] + 1) * sizeof(float));
        g->pesos[orig][g->grau[orig]] = peso;
    }
    g->arestas[orig][g->grau[orig]] = dest;
    if(g->eh_ponderado) {
        g->pesos[orig][g->grau[orig]] = peso;
    }
    g->grau[orig]++;
    if(!eh_digrafo) {
        // Só insere a aresta inversa se não for dígrafo
        insereAresta(g, dest, orig, 1, peso);
    }
    return 0; // Sucesso
}

int removeAresta(Grafo* g, int orig, int destino, int eh_digrafo){
    if(g == NULL || orig < 0 || orig >= g->num_vertices || destino < 0 || destino >= g->num_vertices) {
        return -1; // Erro
    }
    int found = 0;
    for(int i = 0; i < g->grau[orig]; i++) {
        if(g->arestas[orig][i] == destino) {
            found = 1;
            for(int j = i; j < g->grau[orig] - 1; j++) {
                g->arestas[orig][j] = g->arestas[orig][j + 1];
                if(g->eh_ponderado) {
                    g->pesos[orig][j] = g->pesos[orig][j + 1];
                }
            }
            g->grau[orig]--;
            break;
        }
    }
    if(!found) return -2; // Aresta não encontrada
    if(!eh_digrafo) {
        removeAresta(g, destino, orig, 1);
    }   
    return 0; // Sucesso
}

void imprimir_grafo(Grafo* g) {
    if (!g) {
        printf("Grafo inexistente.\n");
        return;
    }
    printf("\nMatriz de adjacencia:\n    ");
    for (int i = 0; i < g->num_vertices; i++)
        printf("%4d", i);
    printf("\n");

    for (int i = 0; i < g->num_vertices; i++) {
        printf("%4d", i);
        for (int j = 0; j < g->num_vertices; j++) {
            int valor = 0;
            float peso = 0.0f;

            // Procura se há aresta de i para j
            for (int k = 0; k < g->grau[i]; k++) {
                if (g->arestas[i][k] == j) {
                    valor = 1;
                    if (g->eh_ponderado) {
                        peso = g->pesos[i][k];
                    }
                    break;
                }
            }
            if (g->eh_ponderado) {
                if (valor)
                    printf("%4.1f", peso);
                else
                    printf("%4s", "-");  // Marca ausência de aresta
            } else {
                printf("%4d", valor);
            }
        }
        printf("\n");
    }
}


int main() {
    Grafo* g = cria_grafo(2,0);
    if (g == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return EXIT_FAILURE;
    }
    int arestasmax_grafo = g->num_vertices * g->grau_max;
    int arestas = arestasmax_grafo;
    int inseridas=0;
    int i = 0;
    srand(time(NULL)); 
    while (i<10)
    {
        adicionaVertice(g);
        i++;
    }
    
    insereAresta(g, 0, 0, 1, 0);
    insereAresta(g, 0, 1, 1, 0);
    imprimir_grafo(g);
    libera_grafo(g);
    return EXIT_SUCCESS;
}