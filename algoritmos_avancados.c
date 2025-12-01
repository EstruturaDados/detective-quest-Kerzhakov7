#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da Sala (Nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda; // Ponteiro para o caminho à esquerda
    struct Sala *direita;  // Ponteiro para o caminho à direita
} Sala;

// -------------------------------------------------------------------------
// Função: criarSala
// Propósito: Cria dinamicamente um novo nó (sala) na memória.
// Parâmetros: nome da sala (string).
// Retorno: Ponteiro para a nova sala criada.
// -------------------------------------------------------------------------
Sala* criarSala(char *nome) {
    // Alocação dinâmica de memória para a estrutura Sala
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro: Falha ao alocar memória!\n");
        exit(1);
    }

    // Copia o nome para o campo da struct e inicializa os filhos como NULL
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// -------------------------------------------------------------------------
// Função: explorarSalas
// Propósito: Controla a navegação interativa do jogador pela árvore.
// Parâmetros: Ponteiro para a sala atual (raiz da sub-árvore).
// -------------------------------------------------------------------------
void explorarSalas(Sala *salaAtual) {
    char opcao;

    printf("\n--- INICIANDO EXPLORAÇÃO ---\n");

    while (salaAtual != NULL) {
        printf("\n📍 Você está em: ** %s **\n", salaAtual->nome);

        // Verifica se é um nó-folha (sem saídas)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n🔍 Fim da linha! Esta sala não tem mais saídas.\n");
            printf("Você investigou todas as pistas deste caminho.\n");
            break; // Encerra o loop de exploração
        }

        // Menu de navegação
        printf("Para onde deseja ir?\n");
        if (salaAtual->esquerda != NULL) printf(" [e] Esquerda\n");
        if (salaAtual->direita != NULL)  printf(" [d] Direita\n");
        printf(" [s] Sair do jogo\n");
        printf("Escolha: ");
        
        // O espaço antes de %c ignora quebras de linha pendentes no buffer
        scanf(" %c", &opcao); 

        // Lógica de movimentação
        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if (opcao == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else if (opcao == 's') {
            printf("Saindo da mansão...\n");
            break;
        } 
        else {
            printf("\n🚫 Caminho inválido! Tente novamente.\n");
        }
    }
}

// -------------------------------------------------------------------------
// Função: liberarMapa (Extra)
// Propósito: Libera a memória alocada recursivamente (boas práticas).
// -------------------------------------------------------------------------
void liberarMapa(Sala *raiz) {
    if (raiz == NULL) return;
    liberarMapa(raiz->esquerda);
    liberarMapa(raiz->direita);
    free(raiz);
}

// -------------------------------------------------------------------------
// Função: main
// Propósito: Monta o mapa inicial e dá início à exploração.
// -------------------------------------------------------------------------
int main() {
    printf("=== DETECTIVE QUEST: MAP SYSTEM ===\n");

    // 1. Criação manual da Árvore (Mapa da Mansão)
    
    // Nível 1 (Raiz)
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 2
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");

    // Nível 3 (Nós-folha / Pontos finais)
    // Filhos da Sala de Estar
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Jardim de Inverno");

    // Filhos da Cozinha
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Porão Escuro");

    // 2. Iniciar a navegação
    explorarSalas(hall);

    // 3. Limpeza de memória antes de fechar
    liberarMapa(hall);

    return 0;
}