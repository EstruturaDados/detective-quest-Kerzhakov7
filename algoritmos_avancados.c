#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estruturas de Dados ---

// Estrutura para a Árvore BST de Pistas (Inventário)
typedef struct PistaNode {
    char conteudo[100]; // Texto da pista
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Estrutura para a Árvore Binária do Mapa (Salas)
typedef struct Sala {
    char nome[50];      // Nome do cômodo
    char pista[100];    // Pista contida na sala (se vazio, não tem pista)
    struct Sala *esq;   // Caminho à esquerda
    struct Sala *dir;   // Caminho à direita
} Sala;

// --- Protótipos das Funções ---
Sala* criarSala(char* nome, char* pista);
PistaNode* inserirPista(PistaNode* raiz, char* pista);
void exibirPistas(PistaNode* raiz);
void explorarSalasComPistas(Sala* mapa, PistaNode** inventarioPistas);

// --- Função Principal ---
int main() {
    // 1. Criação do Mapa da Mansão (Árvore Binária Fixa)
    // Raiz
    Sala* mansao = criarSala("Hall de Entrada", "Pegada de lama no tapete");

    // Lado Esquerdo da Mansão
    mansao->esq = criarSala("Cozinha", "Faca faltando no faqueiro");
    mansao->esq->esq = criarSala("Despensa", ""); // Sala sem pista
    mansao->esq->dir = criarSala("Sala de Jantar", "Taça quebrada");

    // Lado Direito da Mansão
    mansao->dir = criarSala("Biblioteca", "Livro sobre venenos aberto");
    mansao->dir->esq = criarSala("Escritório", "Bilhete ameaçador");
    mansao->dir->dir = criarSala("Jardim de Inverno", "Vaso revirado");

    // Inicializa a árvore de pistas (BST) como vazia
    PistaNode* minhasPistas = NULL;

    printf("=== DETECTIVE QUEST: AVENTURA NA MANSÃO ===\n");
    printf("Bem-vindo, detetive. Explore a mansão para coletar pistas.\n\n");

    // 2. Inicia a exploração
    // Passamos o endereço de minhasPistas (&minhasPistas) para poder atualizar a raiz da BST
    explorarSalasComPistas(mansao, &minhasPistas);

    // 3. Exibe o relatório final
    printf("\n=========================================\n");
    printf("FIM DA JORNADA. RELATÓRIO DE PISTAS:\n");
    printf("=========================================\n");
    
    if (minhasPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(minhasPistas);
    }
    printf("=========================================\n");

    // Nota: Em um programa real, deveríamos ter funções para liberar a memória (free) aqui.
    
    return 0;
}

// --- Implementação das Funções ---

/*
 * Função: criarSala
 * Objetivo: Cria dinamicamente um cômodo da mansão.
 * Parâmetros: Nome da sala e a pista associada.
 * Retorno: Ponteiro para a nova sala criada.
 */
Sala* criarSala(char* nome, char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esq = NULL;
    novaSala->dir = NULL;
    return novaSala;
}

/*
 * Função: inserirPista
 * Objetivo: Insere uma nova pista na árvore BST em ordem alfabética.
 * Parâmetros: Raiz da árvore de pistas e o texto da nova pista.
 * Retorno: Raiz atualizada da árvore de pistas.
 */
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    // Caso base: a árvore está vazia ou chegamos numa folha
    if (raiz == NULL) {
        PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novoNo->conteudo, pista);
        novoNo->esq = NULL;
        novoNo->dir = NULL;
        return novoNo;
    }

    // Lógica da BST: Compara strings para decidir lado (Ordem Alfabética)
    // strcmp < 0: pista vem antes da raiz -> Esquerda
    // strcmp > 0: pista vem depois da raiz -> Direita
    // strcmp == 0: pista já existe -> Não faz nada (evita duplicatas)
    
    int comparacao = strcmp(pista, raiz->conteudo);

    if (comparacao < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (comparacao > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    
    return raiz;
}

/*
 * Função: exibirPistas
 * Objetivo: Imprime a árvore de pistas usando percurso In-Order (Em Ordem).
 * Isso garante que as strings saiam em ordem alfabética.
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);              // Visita filho esquerdo
        printf("- %s\n", raiz->conteudo);     // Visita raiz (imprime pista)
        exibirPistas(raiz->dir);              // Visita filho direito
    }
}

/*
 * Função: explorarSalasComPistas
 * Objetivo: Controla o loop de jogo, navegação entre salas e coleta automática.
 * Parâmetros: Nó atual do mapa e ponteiro para a raiz da árvore de pistas.
 */
void explorarSalasComPistas(Sala* salaAtual, PistaNode** inventarioPistas) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\n--- Você está em: %s ---\n", salaAtual->nome);

        // Verifica e coleta pista automaticamente
        if (strlen(salaAtual->pista) > 0) {
            printf("[!] Pista encontrada: \"%s\"\n", salaAtual->pista);
            // Insere na BST (atualiza o ponteiro da raiz das pistas)
            *inventarioPistas = inserirPista(*inventarioPistas, salaAtual->pista);
        } else {
            printf("(Não há pistas visíveis neste cômodo)\n");
        }

        // Menu de navegação
        printf("Para onde deseja ir? [e]squerda, [d]ireita ou [s]air: ");
        scanf(" %c", &opcao); // Espaço antes de %c para limpar buffer

        if (opcao == 's' || opcao == 'S') {
            printf("Encerrando exploração...\n");
            break;
        } else if (opcao == 'e' || opcao == 'E') {
            if (salaAtual->esq != NULL) {
                salaAtual = salaAtual->esq;
            } else {
                printf("\n>>> Caminho bloqueado ou sem sala à esquerda!\n");
            }
        } else if (opcao == 'd' || opcao == 'D') {
            if (salaAtual->dir != NULL) {
                salaAtual = salaAtual->dir;
            } else {
                printf("\n>>> Caminho bloqueado ou sem sala à direita!\n");
            }
        } else {
            printf("\n>>> Opção inválida.\n");
        }
    }
}