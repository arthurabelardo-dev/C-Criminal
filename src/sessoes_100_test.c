#include <stdio.h>
#include "historico.h"

#define TOTAL_SESSOES_TESTE 120
#define CAMINHO_HISTORICO "evidencias/historico-120-sessoes.txt"
#define CAMINHO_RELATORIO "evidencias/relatorio-100-sessoes.md"

static int gerarHistoricoTeste(void) {
    FILE *arquivo = fopen(CAMINHO_HISTORICO, "w");

    if (arquivo == NULL) {
        printf("FALHA ao criar %s\n", CAMINHO_HISTORICO);
        return 0;
    }

    for (int i = 0; i < TOTAL_SESSOES_TESTE; i++) {
        int casoId = (i % 3) + 1;
        int venceu = (i % 5) != 0;
        int tentativas = (i % 7) + 1;
        int pistasAntesChute = i % 4;
        int momentoChute = (i % 6) + 1;
        int recompensa = venceu ? 80 + (i % 40) : 0;
        long timestamp = 1717200000L + ((long)i * 300L);
        const char *casoNome = casoId == 1 ? "O Ultimo Suspiro do Magnata" :
                               casoId == 2 ? "Frequencia de Fuga no Cassino" :
                                             "Protocolo Apocalipse";
        const char *dificuldade = casoId == 1 ? "Facil" :
                                  casoId == 2 ? "Medio" : "Dificil";

        fprintf(arquivo, "HU09\n%d\n%s\n%s\n%d\n%s\n%d\n%d\n%d\n%d\n%ld\n",
                casoId,
                casoNome,
                dificuldade,
                10 + i,
                venceu ? "RESOLVIDO" : "FALHOU",
                tentativas,
                pistasAntesChute,
                momentoChute,
                recompensa,
                timestamp);
    }

    fclose(arquivo);
    return 1;
}

static int gerarRelatorio(int totalValidado) {
    FILE *relatorio = fopen(CAMINHO_RELATORIO, "w");

    if (relatorio == NULL) {
        printf("FALHA ao criar %s\n", CAMINHO_RELATORIO);
        return 0;
    }

    fprintf(relatorio, "# Evidencia de Teste com 100+ Sessoes\n\n");
    fprintf(relatorio, "- Arquivo de historico: `%s`\n", CAMINHO_HISTORICO);
    fprintf(relatorio, "- Sessoes geradas: %d\n", TOTAL_SESSOES_TESTE);
    fprintf(relatorio, "- Sessoes validadas pela leitura do historico: %d\n", totalValidado);
    fprintf(relatorio, "- Criterio: minimo de 100 sessoes registradas e legiveis.\n");
    fprintf(relatorio, "- Resultado: %s\n", totalValidado >= 100 ? "APROVADO" : "REPROVADO");

    fclose(relatorio);
    return 1;
}

int main(void) {
    int totalValidado;

    if (!gerarHistoricoTeste()) {
        return 1;
    }

    totalValidado = contarSessoesHistorico(CAMINHO_HISTORICO);
    if (!gerarRelatorio(totalValidado)) {
        return 1;
    }

    if (totalValidado < 100) {
        printf("FALHA teste 100+ sessoes: total validado=%d\n", totalValidado);
        return 1;
    }

    printf("OK teste 100+ sessoes: %d sessoes validadas em %s\n",
           totalValidado, CAMINHO_HISTORICO);
    return 0;
}
