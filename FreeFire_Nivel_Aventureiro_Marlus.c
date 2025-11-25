#include <stdio.h>      // Traz funções para mostrar texto na tela e ler do teclado
#include <string.h>     // Traz funções para comparar, copiar e medir textos
#include <stdlib.h>     // Traz funções para pedir memória, limpar tela e outras coisas importantes

typedef struct {                    // Começa a criar um "molde" de dados chamado Item
    char nome[30];                  // Campo para guardar o nome do item (máximo 29 letras + 1 espaço secreto)
    char tipo[20];                  // Campo para guardar o tipo do item (arma, cura, munição, etc.)
    int quantidade;                 // Campo para guardar quantas unidades você tem daquele item
} Item;                             // Termina o molde e dá o nome "Item" para ele

typedef struct No {                 // Começa outro molde chamado "No" (vagão da lista encadeada)
    Item dados;                     // Dentro de cada vagão tem um Item completo
    struct No* proximo;             // Ponteiro que aponta para o próximo vagão (ou NULL se for o último)
} No;                               // Termina o molde do vagão

Item vetor_mochila[20];             // Cria um armário com exatamente 20 gavetas para guardar itens
int total_vetor = 0;                // Contador: quantas gavetas estão ocupadas agora (começa com zero)
int comp_seq_v = 0;                 // Contador de quantas vezes comparamos nomes na busca normal do vetor
int comp_bin_v = 0;                 // Contador de quantas vezes comparamos na busca binária do vetor

No* lista_mochila = NULL;           // Ponteiro que aponta pro primeiro vagão da lista encadeada (NULL = vazia)
int comp_seq_l = 0;                 // Contador de comparações na busca da lista encadeada

void menuPrincipal();               // Diz que vai existir uma função chamada menuPrincipal
void menuVetor();                   // Diz que vai existir o menu da mochila com vetor
void menuLista();                   // Diz que vai existir o menu da mochila com lista encadeada

void inserirVetor();                // Função para colocar um item novo no vetor
void removerVetor();                // Função para tirar um item do vetor
void listarVetor();                 // Função para mostrar todos os itens do vetor
void ordenarVetor();                // Função para organizar os itens do vetor por nome
int buscarSequencialVetor(const char* nome);  // Função para procurar item no vetor (jeito normal)
int buscarBinariaVetor(const char* nome);     // Função para procurar item no vetor (jeito rápido)

void inserirLista();                // Função para colocar item na lista encadeada
void removerLista();                // Função para tirar item da lista encadeada
void listarLista();                 // Função para mostrar itens da lista encadeada
int buscarSequencialLista(const char* nome);  // Função para procurar na lista encadeada
void liberarLista();                // Função para limpar toda a memória da lista encadeada

int main() {                        // Todo programa em C começa aqui — é a porta de entrada
    menuPrincipal();                // Chama o menu principal para começar o jogo
    return 0;                       // Diz ao sistema: "o programa terminou tudo certo"
}

void menuPrincipal() {              // Função que mostra o menu inicial
    int op;                         // Variável que guarda o número que o jogador digitar

    do {                            // Repete o menu até o jogador escolher sair
        system("cls");              // Limpa a tela do terminal (no Windows)

        printf("=== NIVEL AVENTUREIRO - MOCHILA DE SOBREVIVENCIA ===\n\n");
        printf("Escolha como quer organizar sua mochila:\n");
        printf("1. Vetor (rapido com busca binaria)\n");
        printf("2. Lista Encadeada (cresce infinitamente)\n");
        printf("0. Sair do jogo\n");
        printf("\nDigite sua opcao: ");

        scanf("%d", &op);           // Lê o número digitado e guarda na variável op
        getchar();                  // Limpa o "Enter" que fica preso no teclado depois do scanf

        switch(op) {                // Dependendo do número digitado, faz uma coisa diferente
            case 1: menuVetor(); break;     // Abre o menu do vetor
            case 2: menuLista(); break;     // Abre o menu da lista encadeada
            case 0: 
                printf("\nVoce fugiu com vida! Boa sorte sobrevivente!\n");
                break;              // Sai do jogo
            default: 
                printf("Opcao invalida! Tente novamente.\n");
        }

        if (op != 0) {              // Se não escolheu sair (0)
            printf("\nPressione ENTER para continuar...");
            getchar();              // Espera o jogador apertar Enter
        }
    } while (op != 0);              // Continua repetindo enquanto não digitar 0
}

void menuVetor() {                  // Menu da mochila com vetor
    int op;                         // Variável para guardar a opção escolhida

    do {                            // Repete até escolher voltar (0)
        system("cls");              // Limpa a tela
        printf("=== MOCHILA COM VETOR (20 espacos fixos) ===\n");
        printf("Itens carregando: %d de 20\n\n", total_vetor);

        printf("1. Coletar item\n");
        printf("2. Descartar item\n");
        printf("3. Ver mochila\n");
        printf("4. Organizar por nome (precisa pra busca binaria)\n");
        printf("5. Procurar item (busca normal)\n");
        printf("6. Procurar item (busca binaria - MUITO mais rapida)\n");
        printf("0. Voltar\n");
        printf("\nO que quer fazer? ");

        scanf("%d", &op);           // Lê a opção
        getchar();                  // Limpa o Enter

        comp_seq_v = 0;             // Zera contador de comparações antes de cada busca
        comp_bin_v = 0;             // Zera contador da busca binária

        switch(op) {                // Executa a função certa
            case 1: inserirVetor(); break;
            case 2: removerVetor(); break;
            case 3: listarVetor(); break;
            case 4: ordenarVetor(); break;
            case 5: {                   // Bloco para buscar sequencial
                char nome[30];          // Variável temporária para o nome
                printf("Nome do item que quer procurar: ");
                fgets(nome, 30, stdin); // Lê o nome completo
                nome[strcspn(nome, "\n")] = 0;  // Remove o Enter do final
                buscarSequencialVetor(nome);    // Procura
                printf("Foram feitas %d comparacoes\n", comp_seq_v);
                break;
            }
            case 6: {                   // Bloco para busca binária
                char nome[30];
                printf("Nome do item que quer procurar: ");
                fgets(nome, 30, stdin);
                nome[strcspn(nome, "\n")] = 0;
                int pos = buscarBinariaVetor(nome);
                if (pos != -1) printf("Encontrado na posicao %d!\n", pos+1);
                printf("Busca binaria fez apenas %d comparacoes!\n", comp_bin_v);
                break;
            }
            case 0: break;              // Volta pro menu principal
            default: printf("Opcao invalida!\n");
        }

        if (op != 0) {              // Se não voltou
            printf("\nPressione ENTER para continuar...");
            getchar();              // Espera apertar Enter
        }
    } while (op != 0);              // Continua até escolher 0
}

void inserirVetor() {               // Função para adicionar um item no vetor
    if (total_vetor >= 20) {        // Se já tem 20 itens
        printf("Mochila cheia! Nao cabe mais nada!\n");
        return;                     // Sai da função sem fazer nada
    }

    printf("Nome do item: ");
    fgets(vetor_mochila[total_vetor].nome, 30, stdin);  // Lê o nome
    vetor_mochila[total_vetor].nome[strcspn(vetor_mochila[total_vetor].nome, "\n")] = 0;  // Tira o Enter

    printf("Tipo (arma, cura, etc): ");
    fgets(vetor_mochila[total_vetor].tipo, 20, stdin);
    vetor_mochila[total_vetor].tipo[strcspn(vetor_mochila[total_vetor].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &vetor_mochila[total_vetor].quantidade);  // Lê o número
    getchar();                  // Limpa o Enter que sobra

    total_vetor++;              // Aumenta o contador de itens
    printf("Item coletado com sucesso!\n");
}

void removerVetor() {               // Função para remover um item do vetor
    char nome[30];                  // Variável temporária
    printf("Nome do item pra jogar fora: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = 0;

    int i, pos = -1;                // pos = posição do item encontrado (-1 = não achou)
    for (i = 0; i < total_vetor; i++) {
        if (strcmp(vetor_mochila[i].nome, nome) == 0) {  // Se os nomes são iguais
            pos = i;
            break;                  // Para de procurar
        }
    }
    if (pos == -1) {                // Se não achou
        printf("Item nao encontrado!\n");
        return;
    }
    for (i = pos; i < total_vetor - 1; i++) {
        vetor_mochila[i] = vetor_mochila[i+1];  // Move todos pra frente
    }
    total_vetor--;                  // Diminui o contador
    printf("Item descartado!\n");
}

void listarVetor() {                // Mostra todos os itens do vetor
    system("cls");
    printf("=== CONTEUDO DA MOCHILA (VETOR) ===\n");
    if (total_vetor == 0) {
        printf("Ta vazia... corre coletar itens!\n");
    } else {
        for (int i = 0; i < total_vetor; i++) {
            printf("%d. [%s] %s - %d unidades\n", i+1,
                   vetor_mochila[i].tipo,
                   vetor_mochila[i].nome,
                   vetor_mochila[i].quantidade);
        }
    }
}

void ordenarVetor() {               // Organiza os itens por ordem alfabética (Bubble Sort)
    int i, j;
    Item temp;                      // Variável temporária para troca
    for (i = 0; i < total_vetor - 1; i++) {
        for (j = 0; j < total_vetor - i - 1; j++) {
            if (strcmp(vetor_mochila[j].nome, vetor_mochila[j+1].nome) > 0) {
                temp = vetor_mochila[j];
                vetor_mochila[j] = vetor_mochila[j+1];
                vetor_mochila[j+1] = temp;
            }
        }
    }
    printf("Mochila organizada em ordem alfabetica!\n");
}

int buscarSequencialVetor(const char* nome) {  // Procura item do jeito normal (um por um)
    for (int i = 0; i < total_vetor; i++) {
        comp_seq_v++;           // Conta mais uma comparação
        if (strcmp(vetor_mochila[i].nome, nome) == 0) {
            printf("ACHADO! [%s] %s - %d unid.\n",
                   vetor_mochila[i].tipo, vetor_mochila[i].nome, vetor_mochila[i].quantidade);
            return i;
        }
    }
    printf("Nao achei esse item.\n");
    return -1;
}

int buscarBinariaVetor(const char* nome) {     // Procura do jeito rápido (só funciona se estiver ordenado)
    int inicio = 0, fim = total_vetor - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comp_bin_v++;
        int resultado = strcmp(vetor_mochila[meio].nome, nome);
        if (resultado == 0) {
            printf("ACHADO RAPIDO! [%s] %s - %d unid.\n",
                   vetor_mochila[meio].tipo, vetor_mochila[meio].nome, vetor_mochila[meio].quantidade);
            return meio;
        }
        if (resultado < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    printf("Nao encontrado (busca binaria).\n");
    return -1;
}

void menuLista() {                  // Menu da lista encadeada
    int op;
    do {
        system("cls");
        printf("=== MOCHILA COM LISTA ENCADEADA (t38amanho infinito) ===\n\n");
        printf("1. Coletar item\n2. Descartar\n3. Ver mochila\n4. Procurar\n0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();
        comp_seq_l = 0;

        switch(op) {
            case 1: inserirLista(); break;
            case 2: removerLista(); break;
            case 3: listarLista(); break;
            case 4: {
                char nome[30];
                printf("Nome do item: ");
                fgets(nome, 30, stdin);
                nome[strcspn(nome, "\n")] = 0;
                buscarSequencialLista(nome);
                printf("Comparacoes feitas: %d\n", comp_seq_l);
                break;
            }
            case 0: liberarLista(); break;
        }
        if (op != 0) {
            printf("\nPressione ENTER...");
            getchar();
        }
    } while (op != 0);
}

void inserirLista() {               // Adiciona item na frente da lista encadeada
    No* novo = (No*)malloc(sizeof(No));     // Pede memória para um novo vagão
    if (!novo) { printf("Sem memoria!\n"); return; }

    printf("Nome: ");
    fgets(novo->dados.nome, 30, stdin);
    novo->dados.nome[strcspn(novo->dados.nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(novo->dados.tipo, 20, stdin);
    novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);
    getchar();

    novo->proximo = lista_mochila;          // Novo aponta pro antigo primeiro
    lista_mochila = novo;                   // Novo vira o primeiro
    printf("Item adicionado!\n");
}

void removerLista() {               // Remove um item da lista encadeada
    char nome[30];
    printf("Nome do item pra remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = 0;

    No *atual = lista_mochila;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (!atual) { printf("Nao achei!\n"); return; }

    if (!anterior) lista_mochila = atual->proximo;
    else anterior->proximo = atual->proximo;

    free(atual);
    printf("Item removido!\n");
}

void listarLista() {                // Mostra todos os itens da lista encadeada
    system("cls");
    printf("=== MOCHILA (LISTA ENCADEADA) ===\n");
    if (!lista_mochila) { printf("Vazia!\n"); return; }

    No* temp = lista_mochila;
    int num = 1;
    while (temp != NULL) {
        printf("%d. [%s] %s - %d unid.\n", num++,
               temp->dados.tipo, temp->dados.nome, temp->dados.quantidade);
        temp = temp->proximo;
    }
}

int buscarSequencialLista(const char* nome) {   // Procura item na lista encadeada
    No* temp = lista_mochila;
    while (temp != NULL) {
        comp_seq_l++;
        if (strcmp(temp->dados.nome, nome) == 0) {
            printf("ACHADO! [%s] %s - %d unid.\n",
                   temp->dados.tipo, temp->dados.nome, temp->dados.quantidade);
            return 1;
        }
        temp = temp->proximo;
    }
    printf("Nao encontrado.\n");
    return 0;
}

void liberarLista() {               // Limpa toda a memória da lista encadeada
    No* atual = lista_mochila;
    while (atual != NULL) {
        No* prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    lista_mochila = NULL;
    printf("Mochila limpa da memoria!\n");
}