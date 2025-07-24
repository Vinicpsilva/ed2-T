typedef struct grafo Grafo;

Grafo* cria_grafo(int grau_max, int eh_ponderado);
void libera_grafo(Grafo* f);
int insereAresta(Grafo* g, int orig, int dest, int eh_digrafo, float peso);
int removeAresta(Grafo* g, int orig, int destino, int eh_digrafo);
void imprimir_grafo(Grafo* g);
int adicionarVertice(Grafo* g);