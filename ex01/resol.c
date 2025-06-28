#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==================== Modelo Funcionario com construtor ====================//

struct funcionario
{
    char *nome;
    char *naturalidade;
    int idade;
    double salario;
};

typedef struct funcionario Funcionario;

Funcionario *criaFuncionario(char* nome, int idade, char *naturalidade, double salario){
    Funcionario *func = (Funcionario *)malloc(sizeof(Funcionario));

    // Aloca e copia a string para nome
    func->nome = (char *)malloc(strlen(nome) + 1);
    strcpy(func->nome, nome);

    func->idade        = idade;
    func->salario      = salario;

    // Aloca e copia a string para naturalidade
    func->naturalidade = (char *)malloc(strlen(naturalidade) + 1);
    strcpy(func->naturalidade, naturalidade);

    return func;
}

void limpaFuncionarios(Funcionario **ArrayFuncionario, int n){
    for(int i = 0; i < n; i++){
        free(ArrayFuncionario[i]->nome);         // Libera a memória de nome
        free(ArrayFuncionario[i]->naturalidade); // Libera a memória de naturalidade
        free(ArrayFuncionario[i]);
    }
    free(ArrayFuncionario);

    ArrayFuncionario = NULL;
}

Funcionario **lerFuncionarios(char *arq, int *tamanhoArray){
    FILE *file = fopen(arq, "r");

    if(file == NULL){
        printf("Arquivo inexistente!\n");
        exit(EXIT_FAILURE);
    }

    char nome[256];
    int idade;
    char naturalidade[256];
    double salario;

    int count = 0;
    *tamanhoArray = 10;
    Funcionario **ArrayFuncionario = (Funcionario **)malloc(sizeof(Funcionario *) * 10);

    while(fscanf(file, "%s %d %s %lf\n", nome, &idade, naturalidade, &salario) > 0){
        if(*tamanhoArray == count){
            *tamanhoArray += 10;
            ArrayFuncionario = realloc((Funcionario **)ArrayFuncionario, sizeof(Funcionario) * (*tamanhoArray));
        }
        ArrayFuncionario[count] = criaFuncionario(nome, idade, naturalidade, salario);
        count++;
    }

    fclose(file);
    *tamanhoArray = count;
    return ArrayFuncionario;
}

int compararFuncionarioSalario(Funcionario *f1, Funcionario *f2){
    if(f1->salario > f2->salario)
        return 1;
    else if(f1->salario < f2->salario)
        return -1;
    else
        return 0;
}

int compararFuncionarioIdadeDec(Funcionario *f1, Funcionario *f2){
    if(f1->idade > f2->idade)
        return -1;
    else if(f1->idade < f2->idade)
        return 1;
    else
        return 0;
}

int compararFuncionarioNome(Funcionario *f1, Funcionario *f2){
    return strcmp(f1->nome, f2->nome);
}

void printFuncionarios(Funcionario **ArrayFuncionario, int n){
    if (ArrayFuncionario == NULL || n <= 0) {
        printf("Nenhum funcionario para exibir.\n");
        return;
    }

    printf("\n%-5s | %-20s | %-10s | %-20s | %-15s\n", "ID", "Nome", "Idade", "Naturalidade", "Salario");
    printf("--------------------------------------------------------------------------------------\n");
    for(int i = 0; i < n; i++){
        if (ArrayFuncionario[i] != NULL) {
            printf("%-5d | %-20s | %-10d | %-20s | R$ %-12.2lf\n",
                   i + 1,
                   ArrayFuncionario[i]->nome,
                   ArrayFuncionario[i]->idade,
                   ArrayFuncionario[i]->naturalidade,
                   ArrayFuncionario[i]->salario);
        }
    }
    printf("--------------------------------------------------------------------------------------\n");
}

//======================================================================//

//==================== Algoritmo do Bubble Sort ====================//

void bubbleSort(Funcionario **ArrayFuncionario, int n, int (*comparar)(Funcionario*, Funcionario*)){

    Funcionario* temp;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - 1 - i; j++){
            if(comparar(ArrayFuncionario[j], ArrayFuncionario[j + 1]) > 0){
                temp = ArrayFuncionario[j];
                ArrayFuncionario[j]     = ArrayFuncionario[j + 1];
                ArrayFuncionario[j + 1] = temp;
            }
        }
    }
}



//======================================================================//

//==================== Algoritmo do Selection Sort ====================//

void selectionSort(Funcionario **ArrayFuncionario, int n, int (*comparar)(Funcionario*, Funcionario*)){
    Funcionario *temp;
    int flag, menor;
    for(int i = 0; i < n - 1; i++){
        flag = 0;
        menor = i;

        for(int j = i + 1; j < n; j++){
            if(comparar(ArrayFuncionario[j], ArrayFuncionario[menor]) < 0){
                flag = 1;
                menor = j;
            }
        }
        if(flag == 1){
            temp                    = ArrayFuncionario[i];
            ArrayFuncionario[i]     = ArrayFuncionario[menor];
            ArrayFuncionario[menor] = temp;
        }  
    }
}


//======================================================================//

//==================== Menu ====================//

void printMenu() {
    printf("\n==================== MENU DE FUNCIONARIOS ====================\n");
    printf("Escolha uma opcao para listar os funcionarios:\n");
    printf("1) Listar em ordem decrescente de idade | Bubble Sort\n");
    printf("2) Listar em ordem crescente de salario | Bubble Sort\n");
    printf("3) Listar em ordem alfabetica (nome)    | Bubble Sort\n");
    printf("4) Listar em ordem decrescente de idade | Selection Sort\n");
    printf("5) Listar em ordem crescente de salario | Selection Sort\n");
    printf("6) Listar em ordem alfabetica (nome)    | Selection Sort\n");
    printf("7) Sair do programa\n");
    printf("==============================================================\n");
    printf("Digite sua escolha: ");
}

int main() {

    char *arq = "./test.txt";
    int n;
    Funcionario **ArrayFuncionario = lerFuncionarios(arq, &n);

    int escolha;
    do {
        printMenu();
        scanf(" %d", &escolha); // O espaco antes do %c consome espacos em branco e novas linhas

        switch(escolha) {
            case 1:
                bubbleSort(ArrayFuncionario, n, compararFuncionarioIdadeDec);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 2:
                bubbleSort(ArrayFuncionario, n, compararFuncionarioSalario);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 3:
                bubbleSort(ArrayFuncionario, n, compararFuncionarioNome);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 4:
                selectionSort(ArrayFuncionario, n, compararFuncionarioIdadeDec);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 5:
                selectionSort(ArrayFuncionario, n, compararFuncionarioSalario);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 6:
                selectionSort(ArrayFuncionario, n, compararFuncionarioNome);
                printFuncionarios(ArrayFuncionario, n);
                break;
            case 7:
                printf("Finalizando o programa!");
                break; 
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (escolha != 7);

    limpaFuncionarios(ArrayFuncionario, n);
    return 0;
}

//======================================================================//