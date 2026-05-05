#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static void descartarRestanteLinha(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void limparTela(void) {
    system(CLEAR);
}

void pausar(void) {
    printf("\nDigite ENTER para continuar...");
    fflush(stdout);
    descartarRestanteLinha();
}

int lerOpcao(int min, int max) {
    char linha[64];

    while (1) {
        long opcao = 0;
        char *fim = NULL;

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return min;
        }

        opcao = strtol(linha, &fim, 10);

        while (*fim == ' ' || *fim == '\t') {
            fim++;
        }

        if ((fim == linha) || (*fim != '\n' && *fim != '\0') || (opcao < min || opcao > max)) {
            printf(VERMELHO "Opcao invalida. Tente novamente: " RESET);
            continue;
        }

        return (int)opcao;
    }
}
