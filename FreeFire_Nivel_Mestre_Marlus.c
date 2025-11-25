#include <stdio.h>      // Para printf, scanf, fgets
#include <string.h>     // Para strcmp, strcpy, strlen
#include <stdlib.h>     // Para system, malloc, free
#include <time.h>       // Para clock() — medir tempo em milissegundos

// ====================================================================
// 1. STRUCT DO COMPONENTE DA TORRE DE FUGA
// ====================================================================

typedef struct {
    char nome[30];      // Ex: "Chip Central", "Motor Principal"
    char tipo[20];      // Ex: "controle", "suporte", "propulsão"
    int prioridade;     // 1 (baixa) até 10 (CRÍTICA — tem que montar primeiro!)
} Componente;

// ====================================================================
// 2. VARIÁVEIS GLOBAIS
// ====================================================================

Componente torre[20];           // Armazena até 20 componentes da torre
int total_componentes = 0;      // Quantos componentes já foram cadastrados

long comparacoes_bubble = 0;    // Conta comparações no Bubble Sort
long comparacoes_insertion = 0; // Conta comparações no Insertion Sort
long comparacoes_selection = 0; // Conta comparações no Selection Sort

// ====================================================================
// 3. PROTÓTIPOS DAS FUNÇÕES
// ====================================================================

void menuMestre();
void cadastrarComponentes();
void bubbleSortPorNome();
void insertionSortPorTipo();
void selectionSortPorPrioridade();
int buscaBinariaPorNome(const char* nome_procurado);
void mostrarComponentes();
double medirTempo(void (*funcao)());  // Função que mede tempo de execução

// ====================================================================
// 4. FUNÇÃO PRINCIPAL — O DESAFIO FINAL COMEÇA AQUI
// ====================================================================

int main() {
    menuMestre();       // Abre o menu principal do nível mestre
    return 0;           // Termina o programa
}

// ====================================================================
// 5. MENU PRINCIPAL DO NÍVEL MESTRE
// ====================================================================

void menuMestre() {
    int op;
    do {
        system("cls");
        printf("=== DESAFIO FINAL - TORRE DE FUGA DA ILHA ===\n");
        printf("A safe zone esta fechando... monte a torre ou morra!\n\n");
        printf("Componentes cadastrados: %d/20\n\n", total_componentes);
        printf("1. Cadastrar componentes da torre\n");
        printf("2. Ordenar por NOME (Bubble Sort)\n");
        printf("3. Ordenar por TIPO (Insertion Sort)\n");
        printf("4. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5. Buscar componente-chave (Busca Binaria - so funciona se ordenado por nome)\n");
        printf("6. Mostrar torre montada\n");
        printf("0. Fugir da ilha (sair)\n");
        printf("\nEscolha sabiamente: ");
        scanf("%d", &op);
        getchar();  // limpa o Enter

        comparacoes_bubble = comparacoes_insertion = comparacoes_selection = 0;

        switch(op) {
            case 1: cadastrarComponentes(); break;
            case 2: {
                double tempo = medirTempo(bubbleSortPorNome);
                printf("Bubble Sort concluido!\n");
                printf("Comparacoes: %ld | Tempo: %.2f ms\n", comparacoes_bubble, tempo);
                break;
            }
            case 3: {
                double tempo = medirTempo(insertionSortPorTipo);
                printf("Insertion Sort concluido!\n");
                printf("Comparacoes: %ld | Tempo: %.2f ms\n", comparacoes_insertion, tempo);
                break;
            }
            case 4: {
                double tempo = medirTempo(selectionSortPorPrioridade);
                printf("Selection Sort concluido!\n");
                printf("Comparacoes: %ld | Tempo: %.2f ms\n", comparacoes_selection, tempo);
                break;
            }
            case 5: {
                if (total_componentes == 0) {
                    printf("Cadastre componentes primeiro!\n");
                } else {
                    char chave[30];
                    printf("Nome do componente-chave para ativar a torre: ");
                    fgets(chave, 30, stdin);
                    chave[strcspn(chave, "\n")] = 0;
                    int pos = buscaBinariaPorNome(chave);
                    if (pos != -1) {
                        printf("\nCOMPONENTE-CHAVE ENCONTRADO!\n");
                        printf("A torre esta ativa! Voce pode fugir!\n");
                    } else {
                        printf("\nComponente nao encontrado... a torre nao liga!\n");
                        printf("Voce foi consumido pela zona...\n");
                    }
                }
                break;
            }
            case 6: mostrarComponentes(); break;
            case 0: printf("\nFuga iniciada... boa sorte, sobrevivente!\n"); break;
            default: printf("Opcao invalida!\n");
        }
        if (op != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } while (op != 0);
}

// ====================================================================
// 6. CADASTRAR COMPONENTES DA TORRE
// ====================================================================

void cadastrarComponentes() {
    if (total_componentes >= 20) {
        printf("Limite de componentes atingido!\n");
        return;
    }
    printf("\n--- CADASTRANDO COMPONENTE %d ---\n", total_componentes + 1);
    printf("Nome: ");
    fgets(torre[total_componentes].nome, 30, stdin);
    torre[total_componentes].nome[strcspn(torre[total_componentes].nome, "\n")] = 0;

    printf("Tipo (controle/suporte/propulsao): ");
    fgets(torre[total_componentes].tipo, 20, stdin);
    torre[total_componentes].tipo[strcspn(torre[total_componentes].tipo, "\n")] = 0;

    printf("Prioridade (1 a 10 - 10 = mais urgente): ");
    scanf("%d", &torre[total_componentes].prioridade);
    getchar();

    total_componentes++;
    printf("Componente cadastrado!\n");
}

// ====================================================================
// 7. BUBBLE SORT POR NOME (alfabético)
// ====================================================================

void bubbleSortPorNome() {
    int i, j;
    Componente temp;
    for (i = 0; i < total_componentes - 1; i++) {
        for (j = 0; j < total_componentes - i - 1; j++) {
            comparacoes_bubble++;  // Conta cada comparação
            if (strcmp(torre[j].nome, torre[j+1].nome) > 0) {
                temp = torre[j];
                torre[j] = torre[j+1];
                torre[j+1] = temp;
            }
        }
    }
}

// ====================================================================
// 8. INSERTION SORT POR TIPO
// ====================================================================

void insertionSortPorTipo() {
    int i, j;
    Componente chave;
    for (i = 1; i < total_componentes; i++) {
        chave = torre[i];
        j = i - 1;
        while (j >= 0) {
            comparacoes_insertion++;
            if (strcmp(torre[j].tipo, chave.tipo) > 0) {
                torre[j+1] = torre[j];
                j--;
            } else {
                break;
            }
        }
        comparacoes_insertion++;  // conta a última comparação
        torre[j+1] = chave;
    }
}

// ====================================================================
// 9. SELECTION SORT POR PRIORIDADE (decrescente: 10 primeiro)
// ====================================================================

void selectionSortPorPrioridade() {
    int i, j, maior;
    Componente temp;
    for (i = 0; i < total_componentes - 1; i++) {
        maior = i;
        for (j = i + 1; j < total_componentes; j++) {
            comparacoes_selection++;
            if (torre[j].prioridade > torre[maior].prioridade) {
                maior = j;
            }
        }
        if (maior != i) {
            temp = torre[i];
            torre[i] = torre[maior];
            torre[maior] = temp;
        }
    }
}

// ====================================================================
// 10. BUSCA BINÁRIA POR NOME (só funciona se estiver ordenado por nome!)
// ====================================================================

int buscaBinariaPorNome(const char* nome_procurado) {
    int inicio = 0, fim = total_componentes - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        int cmp = strcmp(torre[meio].nome, nome_procurado);
        if (cmp == 0) {
            printf("Componente-chave encontrado na posicao %d!\n", meio+1);
            return meio;
        }
        if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;  // não encontrado
}

// ====================================================================
// 11. MOSTRAR TODOS OS COMPONENTES
// ====================================================================

void mostrarComponentes() {
    system("cls");
    printf("=== TORRE DE FUGA - COMPONENTES ===\n");
    if (total_componentes == 0) {
        printf("Nenhum componente cadastrado!\n");
    } else {
        for (int i = 0; i < total_componentes; i++) {
            printf("%d. [%s] %s - Prioridade: %d\n", i+1,
                   torre[i].tipo, torre[i].nome, torre[i].prioridade);
        }
    }
}

// ====================================================================
// 12. MEDIR TEMPO DE EXECUÇÃO DE UM ALGORITMO
// ====================================================================

double medirTempo(void (*funcao)()) {
    clock_t inicio = clock();   // Marca o tempo atual
    funcao();                   // Executa o algoritmo (bubble, insertion, etc.)
    clock_t fim = clock();      // Marca o tempo depois
    return ((double)(fim - inicio)) * 1000 / CLOCKS_PER_SEC;  // Converte para milissegundos
}