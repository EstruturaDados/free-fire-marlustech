#include <stdio.h>      // Essa linha traz as ferramentas básicas para mostrar texto na tela (printf) e ler do teclado (scanf, fgets)
#include <string.h>     // Essa traz funções para trabalhar com palavras/texto: comparar (strcmp), copiar (strcpy), medir tamanho (strlen)
#include <stdlib.h>     // Essa traz funções extras: limpar tela (system), sair do programa, etc.

// Aqui estamos criando o "molde" de um item do jogo. É como se fosse a ficha de cada objeto
typedef struct {                    // Começa a criar um tipo novo chamado "struct"
    char nome[30];                  // Campo para guardar o nome do item. Máximo 29 letras + 1 espaço secreto no final
    char tipo[20];                  // Campo para o tipo: "arma", "cura", "munição", etc.
    int quantidade;                 // Campo que guarda quantas unidades daquele item o jogador tem
} Item;                             // Terminamos o molde e damos o nome "Item" para ele

// Aqui criamos a mochila do jogador: um "armário" com exatamente 10 gavetas
Item mochila[10];                   // Cada posição (0 a 9) pode guardar 1 Item completo
int total_itens = 0;                // Contador que diz quantas gavetas estão ocupadas temos (começa zerado)

// Vamos avisar ao computador que essas funções vão existir mais abaixo
void inserirItem();                 // Função para coletar um item novo
void removerItem();                 // Função para jogar fora um item
void listarItens();                 // Função para mostrar tudo que está na mochila
void buscarItem();                  // Função para procurar um item específico

// A função main é a porta de entrada. Todo programa em C começa aqui.
int main() {
    int opcao;                      // Variável que vai guardar o número que o jogador digitar

    // Esse "do-while" faz o menu aparecer várias vezes até o jogador escolher sair
    do {
        system("cls");              // Limpa a tela do terminal (só funciona no Windows)

        // Mensagens bonitas para o jogador ver o estado do jogo
        printf("=== MOCHILA DE SOBREVIVENCIA ===\n");
        printf("Itens na mochila: %d/10\n\n", total_itens);

        // Menu de opções
        printf("1. Coletar item (inserir)\n");
        printf("2. Descartar item (remover)\n");
        printf("3. Ver mochila (listar)\n");
        printf("4. Procurar item (buscar)\n");
        printf("0. Fugir da zona (sair)\n");
        printf("\nO que voce quer fazer? ");

        scanf("%d", &opcao);        // Lê o número que o jogador digitou e guarda na variável opcao
        getchar();                  // Limpa o "Enter" que fica preso no teclado depois do scanf

        // Dependendo do número digitado, chama uma função diferente
        switch(opcao) {
            case 1: 
                inserirItem();      // Chama a função de adicionar item
                break;                  // Sai do switch
            case 2: 
                removerItem();          // Chama a função de remover
                break;
            case 3: 
                listarItens();          // Mostra tudo que tem na mochila
                break;
            case 4: 
                buscarItem();           // Procura um item específico
                break;
            case 0: 
                printf("Voce fugiu com vida! Boa sorte sobrevivente!\n");
                break;                  // Sai do jogo
            default: 
                printf("Opcao invalida! Tente de novo.\n"); // Se digitar número errado
        }

        // Se não escolheu sair (0), espera o jogador apertar Enter para continuar
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();              // Espera você apertar Enter
        }

    } while(opcao != 0);            // Continua repetindo enquanto não digitar 0

    return 0;                       // Diz ao Windows que o programa terminou tudo certo
}

// Função para o jogador coletar um item novo no mapa
void inserirItem() {
    // Verifica se já tem 10 itens (mochila cheia)
    if (total_itens >= 10) {
        printf("Mochila cheia! Voce nao pode carregar mais nada!\n");
        return;                     // Sai da função sem fazer mais nada
    }

    printf("\n--- COLETANDO ITEM ---\n");
    printf("Nome do item: ");
    // Lê o nome completo (permite espaços) e guarda na posição atual da mochila
    fgets(mochila[total_itens].nome, 30, stdin);
    // Remove o Enter (\n) que fica no final da linha
    mochila[total_itens].nome[strcspn(mochila[total_itens].nome, "\n")] = 0;

    printf("Tipo (arma, cura, municao, ferramenta): ");
    fgets(mochila[total_itens].tipo, 20, stdin);
    mochila[total_itens].tipo[strcspn(mochila[total_itens].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &mochila[total_itens].quantidade);  // Lê o número
    getchar();                  // Limpa o Enter que sobra do scanf

    total_itens++;              // Aumenta o contador: agora tem 1 item a mais
    printf("\nItem coletado com sucesso!\n");
}

// Função para jogar fora um item que não quer mais
void removerItem() {
    if (total_itens == 0) {     // Se não tem nada na mochila
        printf("Sua mochila ja esta vazia!\n");
        return;
    }

    char nome_busca[30];        // Variável temporária para guardar o nome que o jogador quer remover
    printf("\nDigite o nome do item que quer descartar: ");
    fgets(nome_busca, 30, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0; // tira o Enter

    int encontrado = 0;         // 0 = não achou, 1 = achou

    // Percorre toda a mochila procurando pelo nome
    for (int i = 0; i < total_itens; i++) {
        // strcmp retorna 0 quando as duas palavras são exatamente iguais
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            printf("Descartando: %s (%d unidades)\n", mochila[i].nome, mochila[i].quantidade);

            // Move todos os itens de trás para frente para "fechar o buraco"
            for (int j = i; j < total_itens - 1; j++) {
                mochila[j] = mochila[j + 1];  // Copia o próximo item para a posição atual
            }

            total_itens--;          // Diminui o contador
            encontrado = 1;         // Marca que achou e removeu
            break;                  // Para de procurar
        }
    }

    // Se não achou nenhum item com aquele nome
    if (!encontrado) {
        printf("Item nao encontrado na mochila!\n");
    } else {
        printf("Item descartado!\n");
    }
}

// Função para mostrar tudo que está dentro da mochila
void listarItens() {
    system("cls");              // Limpa a tela para ficar bonito
    printf("=== SUA MOCHILA ===\n");

    if (total_itens == 0) {
        printf("Mochila vazia... voce precisa coletar itens rapido!\n");
    } else {
        // Mostra cada item numerado de 1 em 1
        for (int i = 0; i < total_itens; i++) {
            printf("%d. [%s] %s - Quantidade: %d\n", 
                   i+1,                     // número para o jogador (começa em 1)
                   mochila[i].tipo,         // tipo entre colchetes
                   mochila[i].nome,         // nome do item
                   mochila[i].quantidade);  // quantas unidades
        }
    }
}

// Função para procurar um item específico pelo nome
void buscarItem() {
    if (total_itens == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nome_busca[30];
    printf("\nDigite o nome do item que quer procurar: ");
    fgets(nome_busca, 30, stdin);
    nome_busca[strcspn(nome_busca, "\n")] = 0;

    int encontrado = 0;

    for (int i = 0; i < total_itens; i++) {
        if (strcmp(mochila[i].nome, nome_busca) == 0) {
            printf("\nITEM ENCONTRADO!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Item nao encontrado!\n");
    }
}