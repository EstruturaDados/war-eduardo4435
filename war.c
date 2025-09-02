#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes
#define MAX_NOME 30
#define MAX_COR 10
#define QTD_TERRITORIOS 5
#define QTD_MISSOES 5

// Estrutura para representar um território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Estrutura para representar um jogador
typedef struct {
    char* missao;  // Alocada dinamicamente
} Jogador;

// Vetor de missões pré-definidas
char* missoesPredefinidas[QTD_MISSOES] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor vermelha",
    "Manter pelo menos 2 territorios com mais de 5 tropas",
    "Conquistar um territorio de cada cor",
    "Manter todos os seus territorios com mais de 3 tropas"
};

// Função para limpar buffer após leitura com scanf
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para cadastrar os territórios
void cadastrarTerritorios(Territorio *mapa) {
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, MAX_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

// Função para exibir o mapa
void exibirMapa(const Territorio *mapa) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf(" - Nome: %s\n", mapa[i].nome);
        printf(" - Cor: %s\n", mapa[i].cor);
        printf(" - Tropas: %d\n", mapa[i].tropas);
        printf("-------------------------\n");
    }
}

// Função para simular ataque entre dois territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Nao pode atacar um territorio da mesma cor!\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("Nao ha tropas suficientes para atacar (precisa de pelo menos 2).\n");
        return;
    }

    printf("\nBatalha entre %s (Atacante) e %s (Defensor)\n", atacante->nome, defensor->nome);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Atacante (%s) tirou: %d\n", atacante->nome, dadoAtacante);
    printf("Defensor (%s) tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Atacante venceu! %s agora pertence a %s\n", defensor->nome, atacante->cor);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("Defensor venceu! Atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
    }
}

// Função para atribuir missão ao jogador
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    *destino = malloc(strlen(missoes[sorteio]) + 1);
    if (*destino == NULL) {
        printf("Erro ao alocar memoria para missao.\n");
        exit(1);
    }
    strcpy(*destino, missoes[sorteio]);
}

// Exibir missão do jogador
void exibirMissao(const char* missao) {
    printf("\nSua missao e: %s\n", missao);
}

// Função para verificar se a missão foi cumprida
int verificarMissao(const char* missao, Territorio* mapa) {
    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {
        int cont = 1;
        for (int i = 1; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, mapa[i - 1].cor) == 0) {
                cont++;
                if (cont >= 3) return 1;
            } else {
                cont = 1;
            }
        }
    } else if (strcmp(missao, "Manter todos os seus territorios com mais de 3 tropas") == 0) {
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (mapa[i].tropas <= 3) return 0;
        }
        return 1;
    }
    return 0;
}

// Função para liberar memória
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores) {
    free(mapa);
    for (int i = 0; i < qtdJogadores; i++) {
        free(jogadores[i].missao);
    }
    free(jogadores);
}

int main() {
    srand(time(NULL));

    // Alocação dinâmica para os territórios
    Territorio* mapa = calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    int qtdJogadores = 1; // Pode aumentar futuramente
    Jogador* jogadores = calloc(qtdJogadores, sizeof(Jogador));
    if (!jogadores) {
        printf("Erro ao alocar memoria para jogadores.\n");
        free(mapa);
        return 1;
    }

    // Cadastro inicial dos territórios
    cadastrarTerritorios(mapa);

    // Exibe o mapa antes de mostrar a missão
    exibirMapa(mapa);

    // Atribui e exibe a missão do jogador
    for (int i = 0; i < qtdJogadores; i++) {
        atribuirMissao(&jogadores[i].missao, missoesPredefinidas, QTD_MISSOES);
        printf("\nJogador %d", i + 1);
        exibirMissao(jogadores[i].missao);
    }

    // Loop para ataques
    int opcao;
    do {
        printf("\nDeseja realizar um ataque? (1 = sim, 0 = nao): ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int at, def;
            exibirMapa(mapa);

            printf("Escolha o numero do territorio atacante (1 a %d): ", QTD_TERRITORIOS);
            scanf("%d", &at);
            printf("Escolha o numero do territorio defensor (1 a %d): ", QTD_TERRITORIOS);
            scanf("%d", &def);
            limparBuffer();

            if (at >= 1 && at <= QTD_TERRITORIOS && def >= 1 && def <= QTD_TERRITORIOS && at != def) {
                atacar(&mapa[at - 1], &mapa[def - 1]);
            } else {
                printf("Escolha invalida!\n");
            }

            exibirMapa(mapa);

            // Verifica missão do jogador
            for (int i = 0; i < qtdJogadores; i++) {
                if (verificarMissao(jogadores[i].missao, mapa)) {
                    printf("\nJogador %d cumpriu sua missao e venceu o jogo!\n", i + 1);
                    opcao = 0;
                    break;
                }
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa, jogadores, qtdJogadores);
    printf("\nMemoria liberada. Fim do jogo.\n");

    return 0;
}
