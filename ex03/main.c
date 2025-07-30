/*
    Codigo desenvolvido por Matheus Maximo gitbub: @ScoredSleet

    Pode ser usado a vontade e feito de referencias para outras atividades
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//======================= Estrutura de um Grafo ===============

typedef char* vertice;
typedef double peso;

typedef struct no No;
typedef struct lista_adj ListaAdj;

typedef struct {
    int num_arestas;
    int num_vertices;
    int num_vertices_alocados;
    ListaAdj **list;
} Grafo;

struct lista_adj {
    vertice v;
    int num_arestas_conectadas;
    No* init;
    No* fim;
};

struct no {
    vertice v;
    peso p;
    No *prox;
};

typedef struct {
    vertice* items;
    int inicio;
    int fim;
    int capacidade;
} Fila;

typedef int (*ComparaFunc)(const void *a, const void *b);

int compara(const void *a, const void *b, ComparaFunc funcao_comparacao){
    return funcao_comparacao(a, b);
}

// =========== Funcoes principais para filas ================

Fila *criaFila(int capacidade);
int filaVazia(Fila *q);
void addFila(Fila *q, vertice v);
vertice removeFila(Fila *q);
void liberaFila(Fila* q);

// =========== Funcoes principais para grafos ================

vertice criaVertice(char *nome);
void liberaVertice(vertice v);
Grafo *criaGrafo();
int addVertice(Grafo *graph, vertice v);
int removeVertice(Grafo *g, vertice v);
int compara_vertice(const void *v1, const void *v2);

int addAresta(Grafo *graph, vertice v1, vertice v2, peso p);
int removeAresta(Grafo *g, vertice v1, vertice v2);

int foiVisitado(vertice v, vertice* visitados, int count);
ListaAdj* encontraListaAdj(Grafo* g, vertice v);
int bfs(Grafo *g, vertice inicio);
int detectaCicloUtil(Grafo* g, vertice u, vertice* visitados, int* visitados_count, vertice pai);
int temCiclo(Grafo* g);
int ehConexo(Grafo* g);

void limpaGrafo(Grafo **graph);
void imprimeGrafo(Grafo *graph);

// =========== Funcoes principais para listas ================
int addNo(ListaAdj *list, vertice v, peso p);
int removerNo(ListaAdj *list, vertice v);

void limpaNo(No *no);
void limpaListaAdj(ListaAdj **list_ptr);

// =========== Funcoes Auxiliares ============================

//  Funcao retira de https://prt.linux-console.net/?p=17444
char* strtrim(char* str) {

    size_t len = strlen(str);
    if (len == 0) {
      return str;

    }

    size_t start = 0;
    while (isspace(str[start])) {

      start++;

    }
    size_t end = len - 1;
    while (isspace(str[end])) {

      end--;

    }
    size_t i;
    for (i = 0; i <= end - start; i++) {
      str[i] = str[start + i];
    }

    str[i] = '\0';
    return str;

}

///////////////////////////

// Funcoes de Fila para BFS


Fila *criaFila(int capacidade){
    Fila* q = (Fila*)malloc(sizeof(Fila));

    if (q == NULL) return NULL;

    q->items = (vertice *)malloc(capacidade * sizeof(vertice));
    if (q->items == NULL) {
        free(q);
        return NULL;
    }
    q->inicio = -1;
    q->fim    = -1;
    q->capacidade = capacidade;
    return q;
}

int filaVazia(Fila *q){
    if(q == NULL)
        return -1;
    return q->fim == -1;
}

void addFila(Fila *q, vertice v){
    if(q == NULL)
        return;

    if(q->fim == q->capacidade - 1) {
        printf("Erro: Fila cheia!\n");
        return;
    }

    if(q->inicio = -1)
        q->inicio = 0;
    
    q->fim++;
    q->items[q->fim] = v;
}

vertice removeFila(Fila *q){
    if (filaVazia(q)) {
        return NULL;
    }

    vertice item = q->items[q->inicio];
    q->inicio++;
    if (q->inicio > q->fim) {
        q->inicio = -1;
        q->fim    = -1;
    }

    return item;
}

void liberaFila(Fila* q) {
    if(q == NULL) return;
    free(q->items);
    free(q);
}

// Funcoes de grafos

vertice criaVertice(char *nome){
    vertice v = (char *) malloc(sizeof(char) * strlen(nome) + 1);
    strcpy(v, nome);
    v = strtrim(v);
    return v;
}

void liberaVertice(vertice v){
    if(v == NULL)
        return;
    
    free(v);
}

Grafo *criaGrafo() {
    Grafo *graph = (Grafo *) malloc(sizeof(Grafo));
    if (graph == NULL) return NULL;

    graph->num_vertices = 0;
    graph->num_arestas = 0;
    graph->num_vertices_alocados = 5;

    graph->list = (ListaAdj **) malloc(graph->num_vertices_alocados * sizeof(ListaAdj*));
    if (graph->list == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}

int addVertice(Grafo *graph, vertice v) {
    if (graph == NULL) return 1;

    for (int i = 0; i < graph->num_vertices; i++) {
        if (compara(v, graph->list[i]->v, compara_vertice) == 0) return 1;
    }

    if (graph->num_vertices == graph->num_vertices_alocados) {
        graph->num_vertices_alocados += 5;
        ListaAdj **temp_list = realloc(graph->list, graph->num_vertices_alocados * sizeof(ListaAdj*));
        if (temp_list == NULL) return 1;
        graph->list = temp_list;
    }

    graph->list[graph->num_vertices] = (ListaAdj *) malloc(sizeof(ListaAdj));
    if (graph->list[graph->num_vertices] == NULL) return 1;

    graph->list[graph->num_vertices]->v = v;
    graph->list[graph->num_vertices]->init = NULL;
    graph->list[graph->num_vertices]->fim = NULL;
    graph->list[graph->num_vertices]->num_arestas_conectadas = 0;

    graph->num_vertices++;
    return 0;
}

int removeVertice(Grafo *g, vertice v) {
    if (g == NULL || g->num_vertices == 0) return 1;

    int localizacao_vertice = -1;
    for (int i = 0; i < g->num_vertices; i++) {
        if (compara(v, g->list[i]->v, compara_vertice) == 0) {
            localizacao_vertice = i;
            break;
        }
    }

    if (localizacao_vertice == -1) {
        return 1;
    }

    ListaAdj *lista_removida = g->list[localizacao_vertice];
    g->num_arestas -= lista_removida->num_arestas_conectadas;

    for (int i = localizacao_vertice; i < g->num_vertices - 1; i++) {
        g->list[i] = g->list[i + 1];
    }

    g->num_vertices--;

    limpaListaAdj(&lista_removida);

    for (int i = 0; i < g->num_vertices; i++) {
        removerNo(g->list[i], v);
    }
    
    if (g->num_vertices_alocados - g->num_vertices > 5) {
        g->num_vertices_alocados = (g->num_vertices / 5 + 1) * 5;
        if (g->num_vertices_alocados == 0) g->num_vertices_alocados = 5;
        
        ListaAdj **temp_list = realloc(g->list, g->num_vertices_alocados * sizeof(ListaAdj*));
        if(temp_list != NULL || g->num_vertices == 0) {
             g->list = temp_list;
        }
    }

    return 0;
}

int compara_vertice(const void *v1, const void *v2){
    return strcmp((char *) v1, (char *) v2);
}


int addAresta(Grafo *g, vertice v1, vertice v2, peso p) {
    if (g == NULL || g->list == NULL) return 1;

    int loca_v1 = -1, loca_v2 = -1;
    for (int i = 0; i < g->num_vertices; i++) {
        if (compara(g->list[i]->v, v1, compara_vertice) == 0) loca_v1 = i;
        if (compara(g->list[i]->v, v2, compara_vertice) == 0) loca_v2 = i;
    }

    if (loca_v1 == -1 || loca_v2 == -1) return 1;

    addNo(g->list[loca_v1], v2, p);
    addNo(g->list[loca_v2], v1, p);

    g->num_arestas++;
    return 0;
}

int removeAresta(Grafo *g, vertice v1, vertice v2){
    if(g == NULL)
        return 1;
    
    for(int i = 0, j = 0; i < g->num_vertices && j != 2; i++){
        if(compara(g->list[i]->v, v1, compara_vertice) == 0){
            j++;
            removerNo(g->list[i], v2);
        }
        if(compara(g->list[i]->v, v2, compara_vertice) == 0){
            j++;
            removerNo(g->list[i], v1);
        }
    }

    return 0;
}

int foiVisitado(vertice v, vertice* visitados, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(v, visitados[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

ListaAdj* encontraListaAdj(Grafo* g, vertice v) {
    if (g == NULL) return NULL;

    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->list[i]->v, v) == 0) {
            return g->list[i];
        }
    }
    return NULL;
}

int bfs(Grafo *g, vertice inicio) {
    if (g == NULL || encontraListaAdj(g, inicio) == NULL) {
        printf("Grafo ou vertice de inicio invalido para BFS.\n");
        return -1;
    }

    // Fila para o BFS, com capacidade igual ao número de vértices
    Fila* q = criaFila(g->num_vertices);

    // Array para marcar os vertices visitados
    vertice* visitados = (vertice*)malloc(g->num_vertices * sizeof(vertice));
    int visitados_count = 0;

    // Começa pelo nó inicial: enfileira e marca como visitado
    addFila(q, inicio);
    visitados[visitados_count++] = inicio;

    // Loop principal: continua enquanto a fila não estiver vazia
    while (!filaVazia(q)) {
        // Pega o próximo vértice da fila
        vertice atual_v = removeFila(q);

        // Encontra sua lista de adjacência
        ListaAdj* adjList = encontraListaAdj(g, atual_v);
        if(adjList == NULL) continue; // Segurança, caso o vértice não exista

        // Percorre todos os seus vizinhos
        No* vizinho = adjList->init;
        while (vizinho != NULL) {
            // Se o vizinho ainda não foi visitado...
            if (!foiVisitado(vizinho->v, visitados, visitados_count)) {
                // ...marca como visitado e o adiciona na fila
                visitados[visitados_count++] = vizinho->v;
                addFila(q, vizinho->v);
            }
            vizinho = vizinho->prox;
        }
    }

    // Liberar memória utilizada
    free(visitados);
    liberaFila(q);

    return visitados_count; // Faz a verificação se o grafo é conexo se o valor de visitados for diferente da quantidade de vertices
}

int detectaCicloUtil(Grafo* g, vertice u, vertice* visitados, int* visitados_count, vertice pai) {
    // Marca o nó atual como visitado
    visitados[*visitados_count] = u;
    (*visitados_count)++;

    // Encontra a lista de adjacência para o vértice 'u'
    ListaAdj* adjList = encontraListaAdj(g, u);
    if (adjList == NULL) return 0; // Vértice sem vizinhos

    // Percorre todos os vizinhos
    No* v_no = adjList->init;
    while (v_no != NULL) {
        vertice v = v_no->v;

        // Se o vizinho 'v' não foi visitado, chama a recursão para ele
        if (!foiVisitado(v, visitados, *visitados_count)) {
            if (detectaCicloUtil(g, v, visitados, visitados_count, u)) {
                return 1; // Ciclo encontrado na chamada recursiva
            }
        }
        else if (strcmp(v, pai) != 0) {
            return 1; // Ciclo encontrado!
        }

        v_no = v_no->prox;
    }
    return 0; // Nenhum ciclo encontrado a partir deste vértice
}


// Função principal para detectar ciclos em um grafo
int temCiclo(Grafo* g) {
    if (g == NULL || g->num_vertices == 0) {
        return 0;
    }

    // Array para marcar os vertices visitados
    vertice* visitados = (vertice*)malloc(g->num_vertices * sizeof(vertice));
    int visitados_count = 0;

    // Itera por todos os vértices para garantir que grafos desconexos sejam tratados
    for (int i = 0; i < g->num_vertices; i++) {
        vertice v = g->list[i]->v;
        if (!foiVisitado(v, visitados, visitados_count)) {
            // Chama a função auxiliar recursiva.
            // O pai do primeiro nó é NULO, pois não veio de ninguém.
            if (detectaCicloUtil(g, v, visitados, &visitados_count, NULL)) {
                free(visitados);
                return 1; // Encontrou um ciclo
            }
        }
    }

    free(visitados);
    return 0; // Nenhum ciclo no grafo
}

int ehConexo(Grafo *g){
    if(g == NULL)
        return 0;
    if(g->num_vertices <= 1 || g->num_vertices == bfs(g, g->list[0]->v))
        return 1;
    
    return 0;
}

void limpaGrafo(Grafo **graph_ptr) {
    Grafo *graph = *graph_ptr;
    if (graph == NULL) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        limpaListaAdj(&(graph->list[i]));
    }
    
    free(graph->list);
    free(graph);

    *graph_ptr = NULL;
}

void imprimeGrafo(Grafo *graph) {
    if (graph == NULL) {
        printf("Grafo eh NULO (memoria liberada).\n");
        return;
    }

    printf("================================\n");
    printf("GRAFO (Vertices: %d, Arestas: %d)\n", graph->num_vertices, graph->num_arestas);
    
    if(graph->num_vertices == 0){
        printf("Grafo Vazio.\n");
        printf("--------------------------------\n");
        return;
    }
    printf("--------------------------------\n");

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%s: ", graph->list[i]->v);
        No* no_atual = graph->list[i]->init;
        if (no_atual == NULL) {
            printf("Nenhum vizinho.\n");
        } else {
            while (no_atual != NULL) {
                printf("-> (%s, peso: %.2f) ", no_atual->v, no_atual->p);
                no_atual = no_atual->prox;
            }
            printf("\n");
        }
    }
    printf("================================\n\n");
}

// Funcoes da Lista de Adjacencia

int addNo(ListaAdj *list, vertice v, peso p) {
    if (list == NULL) return 1;

    No* no = (No *) malloc(sizeof(No));
    if (no == NULL) return 1;

    no->v = v;
    no->p = p;
    no->prox = NULL;

    if (list->init == NULL) {
        list->init = no;
        list->fim = no;
    } else {
        list->fim->prox = no;
        list->fim = no;
    }

    list->num_arestas_conectadas++;
    return 0;
}

int removerNo(ListaAdj *list, vertice v) {
    if (list == NULL || list->init == NULL) {
        return 1; // 
    }

    No *atual = list->init;
    No *anterior = NULL;

    while (atual != NULL && compara(atual->v, v, compara_vertice) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        return 1; 
    }

    if (anterior == NULL) {
        list->init = atual->prox;
    } 
    else {
        anterior->prox = atual->prox;
    }

    if (atual == list->fim) {
        list->fim = anterior;
    }

    free(atual);
    list->num_arestas_conectadas--;
    
    return 0; 
}

void limpaNo(No *no) {
    if (no == NULL) return;
    limpaNo(no->prox);
    free(no);
}

void limpaListaAdj(ListaAdj **list_ptr) {
    ListaAdj *list = *list_ptr;
    if (list == NULL) return;

    limpaNo(list->init);
    free(list);
    *list_ptr = NULL;
}

//============================================================

void menu() {
    printf("=========== MENU ===========\n");
    printf("1. Adicionar Vertice\n");
    printf("2. Adicionar Aresta\n");
    printf("3. Imprimir Grafo\n");
    printf("4. Verificar Ciclo\n");
    printf("5. Verificar se e Conexo\n");
    printf("6. Reiniciar Grafo\n");
    printf("7. Sair\n");
    printf("============================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Grafo* meu_grafo = criaGrafo();
    int opcao;
    char cidade_01[100], cidade_02[100];

    vertice v;
    peso p;

    while (1) {
        menu();
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            opcao = -1; 
        }

        switch (opcao) {
            case 1:
                printf("Digite o nome da Cidade: ");
                scanf("%99s", cidade_01);

                v = criaVertice(cidade_01);

                if (addVertice(meu_grafo, v) == 0){
                    printf("Cidade '%s' adicionada com sucesso!\n", v);
                }
                else{
                    printf("Falha ao adicionar Cidade '%s'. Pode ja existir.\n", v);
                    liberaVertice(v);
                }

                break;
            case 2:
                printf("Digite o nome da primeira Cidade: ");
                scanf("%99s", cidade_01);

                printf("Digite o nome da segunda Cidade: ");
                scanf("%99s", cidade_02);

                printf("Digite a distancia entre as cidades: ");
                scanf("%lf", &p);


                if (addAresta(meu_grafo, criaVertice(cidade_01), criaVertice(cidade_02), p) == 0){
                    printf("Rodovia entre '%s' e '%s' adicionada com sucesso!\n", cidade_01, cidade_02);
                }
                else{
                    printf("Falha ao adicionar rodovia. Verifique se as cidades existem.\n");
                }
                break;
            case 3:
                imprimeGrafo(meu_grafo);
                break;
            case 4:
                if (temCiclo(meu_grafo)){
                    printf("RESULTADO: O grafo CONTEM pelo menos um ciclo.\n");
                }
                else{
                    printf("RESULTADO: O grafo NAO contem ciclos.\n");
                }

                break;
            case 5:
                if(ehConexo(meu_grafo)){
                    printf("RESULTADO: O grafo E conexo.\n");
                }
                else{
                    printf("RESULTADO: O grafo NAO E conexo.\n");
                }

                break;
            case 6:
                limpaGrafo(&meu_grafo);
                meu_grafo = criaGrafo();
                printf("Grafo reiniciado com sucesso!\n");
                break;
            case 7:
                limpaGrafo(&meu_grafo);
                printf("Saindo do programa...\n");
                return 0;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
        printf("\n");
    }

    return 0;
}