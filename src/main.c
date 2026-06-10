#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "jogo.h"
#include "historico.h"
#include "tui.h"

static void removerQuebraLinha(char *texto) {
    texto[strcspn(texto, "\r\n")] = '\0';
}

static int lerOpcaoMenuPrincipal(void) {
    char comando[80];

    while (1) {
        if (fgets(comando, sizeof(comando), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        removerQuebraLinha(comando);

        if (strcmp(comando, "historico") == 0) {
            return 4;
        }
        for (int opcao = 1; opcao <= 6; opcao++) {
            if (comando[0] == (char)('0' + opcao) && comando[1] == '\0') {
                return opcao;
            }
        }

        printf(VERMELHO "  Entrada invalida. Digite 1 a 6 ou historico: " RESET);
    }
}

/*
 * lerOpcaoMenuPrincipal:
 * - Lê uma entrada livre do usuário no menu principal. Aceita os dígitos '1'..'6'
 *   ou o comando textual "historico" para acesso rápido.
 * - Faz uma validação simples do formato da string lida e repete até receber
 *   um comando válido.
 */

static int autenticarUsuario(void) {
    const char *usuarioEsperado = "detetive";
    const char *senhaEsperada = "1234";
    int tentativas = 3;

    while (tentativas > 0) {
        char usuario[64];
        char senha[64];

        limparTela();
        printf("\n");
        uiLogo();
        printf("\n");
        uiBanner("C-Criminal // TERMINAL FORENSE", "Departamento de Investigacao - acesso restrito");
        uiStamp("CANAL CRIPTOGRAFADO", "VERSAO 2.0", UI_DIM);
        printf("\n");
        uiBoxTop();
        uiBoxMid("Terminal", "conectado ao servidor de evidencias", UI_CYAN);
        uiBoxMid("Usuario padrao", "detetive", UI_WHITE);
        uiBoxMid("Tentativas", tentativas == 1 ? "ultima tentativa" : "restantes", UI_YELLOW);
        uiBoxBottom();

        uiPrompt("USUARIO");
        if (fgets(usuario, sizeof(usuario), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        removerQuebraLinha(usuario);

        uiPrompt("SENHA");
        if (fgets(senha, sizeof(senha), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        removerQuebraLinha(senha);

        printf("\n");
        uiLoading("Validando credencial", 18, 20);
        uiLoading("Sincronizando banco de evidencias", 18, 16);

        if (strcmp(usuario, usuarioEsperado) == 0 && strcmp(senha, senhaEsperada) == 0) {
            uiAlert("ACESSO", "Credencial liberada. Boa cacada, Detetive.", UI_GREEN);
            uiPause("Pressione ENTER para continuar...");
            return 1;
        }

        tentativas--;
        uiAlert("FALHA", "Usuario ou senha invalidos.", UI_RED);
        if (tentativas > 0) {
            uiPause("Pressione ENTER para tentar novamente...");
        }
    }

    limparTela();
    printf("\n");
    uiBanner("ACESSO BLOQUEADO", "Muitas tentativas invalidas");
    uiAlert("SISTEMA", "Terminal bloqueado por seguranca.", UI_RED);
    uiPause("Pressione ENTER para encerrar...");
    return 0;
}

/*
 * autenticarUsuario:
 * - Fluxo simples de autenticação local com três tentativas.
 * - Mostra banners e mensagens de UI e bloqueia o acesso após esgotar
 *   as tentativas, para simular um mecanismo basico de segurança.
 */

static void exibirMenuPrincipal(void) {
    limparTela();
    printf("\n");
    uiLogo();
    printf("\n");
    uiBanner("DETETIVE DO TERMINAL", "Central de casos ativos");
    uiStamp("PLANTAO FORENSE", "3 CASOS ABERTOS", UI_DIM);
    uiSection("MURAL DE INVESTIGACOES", UI_CYAN);

    uiBoxTop();
    uiMenuItem(1, "Magnata", "Cofre termico", "FACIL", UI_GREEN);
    uiMenuItem(2, "Cassino", "Frequencia de fuga", "MEDIO", UI_YELLOW);
    uiMenuItem(3, "Apocalipse", "Porta infectada", "DIFICIL", UI_RED);
    uiMenuItem(4, "Dossie", "Historico e mentoria", "ARQUIVO", UI_CYAN);
    uiMenuItem(5, "Loja", "Itens estrategicos", "PRE-CASO", UI_MAGENTA);
    uiMenuItem(6, "Sair", "Encerrar terminal", "OFFLINE", UI_DIM);
    uiBoxBottom();
    printf("  %sComando rapido:%s historico\n", UI_DIM, UI_RESET);
    uiPrompt("COMANDO");
}

static void iniciarCaso(int idCaso) {
    if (confirmarCaso(idCaso)) {
        jogarPartida(idCaso);
    }
}

int main(void) {
    /*
     * main: inicializa a UI, semente do RNG e autentica o usuario.
     * - Se autenticacao bem-sucedida, exibe o menu principal em loop até
     *   o jogador optar por sair (opcao 6). As acoes redirecionam para
     *   funcoes responsaveis por cada fluxo (caso, historico, loja).
     */
    int opcao;

    uiInit();
    srand((unsigned int)time(NULL));

    if (!autenticarUsuario()) {
        return 1;
    }

    do {
        exibirMenuPrincipal();
        opcao = lerOpcaoMenuPrincipal();

        switch (opcao) {
            case 1:
            case 2:
            case 3:
                iniciarCaso(opcao);
                break;
            case 4:
                exibirHistorico();
                break;
            case 5:
                exibirLoja();
                break;
            case 6:
                limparTela();
                printf("\n");
                uiBanner("ENCERRANDO SISTEMA", "Arquivando sessoes locais");
                uiLoading("Desconectando terminal", 20, 18);
                printf("\n  Ate a proxima investigacao, Detetive.\n\n");
                break;
        }
    } while (opcao != 6);

    return 0;
}
