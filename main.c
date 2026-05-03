#include <stdio.h>
#include "utils.h"
#include "jogo.h"
#include "historico.h"

static void exibir_menu(void) {
    limpar_tela();
    printf(CIANO);
    printf("╔══════════════════════════════════════╗\n");
    printf("║        C-CRIMINAL INVESTIGACOES      ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf(RESET);
    printf("\n  1. Iniciar nova investigacao\n");
    printf("  2. Analisar historico\n");
    printf("  3. Sair\n\n");
    printf("  Escolha: ");
}

int main(void) {
    int opcao;
    do {
        exibir_menu();
        opcao = ler_opcao(1, 3);
        switch (opcao) {
            case 1: jogar_partida(); break;
            case 2: break;
            case 3:
                limpar_tela();
                printf(VERDE "\n  Encerrando. Ate a proxima, detetive.\n\n" RESET);
                break;
        }
    } while (opcao != 3);

    return 0;
}