#ifndef HISTORICO_H
#define HISTORICO_H

#define MAX_SESSOES    100
#define MAX_TENTATIVAS 10
#define ARQUIVO_LOG    "historico.txt"

typedef struct {
    char caso_nome[50];
    char dificuldade[10];
    int  secreto;
    int  tentativas_usadas;
    int  palpites[MAX_TENTATIVAS];
    int  venceu;
} Sessao;

void salvar_sessao(Sessao s);
int  carregar_historico(Sessao *historico);

#endif