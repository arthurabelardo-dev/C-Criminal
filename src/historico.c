#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"
#include "utils.h"

void salvar_sessao(Sessao s) {
    FILE *f = fopen("historico.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s\n%s\n%d\n%d\n%d\n", s.caso_nome, s.dificuldade, s.secreto, s.tentativas_usadas, s.venceu);
        fclose(f);
    }
}

void exibir_historico(void) {
    FILE *f = fopen("historico.txt", "r");
    
    limpar_tela();
    printf(CIANO "--- DOSSIE COMPLETO ---\n" RESET);
    
    if (f == NULL) {
        printf("Nenhum historico encontrado. Jogue uma partida primeiro.\n");
    } else {
        Sessao s;
        char buffer[50];

        while (fgets(s.caso_nome, sizeof(s.caso_nome), f) != NULL) {
            
            s.caso_nome[strcspn(s.caso_nome, "\n")] = '\0';
            
            fgets(s.dificuldade, sizeof(s.dificuldade), f);
            s.dificuldade[strcspn(s.dificuldade, "\n")] = '\0';

            fgets(buffer, sizeof(buffer), f);
            s.secreto = atoi(buffer);

            fgets(buffer, sizeof(buffer), f);
            s.tentativas_usadas = atoi(buffer);

            fgets(buffer, sizeof(buffer), f);
            s.venceu = atoi(buffer);

            printf("Caso: %-30s | Nivel: %-7s | Alvo: %3d | Tentativas: %2d | Status: ", 
                   s.caso_nome, s.dificuldade, s.secreto, s.tentativas_usadas);
            
            if (s.venceu == 1) {
                printf(VERDE "RESOLVIDO\n" RESET);
            } else {
                printf(VERMELHO "FALHOU\n" RESET);
            }
        }
        fclose(f);
    }
    printf(CIANO "-----------------------\n" RESET);
    pausar();
}