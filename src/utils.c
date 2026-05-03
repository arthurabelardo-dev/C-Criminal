#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void limpar_tela(void) {
    system("clear");
}

void pausar(void) {
    printf("\nDigite ENTER para continuar...");
    getchar();
}

int ler_opcao(int min, int max) {
    int opcao = 0;
    do {
        scanf("%d", &opcao);
        while(getchar() != '\n'); 
        
        if (opcao < min || opcao > max) {
            printf(VERMELHO "Opcao invalida. Tente novamente: " RESET);
        }
    } while (opcao < min || opcao > max);
    
    return opcao;
}