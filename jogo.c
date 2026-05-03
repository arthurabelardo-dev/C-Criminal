#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jogo.h"
#include "historico.h"
#include "utils.h"

static Caso casos[3] = {
    { "O Ultimo Suspiro do Magnata",   "TODO: tema do caso 1", 1,  50,  7, "Facil"   },
    { "Frequencia de Fuga no Cassino", "TODO: tema do caso 2", 1,  100, 6, "Medio"   },
    { "Protocolo Apocalipse",          "TODO: tema do caso 3", 1,  200, 5, "Dificil" }
};

void exibir_cabecalho(Caso c, int tentativas_restantes) {
}

char *classificar_pontuacao(int pontos) {
    return "Detetive";
}

int validar_palpite(int min, int max) {
    return 0;
}

void dar_feedback(Caso c, int palpite, int secreto) {
}

void jogar_partida(void) {
}