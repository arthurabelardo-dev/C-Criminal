#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "historico.h"
#include "utils.h"

static int lerLinha(FILE *f, char *destino, size_t tamanho) {
    if (fgets(destino, (int)tamanho, f) == NULL) {
        return 0;
    }

    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

static int parseInt(const char *texto, int *saida) {
    char *fim = NULL;
    long valor = strtol(texto, &fim, 10);

    if (fim == texto || *fim != '\0' || valor < INT_MIN || valor > INT_MAX) {
        return 0;
    }

    *saida = (int)valor;
    return 1;
}

void salvarSessao(Sessao s) {
    FILE *f = fopen("historico.txt", "a");

    if (f == NULL) {
        perror("Erro ao abrir historico.txt");
        return;
    }

    if (fprintf(f, "%s\n%s\n%d\n%d\n%d\n",
                s.casoNome,
                s.dificuldade,
                s.secreto,
                s.tentativasUsadas,
                s.venceu) < 0) {
        perror("Erro ao escrever historico.txt");
    }

    fclose(f);
}

void exibirHistorico(void) {
    FILE *f = fopen("historico.txt", "r");
    
    limparTela();
    printf(CIANO "--- DOSSIE COMPLETO ---\n" RESET);
    
    if (f == NULL) {
        printf("Nenhum historico encontrado. Jogue uma partida primeiro.\n");
    } else {
        Sessao s;
        char buffer[50];
        int arquivoCorrompido = 0;

        while (lerLinha(f, s.casoNome, sizeof(s.casoNome))) {
            if (!lerLinha(f, s.dificuldade, sizeof(s.dificuldade)) ||
                !lerLinha(f, buffer, sizeof(buffer)) ||
                !parseInt(buffer, &s.secreto) ||
                !lerLinha(f, buffer, sizeof(buffer)) ||
                !parseInt(buffer, &s.tentativasUsadas) ||
                !lerLinha(f, buffer, sizeof(buffer)) ||
                !parseInt(buffer, &s.venceu)) {
                arquivoCorrompido = 1;
                break;
            }

            printf("Caso: %-30s | Nivel: %-7s | Alvo: %3d | Tentativas: %2d | Status: ",
                   s.casoNome, s.dificuldade, s.secreto, s.tentativasUsadas);

            if (s.venceu == 1) {
                printf(VERDE "RESOLVIDO\n" RESET);
            } else {
                printf(VERMELHO "FALHOU\n" RESET);
            }
        }

        if (arquivoCorrompido) {
            printf(AMARELO "Historico com formato invalido. Corrija ou limpe o arquivo.\n" RESET);
        }

        fclose(f);
    }
    printf(CIANO "-----------------------\n" RESET);
    pausar();
}
