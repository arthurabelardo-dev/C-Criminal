#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void limpar_tela(void) {
    system(CLEAR);
}

void pausar(void) {
    printf("\n  Pressione ENTER para continuar...");
    getchar();
}

int ler_opcao(int min, int max) {
    int opcao;
    while (scanf("%d", &opcao) != 1 || opcao < min || opcao > max) {
        while (getchar() != '\n');
        printf(VERMELHO "  Opcao invalida. Tente novamente: " RESET);
    }
    while (getchar() != '\n');
    return opcao;
}