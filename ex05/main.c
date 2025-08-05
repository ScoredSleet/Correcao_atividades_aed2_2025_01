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

// Arvore Binaria de Busca
typedef struct ArvoreBB ArvoreBB;

struct ArvoreBB{
    CEP *cep;          // Array de ponteiros para cidades
    int altura;        // Altura da arvore
    ArvoreBB *esq;     // Subarvore a esquerda
    ArvoreBB *dir;     // Subarvore a direita
};


// ========================= PROTOTIPO DAS FUNCOES ===========================

// Funcoes de CEP
CEP *criarCEP(char cep[10], char sigla[3], char *nome_cidade, char *endereco);
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


// Funcoes de arvore

ArvoreBB *criaArvore();
int addElementoArvoreBB(ArvoreBB *root, CEP *cep, int (* compara)(CEP *, CEP *));
int removeElementoArvoreBB(ArvoreBB **root, CEP *cep, int (* compara)(CEP *, CEP *));
CEP *pesquisaElementoArvoreBB(ArvoreBB *root, CEP *cep, int (* compara)(CEP *, CEP *));
void percursoArvore(ArvoreBB *root, void (tipo_percurso)(ArvoreBB *));

void preFixado(ArvoreBB *root);
void central(ArvoreBB *root);
void posFixado(ArvoreBB *root);

CEP *menorElementoArvoreBB(ArvoreBB *root);
CEP *maiorElementoArvoreBB(ArvoreBB *root);

int altura(ArvoreBB *root);
void atualizaAltura(ArvoreBB *root);

void liberaArvoreBB(ArvoreBB *root);

// Ler de arquivo
ArvoreBB *lerArquivoCEP(char *file, int n);

// Funcoes auxiliares
void limpar_buffer();
int max(int a, int b);

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

// ================= Funcoes de arvore ====================

ArvoreBB *criaArvore(){
    ArvoreBB *root = (ArvoreBB *) malloc(sizeof(ArvoreBB));
    root->altura = 0;
    root->dir = NULL;
    root->esq = NULL;
    root->cep = NULL;

    return root;
}

int addElementoArvoreBB(ArvoreBB *root, CEP *cep, int (* compara)(CEP *, CEP *)){
    if(root == NULL)
        return 1;

    if(root->cep == NULL){
        root->cep = cep;
        return 0;
    }

    int cmp = compara(root->cep, cep);
    
    if(cmp > 0){
        if(root->esq == NULL)
            root->esq = criaArvore();
        return addElementoArvoreBB(root->esq, cep, compara);
    }
    else if(cmp < 0){
        if(root->dir == NULL)
            root->dir = criaArvore();
        return addElementoArvoreBB(root->dir, cep, compara);
    }
    else{
        return 1;
    }
    
}

int removeElementoArvoreBB(ArvoreBB **root_ptr, CEP *cep, int (*compara)(CEP *, CEP *)) {
    if(root_ptr == NULL || *root_ptr == NULL || (*root_ptr)->cep == NULL){
        return 1;
    }

    ArvoreBB *pai = NULL;
    ArvoreBB *atual = *root_ptr;

    while(atual != NULL && atual->cep != NULL){
        int cmp = compara(atual->cep, cep);
        if(cmp == 0){
            break;
        }

        pai = atual;
        if(cmp > 0){
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if(atual == NULL || atual->cep == NULL){
        return 1;
    }

    if(atual->esq == NULL || atual->dir == NULL){
        ArvoreBB *filho = (atual->esq != NULL) ? atual->esq : atual->dir;

        if (pai == NULL) {
            *root_ptr = filho;
        }else
        {
            if(pai->esq == atual){
                pai->esq = filho;
            } 
            else{
                pai->dir = filho;
            }
        }
        
        liberarCEP(atual->cep);
        free(atual);
    }
    else{
        ArvoreBB *menor_pai = atual;
        ArvoreBB *menor = atual->dir;
        while(menor->esq != NULL){
            menor_pai = menor;
            menor = menor->esq;
        }

        CEP *tempCep = atual->cep;
        atual->cep = menor->cep;
        menor->cep = tempCep;

        if(menor_pai->esq == menor){
            menor_pai->esq = menor->dir;
        } 
        else{
            menor_pai->dir = menor->dir;
        }

        liberarCEP(menor->cep);
        free(menor);
    }

    return 0;
}

CEP *pesquisaElementoArvoreBB(ArvoreBB *root, CEP *cep, int (* compara)(CEP *, CEP *)){
    if(root == NULL)
        return NULL;
    
    int cmp = compara(root->cep, cep);
    if(cmp > 0)
        return pesquisaElementoArvoreBB(root->esq, cep, compara);
    else if(cmp < 0)
        return pesquisaElementoArvoreBB(root->dir, cep, compara);
    else
        return root->cep;
}
void percursoArvore(ArvoreBB *root, void (tipo_percurso)(ArvoreBB *)){
    tipo_percurso(root);
}

void preFixado(ArvoreBB *root){
    if(root == NULL)
        return;
    
    printCEP(root->cep);
    preFixado(root->esq);
    preFixado(root->dir);
}

void central(ArvoreBB *root){
    if(root == NULL)
        return;
    
    central(root->esq);
    printCEP(root->cep);
    central(root->dir);
}

void posFixado(ArvoreBB *root){
    if(root == NULL)
        return;
    
    posFixado(root->esq);
    posFixado(root->dir);
    printCEP(root->cep);
}

CEP *menorElementoArvoreBB(ArvoreBB *root){
    if(root == NULL)
        return NULL;
    
    if(root->esq == NULL)
        return root->cep;
    
    return menorElementoArvoreBB(root->esq);
}
CEP *maiorElementoArvoreBB(ArvoreBB *root){
    if(root == NULL)
        return NULL;
    
    if(root->dir == NULL)
        return root->cep;
    
    return maiorElementoArvoreBB(root->dir);
}

int altura(ArvoreBB *root){
    if(root == NULL)
        return 0;
    return root->altura;
}

void atualizaAltura(ArvoreBB *root){
    if(root == NULL)
        return;
    
    if(root->dir == NULL && root->esq == NULL)
        root->altura = 0;
    else{
        atualizaAltura(root->esq);
        atualizaAltura(root->dir);
        root->altura = max(altura(root->esq), altura(root->dir)) + 1;
    }
}

void liberaArvoreBB(ArvoreBB *root){
    if(root == NULL)
        return;
    
    liberaArvoreBB(root->dir);
    liberaArvoreBB(root->esq);
    liberarCEP(root->cep);
    free(root);
}


ArvoreBB *lerArquivoCEP(char *nomeArquivo, int n) {
    if (nomeArquivo == NULL) {
        return NULL;
    }

    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    ArvoreBB *root = criaArvore();
    char linha[256];
    int i = 0;

    if(n == 0)
        n = -1;

    while (fgets(linha, sizeof(linha), arq) != NULL && i != n) {
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

        addElementoArvoreBB(root, criarCEP(cep, sigla, nome_cidade, endereco), compara_cep);
        i++;  
    }

    fclose(arq);
    return root;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int max(int a, int b){
    return (a > b) ? a : b;
}

// ========================= MAIN DE TESTE =========================================
int main() {
    int temp;
    printf("\n========= MENU ESCOLHA DOS DADOS =========\n");
    printf("Digite a quantidade de linhas a serem lidas.\n");
    printf("Ps: 0 ou Negativo == Todos os dados\n");
    printf("================================================\n");
    printf("Linhas: ");
    scanf("%d", &temp);
    limpar_buffer();
    
    if(temp < 0)
        temp = 0;
    
    ArvoreBB *root = lerArquivoCEP("../cep.txt", temp);

    int opcao;
    char cep_str[10], sigla_str[3], cidade_str[100], endereco_str[100];

    do {
        printf("\n========= MENU ARVORE BINARIA DE BUSCA =========\n");
        printf("1. Inserir um CEP\n");
        printf("2. Remover um CEP\n");
        printf("3. Visualizar Percursos\n");
        printf("4. Pesquisar um CEP\n");
        printf("5. Mostrar Menor Elemento\n");
        printf("6. Mostrar Maior Elemento\n");
        printf("7. Altura da Arvore\n");
        printf("0. Sair\n");
        printf("================================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer(); // Limpa o buffer de entrada

        CEP cep_temp;
        CEP *resultado_cep = NULL;

        switch (opcao) {
            case 1:
                printf("Digite o CEP (ex: 12345-678): ");
                fgets(cep_str, 10, stdin);
                cep_str[strcspn(cep_str, "\n")] = 0;
                limpar_buffer();

                printf("Digite a Sigla (ex: MG): ");
                fgets(sigla_str, 3, stdin);
                sigla_str[strcspn(sigla_str, "\n")] = 0;
                limpar_buffer();

                printf("Digite o Nome da Cidade: ");
                fgets(cidade_str, 100, stdin);
                cidade_str[strcspn(cidade_str, "\n")] = 0;

                printf("Digite o Endereco: ");
                fgets(endereco_str, 100, stdin);
                endereco_str[strcspn(endereco_str, "\n")] = 0;

                CEP* novo_cep = criarCEP(cep_str, sigla_str, cidade_str, endereco_str);
                if(novo_cep){
                    temp = addElementoArvoreBB(root, novo_cep, compara_cep);
                    if(temp == 0)
                        printf("CEP inserido com sucesso!\n");
                    else
                        printf("CEP duplicado, nao foi inserido!\n");
                }else{
                    printf("Erro: Dados invalidos para criar CEP.\n");
                }
                break;

            case 2:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }
                printf("Digite o CEP a ser removido: ");
                fgets(cep_temp.cep, 10, stdin);
                cep_temp.cep[strcspn(cep_temp.cep, "\n")] = 0;

                temp = removeElementoArvoreBB(&root, &cep_temp, compara_cep);
                
                if(temp == 0)
                    printf("CEP removido com sucesso!\n");
                else
                    printf("CEP nao existe na arvore!\n");
                break;

            case 3:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }
                printf("\n========= PERCURSO =========\n");
                printf("1. Pre-Fixado\n");
                printf("2. Central\n");
                printf("3. Pos-Fixado\n");
                printf("================================================\n");
                printf("Escolha uma opcao: ");
                scanf("%d", &temp);
                limpar_buffer();
                
                switch (temp)
                {
                    case 1:
                        preFixado(root);
                        break;
                    case 2:
                        central(root);
                        break;
                    case 3:
                        posFixado(root);
                        break;
                    default:
                        printf("Opcao invalida! Tente novamente.\n");
                    break;
                }

                break;

            case 4:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }
                printf("Digite o CEP a ser pesquisado: ");
                fgets(cep_temp.cep, 10, stdin);
                cep_temp.cep[strcspn(cep_temp.cep, "\n")] = 0;

                resultado_cep = pesquisaElementoArvoreBB(root, &cep_temp, compara_cep);
                if (resultado_cep) {
                    printf("CEP encontrado: \n");
                    printCEP(resultado_cep);
                } else {
                    printf("CEP nao encontrado na arvore.\n");
                }
                break;

            case 5:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }
                resultado_cep = menorElementoArvoreBB(root);
                printf("Menor elemento da arvore:\n");
                printCEP(resultado_cep);
                break;

            case 6:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia!\n");
                    break;
                }
                resultado_cep = maiorElementoArvoreBB(root);
                printf("Maior elemento da arvore:\n");
                printCEP(resultado_cep);
                break;
            case 7:
                if (root == NULL || root->cep == NULL) {
                    printf("A arvore esta vazia. Altura 0!\n");
                    break;
                }
                atualizaAltura(root);
                printf("Altura da arvore: %d\n", altura(root));
                break;
            case 0:
                printf("Saindo e liberando memoria...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    liberaArvoreBB(root);
    printf("Programa finalizado.\n");

    return 0;
}
