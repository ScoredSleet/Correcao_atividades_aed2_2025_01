#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

    =================== ESTRUTURA DE DADOS USADA PARA A TAREFA =============================
*/

typedef struct {
    char cep[10];
    char sigla[3];
    char *nome_cidade;
    char *endereco;
} CEP;

// Array dinamicamente alocado

typedef struct {
    CEP **ceps;     // Array de ponteiros para cidades
    int size;             // Número atual de elementos no array
    int capacity;         // Capacidade total(elementos alocados possiveis)
} ArrayCEP;


// ========================= PROTOTIPO DAS FUNCOES ===========================

// Funcoes de CEP
CEP *criarCEP(char cep[9], char sigla[3], char *nome_cidade, char *endereco);
char *getCep(CEP *cidade);
char *getSigla(CEP *cidade);
char *getNome_cidade(CEP *cidade);
char *getEndereco(CEP *cidade);

int compara_cep(CEP *a, CEP *b);
int compara_sigla(CEP *a, CEP *b);
int compara_nome_cidade(CEP *a, CEP *b);
int compara_endereco(CEP *a, CEP *b);

void printCEP(CEP *cidade);

void liberarCEP(CEP *cep);

// Array de CEP
ArrayCEP *criarArrayCEP();
void addElementArrayCEP(ArrayCEP *array, CEP *cep);
void removeElementArrayCEP(ArrayCEP *array, char *cep);
void printArray(ArrayCEP *array);
void liberarArrayCEP(ArrayCEP *array);

// Ordenacao
void ordenar(ArrayCEP *array, int (*compara)(CEP*, CEP*), void (*funcaoOrdenacao)(ArrayCEP*, int, int (*compara)(CEP*, CEP*)));

void merge(ArrayCEP *array, int p, int q, int r, int (*compara)(CEP*, CEP*));
void mergeSortAux(ArrayCEP *array, int p, int r, int (*compara)(CEP*, CEP*));
void mergeSort(ArrayCEP *array, int n, int (*compara)(CEP*, CEP*));

// Buscas
int busca_sequencial(ArrayCEP *array, char *cep);
int busca_binaria_aux(ArrayCEP *array, int p, int q, char *cep);
int busca_binaria(ArrayCEP *array, char *cep);

// Ler de arquivo
ArrayCEP *lerArquivoCEP(char *file);

// Funcoes auxiliares
void limpar_buffer();

// ========================= FUNCOES =========================================
// Funcao de criação de cidade;
CEP *criarCEP(char cep[10], char sigla[3], char *nome_cidade, char *endereco){
    CEP *Cep = NULL;

    // Não vou me preocupar caso a string seja contida apenas de caracteres vazios como " ", porém isso deve ser levado em consideração caso venha a fazer um trabalho final ou real.
    if(strlen(cep) != 9 || strlen(sigla) != 2 || strlen(nome_cidade) == 0 || strlen(endereco) == 0)
        return Cep;
    
    Cep = (CEP *)malloc(sizeof(CEP));
    Cep->nome_cidade = (char *)malloc(sizeof(char) * (strlen(nome_cidade) + 1));
    Cep->endereco    = (char *)malloc(sizeof(char) * (strlen(endereco) + 1));

    strcpy(Cep->sigla, sigla);
    strcpy(Cep->cep, cep);
    strcpy(Cep->nome_cidade, nome_cidade);
    strcpy(Cep->endereco, endereco);

    return Cep;
}

char *getCep(CEP *cep){
    if(cep == NULL)
        return NULL;
    return cep->cep;
}

char *getSigla(CEP *cidade){
    if(cidade == NULL)
        return NULL;
    return cidade->sigla;
}

char *getNome_cidade(CEP *cep){
    if(cep == NULL)
        return NULL;
    return cep->nome_cidade;
}

char *getEndereco(CEP *cep){
    if(cep == NULL)
        return NULL;
    return cep->endereco;
}

int compara_cep(CEP *a, CEP *b){
    return strcmp(a->cep, b->cep);
}

int compara_sigla(CEP *a, CEP *b){
    return strcmp(a->sigla, b->sigla);
}
int compara_nome_cidade(CEP *a, CEP *b){
    return strcmp(a->nome_cidade, b->nome_cidade);
}

int compara_endereco(CEP *a, CEP *b){
    return strcmp(a->endereco, b->endereco);
}

void printCEP(CEP *cep) {
    if (cep == NULL) {
        printf("CEP NULO\n");
        return;
    }
    printf("CEP: %s | Cidade: %-20s | Sigla: %s | Endereco: %s\n",
           cep->cep,
           cep->nome_cidade,
           cep->sigla,
           cep->endereco);
}

void liberarCEP(CEP *cep){ 
    if(cep == NULL)
        return;
    
    free(cep->nome_cidade);
    free(cep->endereco);
    free(cep);
    cep = NULL;
}


ArrayCEP *criarArrayCEP(){
    ArrayCEP *array = (ArrayCEP *) malloc(sizeof(ArrayCEP));

    array->ceps = (CEP **)malloc(sizeof(CEP *) * 2);
    array->size = 0;
    array->capacity = 2;

    return array;
}

void addElementArrayCEP(ArrayCEP *array, CEP *cep){
    if(array == NULL)
        return;
    
    if(array->size == array->capacity){
        array->capacity = array->capacity << 1;
        array->ceps = (CEP **)realloc(array->ceps, sizeof(CEP *) * array->capacity);
    }

    array->ceps[array->size] = cep;
    array->size++;
}

void removeElementArrayCEP(ArrayCEP *array, char *cep){
    if(array == NULL || cep == NULL)
        return;

    int index = busca_binaria(array, cep);
    if(index < 0)
        return;
    
    liberarCEP(array->ceps[index]);
    
    array->size--;
    for(int i = index; i < array->size; i++)
        array->ceps[i] = array->ceps[i + 1];
    
}

void printArray(ArrayCEP *array) {
    if (array == NULL || array->size == 0) {
        printf("Array vazio ou nulo.\n");
        return;
    }
    printf("Total de Cidades: %d | Capacidade: %d\n", array->size, array->capacity);
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < array->size; i++) {
        printCEP(array->ceps[i]);
    }
    printf("------------------------------------------------------------------------\n");
}

void ordenar(ArrayCEP *array, int (*compara)(CEP*, CEP*), void (*funcaoOrdenacao)(ArrayCEP*, int, int (*compara)(CEP*, CEP*))){
    if(array == NULL)
        return;
    if(array->capacity == 0)
        return;

    funcaoOrdenacao(array, array->size, compara);
}

void liberarArrayCEP(ArrayCEP *array){
    if(array == NULL)
        return;

    for(int i = 0; i < array->size; i++)
        liberarCEP(array->ceps[i]);
    
    free(array->ceps);
    free(array);
}

void merge(ArrayCEP *array, int p, int q, int r, int (*compara)(CEP*, CEP*)){
    int n1 = q - p + 1;
    int n2 = r - q;

    CEP **L = (CEP **)malloc(n1 * sizeof(CEP*));
    CEP **M = (CEP **)malloc(n2 * sizeof(CEP*));

    for (int i = 0; i < n1; i++)
        L[i] = array->ceps[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = array->ceps[q + 1 + j];

    int i = 0;
    int j = 0;
    int k = p;

    while (i < n1 && j < n2) {
        if (compara(L[i], M[j]) <= 0) {
            array->ceps[k] = L[i];
            i++;
        } else {
            array->ceps[k] = M[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array->ceps[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array->ceps[k] = M[j];
        j++;
        k++;
    }
    
    // Libere a memória dos arrays temporários
    free(L);
    free(M);
}

void mergeSortAux(ArrayCEP *array, int p, int r, int (*compara)(CEP*, CEP*)){
    if(p < r){
        int q = p + (r - p) / 2; 

        mergeSortAux(array, p, q, compara);
        mergeSortAux(array, q + 1, r, compara);
        merge(array, p, q, r, compara);
    }
}

void mergeSort(ArrayCEP *array, int n, int (*compara)(CEP*, CEP*)){
    mergeSortAux(array, 0, n - 1, compara);
}

int busca_sequencial(ArrayCEP *array, char *cep){
    if(array == NULL)
        return -1;
    
    for(int i = 0; i < array->size; i++){
        if(strcmp(array->ceps[i]->cep, cep) == 0)
            return i;
    }

    return -1;
}

int busca_binaria_aux(ArrayCEP *array, int p, int q, char *cep){
    if(p > q)
        return -1;
    
    int middle = p + (q - p) / 2;
    if(strcmp(array->ceps[middle]->cep, cep) == 0)
        return middle;
    if(strcmp(array->ceps[middle]->cep, cep) < 0)
        return busca_binaria_aux(array, middle + 1, q, cep);
    return busca_binaria_aux(array, p, middle - 1, cep);
}

int busca_binaria(ArrayCEP *array, char *cep){
    if(array == NULL)
        return -1;

    return busca_binaria_aux(array, 0, array->size - 1, cep);
}

ArrayCEP *lerArquivoCEP(char *nomeArquivo) {
    if (nomeArquivo == NULL) {
        return NULL;
    }

    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    ArrayCEP *array = criarArrayCEP();
    char linha[256];

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\n")] = 0;

        char *token;
        char cep[9], sigla[3], nome_cidade[100], endereco[100];

        token = strtok(linha, ";");
        if (token == NULL) continue;
        strcpy(cep, token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(sigla, token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(nome_cidade, token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strcpy(endereco, token);

        addElementArrayCEP(array, criarCEP(cep, sigla, nome_cidade, endereco));
        
    }

    fclose(arq);
    return array;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ========================= MAIN DE TESTE =========================================
int main() {
    ArrayCEP *meuArray = NULL;
    char nomeArquivo[20] = "../cep.txt";
    int opcao = 0;

    printf("===== GERENCIADOR DE CEPs =====\n");

    printf("\nCarregando dados do arquivo %s...\n", nomeArquivo);
    meuArray = lerArquivoCEP(nomeArquivo);

    if (meuArray == NULL) {
        printf("Nao foi possivel carregar o array. Encerrando o programa.\n");
        return 1;
    }

    printf("Dados carregados. Ordenando por CEP para otimizar buscas...\n");
    ordenar(meuArray, compara_cep, mergeSort);

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Pesquisar um CEP\n");
        printf("2. Adicionar um CEP\n");
        printf("3. Remover um CEP\n");
        printf("4. Imprimir todos os CEPs\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: { // Pesquisar por CEP
                int opcao_busca = 1;
                do
                {
                    printf("\nEscolha qual o metodo de busca a ser utilizado: ");
                    printf("\n1 - Busca Binaria");
                    printf("\n2 - Busca Sequencial");
                    printf("\nEscolha uma opcao: ");
                    scanf(" %d", &opcao_busca);
                } while (opcao_busca > 2 || opcao_busca < 1);
                
                char cep_busca[10];
                printf("\nDigite o CEP a ser pesquisado (8 digitos com traco): ");
                scanf("%9s", cep_busca);
                limpar_buffer();

                int indice;
                if(opcao_busca == 1)
                    indice = busca_binaria(meuArray, cep_busca);
                else
                    indice = busca_sequencial(meuArray, cep_busca);
                
                if (indice != -1) {
                    printf("\nCEP encontrado:\n");
                    printCEP(meuArray->ceps[indice]);
                } else {
                    printf("\nCEP %s nao encontrado.\n", cep_busca);
                }
                break;
            }
            case 2: { // Adicionar CEP
                char cep[10], sigla[3], nome_cidade[100], endereco[100];
                printf("\n--- Adicionar Novo CEP ---\n");
                printf("Digite o CEP (8 digitos com traco): ");
                scanf("%8s", cep);
                limpar_buffer();

                printf("Digite a Sigla do Estado (2 caracteres): ");
                scanf("%2s", sigla);
                limpar_buffer();

                printf("Digite o Nome da Cidade: ");
                fgets(nome_cidade, 100, stdin);
                nome_cidade[strcspn(nome_cidade, "\n")] = 0;

                printf("Digite o Endereco: ");
                fgets(endereco, 100, stdin);
                endereco[strcspn(endereco, "\n")] = 0;

                CEP* novo_cep = criarCEP(cep, sigla, nome_cidade, endereco);
                if (novo_cep != NULL) {
                    addElementArrayCEP(meuArray, novo_cep);
                    
                    // Ordenacao para manter os dados consistentes(Sim eu sei que é custoso, e é O(nlogn) para conseguir fazer a cada inserção, o melhor seria ter que colocar no inicio da pesquisa, antes de entrar nas escolhas)
                    ordenar(meuArray, compara_cep, mergeSort);
                    printf("CEP adicionado e array reordenado com sucesso!\n");
                } else {
                    printf("Erro: Dados invalidos. O CEP nao foi adicionado.\n");
                }
                break;
            }
            case 3: { // Remover CEP
                char cep_remover[10];
                printf("\nDigite o CEP a ser removido (8 digitos com traco): ");
                scanf("%8s", cep_remover);
                limpar_buffer();
                
                ordenar(meuArray, compara_cep, mergeSort);
                removeElementArrayCEP(meuArray, cep_remover);
                break;
            }
            case 4: { // Imprimir todos
                printf("\n--- LISTA DE TODOS OS CEPs ---\n");
                printArray(meuArray);
                break;
            }
            case 5: { // Sair
                printf("\nEncerrando o programa...\n");
                break;
            }
            default: {
                printf("\nOpcao invalida. Por favor, tente novamente.\n");
                break;
            }
        }

    } while (opcao != 5);

    // Liberar toda a memória alocada
    printf("Liberando toda a memoria alocada...\n");
    liberarArrayCEP(meuArray);

    printf("\n===== PROGRAMA FINALIZADO =====\n");
    return 0;
}
