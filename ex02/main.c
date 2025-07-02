#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============= Prototipo de todas as funcoes ======================== //
int *gerarDadosCrescente(int n);
int *gerarDadosDecrescente(int n);
int *gerarDadosAleatorio(int n);
int *gerarDados(int n, int escolha);
void printarDados(int arr[], int n);

void bubbleSort(int arr[], int n, int *comparacoes, int *trocas);
void selectionSort(int arr[], int n, int *comparacoes, int *trocas);
void insertionSort(int arr[], int n, int *comparacoes, int *trocas);
void shellSort(int arr[], int n, int *comparacoes, int *trocas);

void calcularMetricas(int n, int escolha, int alg);
void printMetricas(char *nome, double tempo_execucao, int comparacoes, int trocas, int n);
void printMenu();

// ===================== Gerador com Base em um N ===================== //

int* gerarDadosCrescente(int n){
    int *vetor = (int *)malloc(sizeof(int) * n);

    // Gerando os valores de 1 até N
    for(int i = 0; i < n; i++)
        vetor[i] = i + 1;
    
    return vetor;
}

int* gerarDadosDecrescente(int n){
    int *vetor = (int *)malloc(sizeof(int) * n);

    // Gerando os valores de N até 1
    for(int i = 0; i < n; i++)
        vetor[i] = n - i;
    
    return vetor;
}

int* gerarDadosAleatorio(int n){
    // Colocando a semente aleatoria a cada iteração do programa
    srand (time(NULL));

    int *vetor = (int *)malloc(sizeof(int) * n);

    // Gerando valores aleatoria com 1 <= x <= n
    for(int i = 0; i < n; i++)
        vetor[i] = (rand() % n) + 1;

    return vetor;
}

int *gerarDados(int n, int escolha){
    switch (escolha)
    {
        case 1:
            return gerarDadosCrescente(n);
            break;
        case 2:
            return gerarDadosDecrescente(n);
            break;
        case 3:
            return gerarDadosAleatorio(n);
            break;
        default:
            return NULL;
            break;
    }
}

void printarDados(int arr[], int n){
    printf("\n============================================\n");
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n============================================\n");
}

// ============================================================ //

// ========================== ALGORITMOS DE ORDENAÇÃO ========= //

// === Bubble Sort === //

void bubbleSort(int arr[], int n, int *comparacoes, int *trocas){
    int temp, flag = 0;
    for(int i = 0; i < n - 1; i++){
        flag = 0;
        for(int j = 0; j < n - 1 - i; j++){
            if(arr[j] > arr[j + 1]){
                temp       = arr[j];
                arr[j]     = arr[j + 1];
                arr[j + 1] = temp;
                *trocas += 1;
                flag = 1;
            }
            *comparacoes += 1;
        }

        if(flag == 0)
            break;
    }
}

// ================== //

// === Selection Sort === //

void selectionSort(int arr[], int n, int *comparacoes, int *trocas){
    int temp, menor, flag;

    for(int i = 0; i < n - 1; i++){
        menor = i;
        flag = 0;
        for(int j = i + 1; j < n; j++){
            if(arr[j] < arr[menor]){
                flag = 1;
                menor = j;
            }
            *comparacoes += 1;
        }

        if(flag == 1){
            temp       = arr[i];
            arr[i]     = arr[menor];
            arr[menor] = temp;
            *trocas += 1;
        }
    }
}

// ====================== //

// === Insertion Sort === //

void insertionSort (int arr[], int n, int *comparacoes, int *trocas)
{ 
    int i, j, tmp;
    for (i = 1; i < n; i++) {
        tmp = arr[i];
        //coloca o item no lugar apropriado na sequência destino
        for (j = i - 1 ; j >= 0 && arr[j] > tmp ; j--, *comparacoes += 1){
            arr[j + 1] = arr[j];
            *trocas += 1;
        }
        if(j >= 0)
            *comparacoes += 1;
            
        arr[j + 1] = tmp;
    }
}

// ====================== //

// ===== Shell Sort ======= //

void shellSort(int arr[], int n, int *comparacoes, int *trocas){
    int h = 1, temp, i, j;

    // Calculo do H
    do {h = 3 * h + 1;} while(h < n);

    do{
        h /= 3;
        for(i = h; i < n; i++){
            temp = arr[i];
            j = i;
            while(arr[j - h] > temp){
                arr[j] = arr[j - h]; 
                j -= h;
                *trocas += 1;
                *comparacoes += 1;
                if(j < h){
                    *comparacoes -= 1;
                    break;
                }
            }
            *comparacoes += 1;
            arr[j] = temp;
        }
    } while(h != 1);
}

// ====================== //

// ================= MENU ================ //

void calcularMetricas(int n, int escolha, int alg){
    int cmp, trocas;
    int *arr = NULL;

    time_t ini;
    time_t fim;
    double tempo_total;

    cmp = 0;
    trocas = 0;
    arr = gerarDados(n, escolha);

    // Tempo de cada algortimo 
    switch (alg)
    {
        case 1:
            ini = clock();
            bubbleSort(arr, n, &cmp, &trocas);
            fim = clock();
            break;
        case 2:
            ini = clock();
            selectionSort(arr, n, &cmp, &trocas);
            fim = clock();
            break;
        case 3:
            ini = clock();
            insertionSort(arr, n, &cmp, &trocas);
            fim = clock();
            break;
        case 4:
            ini = clock();
            shellSort(arr, n, &cmp, &trocas);
            fim = clock();
            break;
        default:
            ini = clock();
            bubbleSort(arr, n, &cmp, &trocas);
            fim = clock();
            break;
    }

    if(arr != NULL){
        free(arr);
        arr = NULL;
    }

    tempo_total = (double) (fim - ini) / CLOCKS_PER_SEC;

    switch (alg)
    {
        case 1:
            printMetricas("Bubble Sort", tempo_total, cmp, trocas, n);
            break;
        case 2:
            printMetricas("Selection Sort", tempo_total, cmp, trocas, n);
            break;
        case 3:
            printMetricas("Insertion Sort", tempo_total, cmp, trocas, n);
            break;
        case 4:
            printMetricas("Shell Sort", tempo_total, cmp, trocas, n);
            break;
        default:
            printMetricas("Bubble Sort", tempo_total, cmp, trocas, n);
            break;
    }

    // ========================================== //

}

void printMetricas(char *nome, double tempo_execucao, int comparacoes, int trocas, int n) {
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("%-20s | %-20.8f | %-15d | %-15d | %-15d\n", nome, tempo_execucao, comparacoes, trocas, n);
    printf("--------------------------------------------------------------------------------------------------------\n");
}

void printMenu() {
    printf("\n==================== MENU DE FUNCIONARIOS ====================\n");
    printf("Escolha uma opcao para listar os funcionarios:\n");
    printf("1) Vetores de entrada com números inteiros crescentes (5.000, 10.000, 20.000 e 100.000)\n");
    printf("2) Vetores de entrada com números inteiros decrescente (5.000, 10.000, 20.000 e 100.000)\n");
    printf("3) Vetores de entrada com números inteiros aleatorios (5.000, 10.000, 20.000 e 100.000)\n");
    printf("4) Sair do programa\n");
    printf("==============================================================\n");
    printf("Digite sua escolha: ");
}

int main(){
    int escolha = 0;
    do {
        printMenu();
        scanf(" %d", &escolha);

        if(escolha >= 1 && escolha <= 3){
            printf("\n%-20s | %-20s | %-15s | %-15s | %-20s\n", "Algoritmo", "Tempo de Execucao (s)", "Comparacoes", "Trocas", "Numero de Elementos");
            /*
                Bubble Sort    == 1
                Selection Sort == 2
                Insertion Sort == 3
                Shell Sort     == 4
            */ 
            
            // ========== Metricas com 5.000 elementos ============ //
            calcularMetricas(5000, escolha, 1);
            calcularMetricas(5000, escolha, 2);
            calcularMetricas(5000, escolha, 3);
            calcularMetricas(5000, escolha, 4);

            // ========== Metricas com 10.000 elementos ============ //
            calcularMetricas(10000, escolha, 1);
            calcularMetricas(10000, escolha, 2);
            calcularMetricas(10000, escolha, 3);
            calcularMetricas(10000, escolha, 4);

            // ========== Metricas com 20.000 elementos ============ //
            calcularMetricas(20000, escolha, 1);
            calcularMetricas(20000, escolha, 2);
            calcularMetricas(20000, escolha, 3);
            calcularMetricas(20000, escolha, 4);

            // ========== Metricas com 100.000 elementos ============ //
            calcularMetricas(100000, escolha, 1);
            calcularMetricas(100000, escolha, 2);
            calcularMetricas(100000, escolha, 3);
            calcularMetricas(100000, escolha, 4);
        }
        else if(escolha == 4){
            printf("Programa finalizado.\n");
        }
        else{
            printf("Opcao invalida! Tente novamente.\n");
        }

    } while (escolha != 4);
    
    return 0;
}