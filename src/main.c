#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "jogo.h"
#include "historico.h"

static void tela_login(void) {
    limpar_tela();
    printf("\n");
    printf(CIANO "  ============================================================\n" RESET);
    printf("               SISTEMA DE INVESTIGACAO CRIMINAL\n");
    printf("         DEPARTAMENTO DE INVESTIGACAO - ACESSO RESTRITO\n");
    printf(CIANO "  ============================================================\n\n" RESET);
    printf("  USUARIO: 1\n\n");
    printf(VERDE "  senha: " RESET);
    
    char senha_falsa[50];
    scanf("%s", senha_falsa);
    while(getchar() != '\n');
}

static void exibir_menu(void) {
    limpar_tela();
    printf("\n");
    printf("  ============================================================\n");
    printf(CIANO "             ||||| DETETIVE DO TERMINAL |||||\n" RESET);
    printf(AMARELO "          *** SISTEMA DE INVESTIGACAO CRIMINAL ***\n\n" RESET);
    printf("  Novos casos aguardam sua atencao.\n\n");
    printf("  Selecione uma investigacao:\n\n");
    
    printf("  [1] O Ultimo Suspiro do Magnata    " VERDE "[Facil]\n" RESET);
    printf("  [2] Frequencia de Fuga no Cassino  " AMARELO "[Medio]\n" RESET);
    printf("  [3] Protocolo Apocalipse           " VERMELHO "[Dificil]\n\n" RESET);
    printf("  [4] Ver meu dossie\n");
    printf("  [5] Sair\n\n");
    printf("  > ");
}

int main(void) {
    srand(time(NULL)); 
    tela_login();

    int opcao;
    do {
        exibir_menu();
        opcao = ler_opcao(1, 5);

        switch (opcao) {
            case 1: jogar_partida(1); break;
            case 2: jogar_partida(2); break;
            case 3: jogar_partida(3); break;
            case 4: exibir_historico(); break;
            case 5:
                limpar_tela();
                printf("\n  Encerrando o sistema...\n\n");
                printf("  Ate a proxima investigacao, Detetive.\n\n");
                break;
        }
    } while (opcao != 5);

    return 0;
}