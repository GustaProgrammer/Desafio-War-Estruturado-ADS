// Desafio Mestre WAR Estruturado Tema 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_TERRITORIOS 10
#define TOTAL_MISSOES 5
#define TOTAL_JOGADORES 2

// Estrutura que representa um território
typedef struct {
    char nome[30];
    char continente[20];
    char cor[10]; // dono do território
    int tropas;
} Territorio;

// Funções
void cadastrarTerritorios(Territorio *mapa);
void exibirMapa(Territorio *mapa, int tamanho);
void atacar(Territorio *atacante, Territorio *defensor);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador);
void liberarMemoria(Territorio *mapa, char **missoesJogadores);
void exibirMissao(char *missao);

// Função principal
int main() {
    srand((unsigned)time(NULL));

    int i;
    Territorio *mapa = malloc(TOTAL_TERRITORIOS * sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    // Cria vetor de missões
    char *missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territórios seguidos.",
        "Eliminar todos os territórios da cor Vermelha.",
        "Conquistar toda a América.",
        "Dominar 5 territórios com pelo menos 3 tropas.",
        "Conquistar 2 continentes completos."
    };

    // Alocação de missões dos jogadores
    char **missoesJogadores = malloc(TOTAL_JOGADORES * sizeof(char*));
    if (!missoesJogadores) {
        free(mapa);
        fprintf(stderr, "Falha ao alocar memória para missões.\n");
        return 1;
    }
    for (i = 0; i < TOTAL_JOGADORES; i++) {
        missoesJogadores[i] = malloc(100 * sizeof(char));
        if (!missoesJogadores[i]) {
            for (int j = 0; j < i; j++) free(missoesJogadores[j]);
            free(missoesJogadores);
            free(mapa);
            fprintf(stderr, "Falha ao alocar memória para missão do jogador %d.\n", i+1);
            return 1;
        }
    }

    // Cadastro de territórios fixos (continentes)
    cadastrarTerritorios(mapa);

    // Sorteio de missões
    for (i = 0; i < TOTAL_JOGADORES; i++) {
        atribuirMissao(missoesJogadores[i], missoes, TOTAL_MISSOES);
        printf("\nJogador %d - Sua missão: ", i + 1);
        exibirMissao(missoesJogadores[i]);
    }

    printf("\n==== INÍCIO DO JOGO ====\n");
    int rodada = 1;
    char linha[128];
    while (1) {
        printf("\n--- Rodada %d ---\n", rodada);
        exibirMapa(mapa, TOTAL_TERRITORIOS);

        // Escolha de ataque (entrada segura)
        int atacante, defensor;
        printf("\nEscolha o território atacante (0 a %d): ", TOTAL_TERRITORIOS - 1);
        if (!fgets(linha, sizeof(linha), stdin) || sscanf(linha, "%d", &atacante) != 1) {
            printf("Entrada inválida.\n");
            continue;
        }
        printf("Escolha o território defensor (0 a %d): ", TOTAL_TERRITORIOS - 1);
        if (!fgets(linha, sizeof(linha), stdin) || sscanf(linha, "%d", &defensor) != 1) {
            printf("Entrada inválida.\n");
            continue;
        }

        if (atacante == defensor || atacante < 0 || defensor < 0 || atacante >= TOTAL_TERRITORIOS || defensor >= TOTAL_TERRITORIOS) {
            printf("\nAtaque inválido!\n");
            continue;
        }

        if (mapa[atacante].tropas < 2) { // exigência mínima para atacar
            printf("\nTerritório atacante precisa ter pelo menos 2 tropas para atacar.\n");
            continue;
        }

        atacar(&mapa[atacante], &mapa[defensor]);

        // Verificação de missão de cada jogador
        for (i = 0; i < TOTAL_JOGADORES; i++) {
            char corJogador[10];
            strncpy(corJogador, (i == 0) ? "Azul" : "Vermelho", sizeof(corJogador)-1);
            corJogador[sizeof(corJogador)-1] = '\0';

            if (verificarMissao(missoesJogadores[i], mapa, TOTAL_TERRITORIOS, corJogador)) {
                printf("\n🎉 Jogador %d (%s) cumpriu sua missão! VITÓRIA! 🎉\n", i + 1, corJogador);
                liberarMemoria(mapa, missoesJogadores);
                return 0;
            }
        }

        rodada++;
    }

    liberarMemoria(mapa, missoesJogadores);
    return 0;
}

// Função para cadastrar territórios já definidos por continente
void cadastrarTerritorios(Territorio *mapa) {
    strncpy(mapa[0].nome, "Brasil", sizeof(mapa[0].nome)-1); mapa[0].nome[sizeof(mapa[0].nome)-1]='\0';
    strncpy(mapa[0].continente, "América", sizeof(mapa[0].continente)-1); mapa[0].continente[sizeof(mapa[0].continente)-1]='\0';
    strncpy(mapa[0].cor, "Azul", sizeof(mapa[0].cor)-1); mapa[0].cor[sizeof(mapa[0].cor)-1]='\0'; mapa[0].tropas = 5;

    strncpy(mapa[1].nome, "Argentina", sizeof(mapa[1].nome)-1); mapa[1].nome[sizeof(mapa[1].nome)-1]='\0';
    strncpy(mapa[1].continente, "América", sizeof(mapa[1].continente)-1); mapa[1].continente[sizeof(mapa[1].continente)-1]='\0';
    strncpy(mapa[1].cor, "Vermelho", sizeof(mapa[1].cor)-1); mapa[1].cor[sizeof(mapa[1].cor)-1]='\0'; mapa[1].tropas = 3;

    strncpy(mapa[2].nome, "Canadá", sizeof(mapa[2].nome)-1); mapa[2].nome[sizeof(mapa[2].nome)-1]='\0';
    strncpy(mapa[2].continente, "América", sizeof(mapa[2].continente)-1); mapa[2].continente[sizeof(mapa[2].continente)-1]='\0';
    strncpy(mapa[2].cor, "Azul", sizeof(mapa[2].cor)-1); mapa[2].cor[sizeof(mapa[2].cor)-1]='\0'; mapa[2].tropas = 4;

    strncpy(mapa[3].nome, "Espanha", sizeof(mapa[3].nome)-1); mapa[3].nome[sizeof(mapa[3].nome)-1]='\0';
    strncpy(mapa[3].continente, "Europa", sizeof(mapa[3].continente)-1); mapa[3].continente[sizeof(mapa[3].continente)-1]='\0';
    strncpy(mapa[3].cor, "Vermelho", sizeof(mapa[3].cor)-1); mapa[3].cor[sizeof(mapa[3].cor)-1]='\0'; mapa[3].tropas = 2;

    strncpy(mapa[4].nome, "França", sizeof(mapa[4].nome)-1); mapa[4].nome[sizeof(mapa[4].nome)-1]='\0';
    strncpy(mapa[4].continente, "Europa", sizeof(mapa[4].continente)-1); mapa[4].continente[sizeof(mapa[4].continente)-1]='\0';
    strncpy(mapa[4].cor, "Azul", sizeof(mapa[4].cor)-1); mapa[4].cor[sizeof(mapa[4].cor)-1]='\0'; mapa[4].tropas = 3;

    strncpy(mapa[5].nome, "Alemanha", sizeof(mapa[5].nome)-1); mapa[5].nome[sizeof(mapa[5].nome)-1]='\0';
    strncpy(mapa[5].continente, "Europa", sizeof(mapa[5].continente)-1); mapa[5].continente[sizeof(mapa[5].continente)-1]='\0';
    strncpy(mapa[5].cor, "Vermelho", sizeof(mapa[5].cor)-1); mapa[5].cor[sizeof(mapa[5].cor)-1]='\0'; mapa[5].tropas = 4;

    strncpy(mapa[6].nome, "Egito", sizeof(mapa[6].nome)-1); mapa[6].nome[sizeof(mapa[6].nome)-1]='\0';
    strncpy(mapa[6].continente, "África", sizeof(mapa[6].continente)-1); mapa[6].continente[sizeof(mapa[6].continente)-1]='\0';
    strncpy(mapa[6].cor, "Azul", sizeof(mapa[6].cor)-1); mapa[6].cor[sizeof(mapa[6].cor)-1]='\0'; mapa[6].tropas = 2;

    strncpy(mapa[7].nome, "Nigéria", sizeof(mapa[7].nome)-1); mapa[7].nome[sizeof(mapa[7].nome)-1]='\0';
    strncpy(mapa[7].continente, "África", sizeof(mapa[7].continente)-1); mapa[7].continente[sizeof(mapa[7].continente)-1]='\0';
    strncpy(mapa[7].cor, "Vermelho", sizeof(mapa[7].cor)-1); mapa[7].cor[sizeof(mapa[7].cor)-1]='\0'; mapa[7].tropas = 3;

    strncpy(mapa[8].nome, "China", sizeof(mapa[8].nome)-1); mapa[8].nome[sizeof(mapa[8].nome)-1]='\0';
    strncpy(mapa[8].continente, "Ásia", sizeof(mapa[8].continente)-1); mapa[8].continente[sizeof(mapa[8].continente)-1]='\0';
    strncpy(mapa[8].cor, "Azul", sizeof(mapa[8].cor)-1); mapa[8].cor[sizeof(mapa[8].cor)-1]='\0'; mapa[8].tropas = 4;

    strncpy(mapa[9].nome, "Austrália", sizeof(mapa[9].nome)-1); mapa[9].nome[sizeof(mapa[9].nome)-1]='\0';
    strncpy(mapa[9].continente, "Oceania", sizeof(mapa[9].continente)-1); mapa[9].continente[sizeof(mapa[9].continente)-1]='\0';
    strncpy(mapa[9].cor, "Vermelho", sizeof(mapa[9].cor)-1); mapa[9].cor[sizeof(mapa[9].cor)-1]='\0'; mapa[9].tropas = 2;
}

// Exibe o mapa completo
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("[%d] %-10s | %-10s | %-10s | Tropas: %d\n", i, mapa[i].nome, mapa[i].continente, mapa[i].cor, mapa[i].tropas);
    }
}

// Função de ataque
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nNão é possível atacar seu próprio território!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nBatalha entre %s (%s) e %s (%s)!\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Vitória do atacante! %s conquistou %s!\n", atacante->cor, defensor->nome);
        // transfere tropas do atacante para o defensor garantindo ao menos 1 remanescente
        int movidas = atacante->tropas / 2;
        if (movidas < 1) movidas = 1;
        if (movidas > atacante->tropas - 1) movidas = atacante->tropas - 1;
        if (movidas < 1) movidas = 1; // proteção adicional

        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor)-1);
        defensor->cor[sizeof(defensor->cor)-1] = '\0';
        defensor->tropas = movidas;
        atacante->tropas -= movidas;
        if (atacante->tropas < 1) atacante->tropas = 1;
    } else {
        printf("Defensor resistiu! %s perdeu uma tropa.\n", atacante->nome);
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

// Sorteia e atribui uma missão
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strncpy(destino, missoes[sorteio], 99);
    destino[99] = '\0';
}

// Exibe a missão do jogador
void exibirMissao(char *missao) {
    printf("%s\n", missao);
}

// Verifica se a missão foi cumprida (versão ampliada)
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    int cont = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0)
            cont++;
    }
    if (strstr(missao, "3 territórios") && cont >= 3)
        return 1;
    if (strstr(missao, "5 territórios") && cont >= 5)
        return 1;
    if (strstr(missao, "toda a América")) {
        int am_total = 0, am_owned = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].continente, "América") == 0) {
                am_total++;
                if (strcmp(mapa[i].cor, corJogador) == 0) am_owned++;
            }
        }
        if (am_total > 0 && am_owned == am_total) return 1;
    }
    if (strstr(missao, "Eliminar") && strstr(missao, "Vermelha")) {
        int verm = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "Vermelho") == 0) verm++;
        if (verm == 0) return 1;
    }
    if (strstr(missao, "2 continentes")) {
        // conta continentes totalmente dominados pelo jogador
        char nomes[TOTAL_TERRITORIOS][20];
        int totalPorCont[TOTAL_TERRITORIOS];
        int ownedPorCont[TOTAL_TERRITORIOS];
        int uniq = 0;
        for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
            int idx = -1;
            for (int j = 0; j < uniq; j++)
                if (strcmp(nomes[j], mapa[i].continente) == 0) { idx = j; break; }
            if (idx == -1) {
                strncpy(nomes[uniq], mapa[i].continente, sizeof(nomes[uniq])-1);
                nomes[uniq][sizeof(nomes[uniq])-1] = '\0';
                totalPorCont[uniq] = 1;
                ownedPorCont[uniq] = (strcmp(mapa[i].cor, corJogador) == 0) ? 1 : 0;
                uniq++;
            } else {
                totalPorCont[idx]++;
                if (strcmp(mapa[i].cor, corJogador) == 0) ownedPorCont[idx]++;
            }
        }
        int completos = 0;
        for (int j = 0; j < uniq; j++)
            if (ownedPorCont[j] == totalPorCont[j]) completos++;
        if (completos >= 2) return 1;
    }
    return 0;
}

// Libera memória alocada dinamicamente
void liberarMemoria(Territorio *mapa, char **missoesJogadores) {
    free(mapa);
    for (int i = 0; i < TOTAL_JOGADORES; i++)
        free(missoesJogadores[i]);
    free(missoesJogadores);
}
