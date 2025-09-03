#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

    =================== ESTRUTURA DE DADOS USADA PARA A TAREFA =============================
*/

// Produto
typedef struct {
    char tipo;
    char *descricao;
} Produto;

// Definicao da minha tabela hash
typedef struct {
    int len;
    int quant_atual;
    Produto **produtos;
} TabelaHash;

const char TIPOS_DISPONIVEIS[] = {'A', 'H', 'L', 'V'};
const int  QUANT_TIPOS = 4;

// ========================= PROTOTIPO DAS FUNCOES ===========================

Produto *criaProduto(char tipo, char *descricao);
int verificaTipo(char tipo);
int printProduto(Produto *prod);
void destroiProduto(Produto *prod);

TabelaHash *criaTabelaHash(int len);
int funcaoHash(TabelaHash *tabela, char tipo);
int insereTabelaHash(TabelaHash *tabela, Produto *prod);
void destroiTabelaHash(TabelaHash *tabela);

void consultaProdutosPorTipo(TabelaHash *tabela, char tipo);
void contaProdutosPorTipo(TabelaHash *tabela);
void limparBuffer();


// ========================= FUNCOES =========================================

Produto *criaProduto(char tipo, char *descricao){
    if(descricao == NULL || strlen(descricao) == 0 || verificaTipo(tipo) == 1)
        return NULL;

    Produto *prod = (Produto *)malloc(sizeof(Produto));
    if (prod == NULL) return NULL;

    prod->tipo = tipo;
    prod->descricao = (char *)malloc(strlen(descricao) + 1);
    if (prod->descricao == NULL) {
        free(prod);
        return NULL;
    }
    strcpy(prod->descricao, descricao);

    return prod;
}

int verificaTipo(char tipo){
    for(int i = 0; i < QUANT_TIPOS; i++){
        if(tipo == TIPOS_DISPONIVEIS[i])
            return 0;
    }
    return 1;
}

int printProduto(Produto *prod){
    if(prod == NULL)
        return -1;

    printf("  - Tipo: %c, Descricao: %s\n", prod->tipo, prod->descricao);

    return 0;
}

void destroiProduto(Produto *prod){
    if(prod != NULL){
        free(prod->descricao);
        free(prod);
    }

    prod = NULL;
}

TabelaHash *criaTabelaHash(int len){
    if(len <= 0)
        return NULL;

    TabelaHash *tabela = (TabelaHash *)malloc(sizeof(TabelaHash));
    if (tabela == NULL) return NULL;

    tabela->len = len;
    tabela->quant_atual = 0;
    tabela->produtos = (Produto **)malloc(sizeof(Produto *) * len);
    if (tabela->produtos == NULL) {
        free(tabela);
        return NULL;
    }
    
    // Inicializando cada posicao com NULL
    for(int i = 0; i < len; i++)
        tabela->produtos[i] = NULL;
    
    return tabela;
}


int funcaoHash(TabelaHash *tabela, char tipo){
    if(tabela == NULL || verificaTipo(tipo) != 0)
        return -1;
    
    return ((int)tipo) % tabela->len;
}

int insereTabelaHash(TabelaHash *tabela, Produto *prod){
    if(tabela == NULL || prod == NULL)
        return -1;
    
    if (tabela->quant_atual >= tabela->len) {
        return -2;
    }
    
    int pos = funcaoHash(tabela, prod->tipo);
    
    while (tabela->produtos[pos] != NULL) {
        pos = (pos + 1) % tabela->len;
    }

    tabela->produtos[pos] = prod;
    tabela->quant_atual++;
    return pos;
}

void destroiTabelaHash(TabelaHash *tabela){
    if(tabela == NULL)
        return;
    
    for(int i = 0; i < tabela->len; i++){
        if (tabela->produtos[i] != NULL) {
            destroiProduto(tabela->produtos[i]);
        }
    }

    free(tabela->produtos);
    free(tabela);

    tabela->produtos = NULL;
    tabela           = NULL;
}

// ========================= MAIN DE TESTE =========================================
void consultaProdutosPorTipo(TabelaHash *tabela, char tipo) {
    if(tabela == NULL) return;
    
    printf("\n--- Produtos do Tipo '%c' ---\n", tipo);
    int encontrados = 0;
    for (int i = 0; i < tabela->len; i++) {
        if (tabela->produtos[i] != NULL && tabela->produtos[i]->tipo == tipo) {
            printProduto(tabela->produtos[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum produto encontrado para este tipo.\n");
    }
}

void contaProdutosPorTipo(TabelaHash *tabela) {
    if (tabela == NULL) return;

    int contadores[QUANT_TIPOS];
    for(int i = 0; i < QUANT_TIPOS; i++)
        contadores[i] = 0;

    for(int i = 0; i < tabela->len; i++){
        if (tabela->produtos[i] != NULL) {
            switch(tabela->produtos[i]->tipo) {
                case 'A': contadores[0]++; break;
                case 'H': contadores[1]++; break;
                case 'L': contadores[2]++; break;
                case 'V': contadores[3]++; break;
            }
        }
    }

    printf("\n--- Quantidade de Produtos por Tipo ---\n");
    printf("Alimentacao - %d produto(s)\n", contadores[0]);
    printf("Higiene     - %d produto(s)\n", contadores[1]);
    printf("Limpeza     - %d produto(s)\n", contadores[2]);
    printf("Vestuario   - %d produto(s)\n", contadores[3]);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    TabelaHash *tabela = criaTabelaHash(50);
    int opcao;
    char descricao[100];
    char tipo;

    if (tabela == NULL) {
        printf("Erro ao alocar memoria para a tabela hash.\n");
        return 1;
    }

    do {
        printf("\n========= MENU =========\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Consultar produtos por tipo\n");
        printf("3 - Contar produtos por tipo\n");
        printf("4 - Sair\n");
        printf("========================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                printf("Digite o tipo do produto (A, H, L, V): ");
                scanf(" %c", &tipo);
                limparBuffer();

                if (verificaTipo(tipo) != 0) {
                    printf("Erro: Tipo de produto invalido!\n");
                    break;
                }

                printf("Digite a descricao do produto: ");
                fgets(descricao, 100, stdin);
                descricao[strcspn(descricao, "\n")] = 0;

                Produto *p = criaProduto(tipo, descricao);
                if (p == NULL) {
                    printf("Erro ao criar o produto.\n");
                    break;
                }

                int res = insereTabelaHash(tabela, p);
                if (res == -2) {
                    printf("Nao foi possivel fazer a insercao. A tabela esta cheia.\n");
                    destroiProduto(p);
                } else if (res == -1) {
                    printf("Erro desconhecido ao inserir na tabela.\n");
                    destroiProduto(p);
                } else {
                    printf("Produto '%s' inserido com sucesso!\n", descricao);
                }
                break;

            case 2:
                printf("Digite o tipo a ser consultado (A, H, L, V): ");
                scanf(" %c", &tipo);
                limparBuffer();

                if (verificaTipo(tipo) != 0) {
                    printf("Erro: Tipo de produto invalido!\n");
                    break;
                }
                consultaProdutosPorTipo(tabela, tipo);
                break;

            case 3:
                contaProdutosPorTipo(tabela);
                break;

            case 4:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 4);

    destroiTabelaHash(tabela);
    return 0;
}
