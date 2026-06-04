#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "historico.h"
#include "utils.h"
#include "tui.h"

static int saldoCache = 0;
static int reputacaoCache = 50;
static int confiancaCache = 60;
static int itensCache[TOTAL_ITENS_LOJA] = {0};
static int saldoCarregado = 0;
static const char *itensNome[TOTAL_ITENS_LOJA] = {
    "Segunda Chance",
    "Analise Extra",
    "Scanner Forense",
    "Intuicao",
    "+2 Tentativas",
    "+1 Interrogatorio"
};
static const char *itensDescricao[TOTAL_ITENS_LOJA] = {
    "Ignora uma tentativa errada (uso unico por caso).",
    "Revela uma pista adicional confiavel (nunca falsa).",
    "Remove uma pista falsa ativa no caso (requer confianca >=50).",
    "Mostra intervalo de +-10 ao redor do alvo.",
    "Aumenta o limite de tentativas do caso em +2.",
    "Adiciona +1 slot de interrogatorio no caso."
};
static const int itensPreco[TOTAL_ITENS_LOJA] = {50, 30, 60, 80, 40, 25};

static int precoAjustadoPorConfianca(ItemLoja item) {
    int preco = itensPreco[item];
    if (confiancaCache >= 70) {
        return (preco * 80) / 100;
    }
    if (confiancaCache < 30) {
        return (preco * 120) / 100;
    }
    return preco;
}

static int somaTentativasRec(const Sessao *sessoes, int total) {
    if (total <= 0) {
        return 0;
    }
    return sessoes[total - 1].tentativasUsadas + somaTentativasRec(sessoes, total - 1);
}

static int minTentativasRec(const Sessao *sessoes, int total) {
    int anterior;

    if (total <= 1) {
        return sessoes[0].tentativasUsadas;
    }

    anterior = minTentativasRec(sessoes, total - 1);
    if (sessoes[total - 1].tentativasUsadas < anterior) {
        return sessoes[total - 1].tentativasUsadas;
    }
    return anterior;
}

static int maxTentativasRec(const Sessao *sessoes, int total) {
    int anterior;

    if (total <= 1) {
        return sessoes[0].tentativasUsadas;
    }

    anterior = maxTentativasRec(sessoes, total - 1);
    if (sessoes[total - 1].tentativasUsadas > anterior) {
        return sessoes[total - 1].tentativasUsadas;
    }
    return anterior;
}

static long long somaQuadradosTentativasRec(const Sessao *sessoes, int total) {
    long long valor;

    if (total <= 0) {
        return 0;
    }

    valor = sessoes[total - 1].tentativasUsadas;
    return (valor * valor) + somaQuadradosTentativasRec(sessoes, total - 1);
}

static double raizQuadradaNewton(double valor) {
    double aproximacao;

    if (valor <= 0.0) {
        return 0.0;
    }

    aproximacao = valor > 1.0 ? valor : 1.0;
    for (int i = 0; i < 24; i++) {
        aproximacao = 0.5 * (aproximacao + (valor / aproximacao));
    }

    return aproximacao;
}

static int lerSessao(FILE *f, Sessao *s) {
    char buffer[64];
    char primeiraLinha[64];

    memset(s, 0, sizeof(*s));

    if (fgets(primeiraLinha, sizeof(primeiraLinha), f) == NULL) {
        return 0;
    }
    primeiraLinha[strcspn(primeiraLinha, "\n")] = '\0';

    if (strcmp(primeiraLinha, "HU09") == 0 || strcmp(primeiraLinha, "HU08") == 0) {
        int formatoHu09 = strcmp(primeiraLinha, "HU09") == 0;

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            return 0;
        }
        s->casoId = atoi(buffer);

        if (fgets(s->casoNome, sizeof(s->casoNome), f) == NULL) {
            return 0;
        }
        s->casoNome[strcspn(s->casoNome, "\n")] = '\0';

        if (fgets(s->dificuldade, sizeof(s->dificuldade), f) == NULL) {
            return 0;
        }
        s->dificuldade[strcspn(s->dificuldade, "\n")] = '\0';

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            return 0;
        }
        s->secreto = atoi(buffer);

        if (fgets(s->resultado, sizeof(s->resultado), f) == NULL) {
            return 0;
        }
        s->resultado[strcspn(s->resultado, "\n")] = '\0';
        s->venceu = strcmp(s->resultado, "RESOLVIDO") == 0;

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            return 0;
        }
        s->tentativasUsadas = atoi(buffer);

        if (formatoHu09) {
            if (fgets(buffer, sizeof(buffer), f) == NULL) {
                return 0;
            }
            s->pistasAntesChute = atoi(buffer);

            if (fgets(buffer, sizeof(buffer), f) == NULL) {
                return 0;
            }
            s->momentoChute = atoi(buffer);
        } else {
            s->pistasAntesChute = -1;
            s->momentoChute = 0;
        }

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            return 0;
        }
        s->recompensa = atoi(buffer);

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            return 0;
        }
        s->timestamp = atol(buffer);

        return 1;
    }

    strncpy(s->casoNome, primeiraLinha, sizeof(s->casoNome) - 1);
    s->casoNome[sizeof(s->casoNome) - 1] = '\0';

    if (fgets(s->dificuldade, sizeof(s->dificuldade), f) == NULL) {
        return 0;
    }
    s->dificuldade[strcspn(s->dificuldade, "\n")] = '\0';

    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        return 0;
    }
    s->secreto = atoi(buffer);

    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        return 0;
    }
    s->tentativasUsadas = atoi(buffer);

    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        return 0;
    }
    s->venceu = atoi(buffer);
    s->casoId = 0;
    strncpy(s->resultado, s->venceu ? "RESOLVIDO" : "FALHOU", sizeof(s->resultado) - 1);
    s->resultado[sizeof(s->resultado) - 1] = '\0';
    s->pistasAntesChute = -1;
    s->momentoChute = 0;
    s->recompensa = 0;
    s->timestamp = 0;

    return 1;
}

static void formatarTimestamp(long timestamp, char *destino, size_t tamanho) {
    time_t valor = (time_t)timestamp;
    struct tm *momento;

    if (timestamp <= 0) {
        snprintf(destino, tamanho, "legado");
        return;
    }

    momento = localtime(&valor);
    if (momento == NULL) {
        snprintf(destino, tamanho, "indefinido");
        return;
    }

    strftime(destino, tamanho, "%d/%m/%Y %H:%M", momento);
}

static int compararMelhorCaso(const Sessao *atual, const Sessao *melhor) {
    if (atual->venceu != melhor->venceu) {
        return atual->venceu > melhor->venceu;
    }
    if (atual->recompensa != melhor->recompensa) {
        return atual->recompensa > melhor->recompensa;
    }
    if (atual->tentativasUsadas != melhor->tentativasUsadas) {
        return atual->tentativasUsadas < melhor->tentativasUsadas;
    }
    return atual->timestamp > melhor->timestamp;
}

static int compararPiorCaso(const Sessao *atual, const Sessao *pior) {
    if (atual->venceu != pior->venceu) {
        return atual->venceu < pior->venceu;
    }
    if (atual->recompensa != pior->recompensa) {
        return atual->recompensa < pior->recompensa;
    }
    if (atual->tentativasUsadas != pior->tentativasUsadas) {
        return atual->tentativasUsadas > pior->tentativasUsadas;
    }
    return atual->timestamp > pior->timestamp;
}

static void exibirAnaliseMetodo(int totalMetodo, double mediaPistas, double mediaMomento) {
    char valor[80];

    uiSection("ANALISE DE METODO", UI_GREEN);
    uiBoxTop();
    snprintf(valor, sizeof(valor), "%d casos com metricas HU-09", totalMetodo);
    uiBoxMid("Base analisada", valor, UI_CYAN);
    snprintf(valor, sizeof(valor), "%.2f", mediaPistas);
    uiBoxMid("Media pistas/chute", valor, mediaPistas >= 2.0 ? UI_YELLOW : UI_GREEN);
    snprintf(valor, sizeof(valor), "%.2f tentativa", mediaMomento);
    uiBoxMid("Momento medio", valor, UI_MAGENTA);

    if (mediaPistas >= 2.0) {
        uiBoxWrap("Voce tende a investigar muito antes de chutar.", UI_YELLOW);
    } else {
        uiBoxWrap("Voce costuma arriscar cedo.", UI_GREEN);
    }
    uiBoxBottom();
}

static int faixaInvestigacao(int pistasAntesChute) {
    if (pistasAntesChute <= 1) {
        return 0;
    }
    if (pistasAntesChute == 2) {
        return 1;
    }
    return 2;
}

static const char *nomeFaixaInvestigacao(int faixa) {
    if (faixa == 0) {
        return "0-1 pista";
    }
    if (faixa == 1) {
        return "2 pistas";
    }
    return "3+ pistas";
}

static int melhorFaixaPorTaxa(const int *totalFaixa, const int *vitoriasFaixa) {
    int melhor = -1;

    for (int i = 0; i < 3; i++) {
        if (totalFaixa[i] <= 0) {
            continue;
        }
        if (melhor < 0) {
            melhor = i;
            continue;
        }
        if (vitoriasFaixa[i] * totalFaixa[melhor] > vitoriasFaixa[melhor] * totalFaixa[i]) {
            melhor = i;
        } else if (vitoriasFaixa[i] * totalFaixa[melhor] == vitoriasFaixa[melhor] * totalFaixa[i] &&
                   totalFaixa[i] > totalFaixa[melhor]) {
            melhor = i;
        }
    }

    return melhor;
}

static void exibirAnaliseVies(int totalMetodo, const int *totalFaixa, const int *vitoriasFaixa) {
    char valor[120];
    int melhorFaixa = melhorFaixaPorTaxa(totalFaixa, vitoriasFaixa);

    if (totalMetodo < 5 || melhorFaixa < 0) {
        return;
    }

    uiSection("VIES DE PALPITE", UI_RED);
    uiBoxTop();
    snprintf(valor, sizeof(valor), "%d casos com metricas HU-09", totalMetodo);
    uiBoxMid("Base analisada", valor, UI_CYAN);

    for (int i = 0; i < 3; i++) {
        int taxa = totalFaixa[i] > 0 ? (vitoriasFaixa[i] * 100) / totalFaixa[i] : 0;
        snprintf(valor, sizeof(valor), "%d/%d (%d%%)", vitoriasFaixa[i], totalFaixa[i], taxa);
        uiBoxMid(nomeFaixaInvestigacao(i), valor, i == melhorFaixa ? UI_GREEN : UI_WHITE);
    }

    snprintf(valor, sizeof(valor), "%s", nomeFaixaInvestigacao(melhorFaixa));
    uiBoxMid("Melhor faixa", valor, UI_GREEN);
    if (melhorFaixa == 0) {
        uiBoxWrap("Voce tende a investigar mais do que o necessario.", UI_YELLOW);
    } else {
        uiBoxWrap("Voce tende a chutar cedo demais.", UI_RED);
    }
    uiBoxBottom();
}

static void exibirMentoriaDinamica(int total, int vitorias, double media, double desvio,
                                   int melhorTentativas, int piorTentativas) {
    uiSection("MENTORIA DINAMICA", UI_YELLOW);
    if (total <= 0) {
        return;
    }

    uiBoxTop();
    if (total < 3) {
        uiBoxWrap("Amostra pequena: jogue mais casos para consolidar o padrao do seu estilo.", UI_WHITE);
    }
    if (media > 4.0) {
        uiBoxWrap("Sua media esta alta. Use busca binaria: palpite no centro e corte metade da faixa.", UI_YELLOW);
    }
    if (desvio > 1.5) {
        uiBoxWrap("Seu ritmo oscila bastante. Defina abertura fixa (centro) e ajuste por intervalos.", UI_MAGENTA);
    } else {
        uiBoxWrap("Boa consistencia: o desvio baixo indica execucao estavel entre partidas.", UI_GREEN);
    }
    if ((vitorias * 100) / total < 50) {
        uiBoxWrap("Taxa de sucesso abaixo de 50%: priorize reduzir erros cedo antes de gastar pistas.", UI_RED);
    } else if (melhorTentativas <= 2 && piorTentativas >= 5) {
        uiBoxWrap("Voce ja resolve rapido em alguns cenarios. Busque repetir essa abertura vencedora.", UI_CYAN);
    } else {
        uiBoxWrap("Estrategia madura: mantenha leitura de feedback e controle de risco por tentativa.", UI_GREEN);
    }
    uiBoxBottom();
}

static void carregarPerfil(void) {
    FILE *f;
    char buffer[64];

    if (saldoCarregado) {
        return;
    }

    f = fopen("perfil.txt", "r");
    if (f == NULL) {
        saldoCache = 0;
        saldoCarregado = 1;
        return;
    }

    if (fgets(buffer, sizeof(buffer), f) != NULL) {
        saldoCache = atoi(buffer);
        if (saldoCache < 0) {
            saldoCache = 0;
        }
    }
    if (fgets(buffer, sizeof(buffer), f) != NULL) {
        reputacaoCache = atoi(buffer);
    } else {
        reputacaoCache = 50;
    }
    if (fgets(buffer, sizeof(buffer), f) != NULL) {
        confiancaCache = atoi(buffer);
    } else {
        confiancaCache = 60;
    }
    for (int i = 0; i < TOTAL_ITENS_LOJA; i++) {
        if (fgets(buffer, sizeof(buffer), f) != NULL) {
            itensCache[i] = atoi(buffer);
        } else {
            itensCache[i] = 0;
        }
        if (itensCache[i] < 0) {
            itensCache[i] = 0;
        }
    }
    if (reputacaoCache < 0) {
        reputacaoCache = 0;
    }
    if (reputacaoCache > 100) {
        reputacaoCache = 100;
    }
    if (confiancaCache < 0) {
        confiancaCache = 0;
    }
    if (confiancaCache > 100) {
        confiancaCache = 100;
    }

    fclose(f);
    saldoCarregado = 1;
}

static void salvarPerfil(void) {
    FILE *f = fopen("perfil.txt", "w");
    if (f == NULL) {
        return;
    }
    fprintf(f, "%d\n%d\n%d\n", saldoCache, reputacaoCache, confiancaCache);
    for (int i = 0; i < TOTAL_ITENS_LOJA; i++) {
        fprintf(f, "%d\n", itensCache[i]);
    }
    fclose(f);
}

void creditar(int valor) {
    if (valor <= 0) {
        return;
    }
    carregarPerfil();
    saldoCache += valor;
    salvarPerfil();
}

int debitar(int valor) {
    if (valor <= 0) {
        return 1;
    }

    carregarPerfil();
    if (valor > saldoCache) {
        return 0;
    }

    saldoCache -= valor;
    salvarPerfil();
    return 1;
}

int getSaldo(void) {
    carregarPerfil();
    return saldoCache;
}

int getScore(void) {
    carregarPerfil();
    return reputacaoCache;
}

void incrementar(int valor) {
    if (valor <= 0) {
        return;
    }
    carregarPerfil();
    reputacaoCache += valor;
    if (reputacaoCache > 100) {
        reputacaoCache = 100;
    }
    salvarPerfil();
}

void decrementar(int valor) {
    if (valor <= 0) {
        return;
    }
    carregarPerfil();
    reputacaoCache -= valor;
    if (reputacaoCache < 0) {
        reputacaoCache = 0;
    }
    salvarPerfil();
}

int getConfiancaDelegacia(void) {
    carregarPerfil();
    return confiancaCache;
}

void aumentarConfianca(int valor) {
    if (valor <= 0) {
        return;
    }
    carregarPerfil();
    confiancaCache += valor;
    if (confiancaCache > 100) {
        confiancaCache = 100;
    }
    salvarPerfil();
}

void reduzirConfianca(int valor) {
    if (valor <= 0) {
        return;
    }
    carregarPerfil();
    confiancaCache -= valor;
    if (confiancaCache < 0) {
        confiancaCache = 0;
    }
    salvarPerfil();
}

int getQuantidadeItem(ItemLoja item) {
    if (item < 0 || item >= TOTAL_ITENS_LOJA) {
        return 0;
    }
    carregarPerfil();
    return itensCache[item];
}

int consumirItem(ItemLoja item) {
    if (item < 0 || item >= TOTAL_ITENS_LOJA) {
        return 0;
    }
    carregarPerfil();
    if (itensCache[item] <= 0) {
        return 0;
    }
    itensCache[item]--;
    salvarPerfil();
    return 1;
}

int comprarItem(ItemLoja item) {
    int preco;
    if (item < 0 || item >= TOTAL_ITENS_LOJA) {
        return 0;
    }
    carregarPerfil();
    preco = precoAjustadoPorConfianca(item);
    if (saldoCache < preco) {
        return 0;
    }
    saldoCache -= preco;
    itensCache[item]++;
    salvarPerfil();
    return 1;
}

void exibirLoja(void) {
    int opcao = 0;

    while (opcao != 7) {
        limparTela();
        printf("\n");
        uiBanner("LOJA ESTRATEGICA", "Suprimentos entre casos");
        uiStamp("COMPRAS PRE-CASO", "Sem acerto automatico", UI_DIM);
        uiSection("CATALOGO", UI_CYAN);
        uiBoxTop();
        for (int i = 0; i < TOTAL_ITENS_LOJA; i++) {
            char linha[220];
            const char *status = "Disponivel";
            int preco = precoAjustadoPorConfianca((ItemLoja)i);
            if (i == ITEM_SCANNER_FORENSE && getConfiancaDelegacia() < 50) {
                status = "Bloqueado: confianca < 50";
            } else if (getSaldo() < preco) {
                status = "Indisponivel: saldo baixo";
            }
            snprintf(linha, sizeof(linha), "[%d] %s | custo: %d | estoque: %d",
                     i + 1, itensNome[i], preco, getQuantidadeItem((ItemLoja)i));
            uiBoxWrap(linha, UI_WHITE);
            uiBoxWrap(itensDescricao[i], UI_DIM);
            uiBoxWrap(status, strcmp(status, "Disponivel") == 0 ? UI_GREEN : UI_YELLOW);
            if (i < TOTAL_ITENS_LOJA - 1) {
                uiBoxWrap(" ", UI_DIM);
            }
        }
        uiBoxBottom();
        uiSection("RECURSOS", UI_MAGENTA);
        uiBoxTop();
        {
            char linha[80];
            snprintf(linha, sizeof(linha), "%d moedas", getSaldo());
            uiBoxMid("Saldo", linha, UI_CYAN);
            snprintf(linha, sizeof(linha), "%d", getConfiancaDelegacia());
            uiBoxMid("Confianca", linha, UI_GREEN);
        }
        uiBoxBottom();
        uiSection("COMANDO", UI_YELLOW);
        printf("  %s[1..6]%s Comprar item | %s[7]%s Voltar\n", UI_WHITE, UI_RESET, UI_YELLOW, UI_RESET);
        uiPrompt("LOJA");
        opcao = lerOpcao(1, 7);
        if (opcao == 7) {
            break;
        }
        if (opcao == 3 && getConfiancaDelegacia() < 50) {
            uiAlert("LOJA", "Scanner Forense bloqueado: confianca da delegacia abaixo de 50.", UI_RED);
            pausar();
            continue;
        }
        if (!comprarItem((ItemLoja)(opcao - 1))) {
            uiAlert("LOJA", "Compra recusada: saldo insuficiente.", UI_RED);
            pausar();
            continue;
        }
        uiAlert("LOJA", "Item adquirido e armazenado no inventario.", UI_GREEN);
        pausar();
    }
}

void salvarSessao(Sessao s) {
    FILE *f = fopen("historico.txt", "a");
    if (f != NULL) {
        fprintf(f, "HU09\n%d\n%s\n%s\n%d\n%s\n%d\n%d\n%d\n%d\n%ld\n",
                s.casoId,
                s.casoNome,
                s.dificuldade,
                s.secreto,
                s.resultado[0] != '\0' ? s.resultado : (s.venceu ? "RESOLVIDO" : "FALHOU"),
                s.tentativasUsadas,
                s.pistasAntesChute,
                s.momentoChute,
                s.recompensa,
                s.timestamp);
        fclose(f);
    }
}

void exibirHistorico(void) {
    FILE *f = fopen("historico.txt", "r");
    int total = 0;
    int vitorias = 0;
    int capacidade = 0;
    Sessao *sessoes = NULL;
    char melhorCaso[50] = "-";
    char piorCaso[50] = "-";
    
    limparTela();
    printf("\n");
    uiBanner("DOSSIE COMPLETO", "Historico e analise forense do detetive");
    uiStamp("ARQUIVO LOCAL", "ANALISE DE PERFIL", UI_DIM);
    
    if (f == NULL) {
        uiSection("BANCO DE SESSOES", UI_YELLOW);
        uiBoxTop();
        uiBoxText("Nenhum historico encontrado.");
        uiBoxText("Resolva ou tente um caso para gerar dados de perfil.");
        uiBoxBottom();
    } else {
        Sessao s;
        int melhorTentativas = 0;
        int piorTentativas = 0;
        int melhorIndice = -1;
        int piorIndice = -1;
        double mediaTentativas = 0.0;
        double desvioPadrao = 0.0;
        double mediaPistasAntesChute = 0.0;
        double mediaMomentoChute = 0.0;
        int totalFaixa[3] = {0, 0, 0};
        int vitoriasFaixa[3] = {0, 0, 0};
        int totalMetodo = 0;
        int temAnalise = 0;

        while (lerSessao(f, &s)) {
            if (total >= capacidade) {
                int novaCapacidade = capacidade == 0 ? 16 : capacidade * 2;
                Sessao *novoBuffer = realloc(sessoes, (size_t)novaCapacidade * sizeof(Sessao));
                if (novoBuffer == NULL) {
                    uiAlert("MEMORIA", "Falha ao expandir analise. Relatorio parcial exibido.", UI_RED);
                    break;
                }
                sessoes = novoBuffer;
                capacidade = novaCapacidade;
            }

            sessoes[total] = s;
            if (sessoes[total].venceu == 1) {
                vitorias++;
            }

            total++;
        }
        fclose(f);

        uiSection("CASOS JOGADOS", UI_CYAN);
        uiBoxTop();
        uiBoxText("Marcadores: [MELHOR] melhor caso | [PIOR] pior caso");
        uiBoxText("Caso                           Resultado  Tent.  Pistas  Chute  Recompensa  Quando");
        uiBoxBottom();

        char valor[80];
        if (total > 0) {
            int somaTentativas = somaTentativasRec(sessoes, total);
            melhorTentativas = minTentativasRec(sessoes, total);
            piorTentativas = maxTentativasRec(sessoes, total);
            long long somaQuadrados = somaQuadradosTentativasRec(sessoes, total);
            double variancia;

            mediaTentativas = (double)somaTentativas / total;
            variancia = ((double)somaQuadrados / total) - (mediaTentativas * mediaTentativas);
            if (variancia < 0.0) {
                variancia = 0.0;
            }
            desvioPadrao = raizQuadradaNewton(variancia);

            for (int i = 0; i < total; i++) {
                if (melhorIndice < 0 || compararMelhorCaso(&sessoes[i], &sessoes[melhorIndice])) {
                    melhorIndice = i;
                }
                if (piorIndice < 0 || compararPiorCaso(&sessoes[i], &sessoes[piorIndice])) {
                    piorIndice = i;
                }
                if (sessoes[i].tentativasUsadas == melhorTentativas) {
                    strncpy(melhorCaso, sessoes[i].casoNome, sizeof(melhorCaso) - 1);
                    melhorCaso[sizeof(melhorCaso) - 1] = '\0';
                    break;
                }
            }
            for (int i = 0; i < total; i++) {
                if (sessoes[i].tentativasUsadas == piorTentativas) {
                    strncpy(piorCaso, sessoes[i].casoNome, sizeof(piorCaso) - 1);
                    piorCaso[sizeof(piorCaso) - 1] = '\0';
                    break;
                }
            }
            for (int i = 0; i < total; i++) {
                if (sessoes[i].pistasAntesChute >= 0 && sessoes[i].momentoChute > 0) {
                    int faixa = faixaInvestigacao(sessoes[i].pistasAntesChute);
                    mediaPistasAntesChute += sessoes[i].pistasAntesChute;
                    mediaMomentoChute += sessoes[i].momentoChute;
                    totalFaixa[faixa]++;
                    if (sessoes[i].venceu) {
                        vitoriasFaixa[faixa]++;
                    }
                    totalMetodo++;
                }
            }
            if (totalMetodo > 0) {
                mediaPistasAntesChute /= totalMetodo;
                mediaMomentoChute /= totalMetodo;
            }

            uiSection("LINHA DO TEMPO", UI_CYAN);
            for (int i = 0; i < total; i++) {
                char quando[24];
                char pistasChute[12];
                char momentoChute[12];
                const char *cor = UI_WHITE;
                const char *marcador = "        ";

                formatarTimestamp(sessoes[i].timestamp, quando, sizeof(quando));
                if (i == melhorIndice && i == piorIndice) {
                    cor = UI_MAGENTA;
                    marcador = "[AMBOS] ";
                } else if (i == melhorIndice) {
                    cor = UI_GREEN;
                    marcador = "[MELHOR]";
                } else if (i == piorIndice) {
                    cor = UI_RED;
                    marcador = "[PIOR]  ";
                } else if (!sessoes[i].venceu) {
                    cor = UI_YELLOW;
                }
                if (sessoes[i].pistasAntesChute >= 0) {
                    snprintf(pistasChute, sizeof(pistasChute), "%d", sessoes[i].pistasAntesChute);
                } else {
                    snprintf(pistasChute, sizeof(pistasChute), "-");
                }
                if (sessoes[i].momentoChute > 0) {
                    snprintf(momentoChute, sizeof(momentoChute), "%da", sessoes[i].momentoChute);
                } else {
                    snprintf(momentoChute, sizeof(momentoChute), "-");
                }

                printf("  %s%s %-30s %-9s %5d %7s %6s %10d  %s%s\n",
                       cor,
                       marcador,
                       sessoes[i].casoNome,
                       sessoes[i].resultado[0] != '\0' ? sessoes[i].resultado : (sessoes[i].venceu ? "RESOLVIDO" : "FALHOU"),
                       sessoes[i].tentativasUsadas,
                       pistasChute,
                       momentoChute,
                       sessoes[i].recompensa,
                       quando,
                       UI_RESET);
            }
            printf("\n");

            uiSection("PAINEL ESTATISTICO", UI_MAGENTA);
            uiBoxTop();
            snprintf(valor, sizeof(valor), "%d sessoes", total);
            uiBoxMid("Amostra", valor, UI_CYAN);
            snprintf(valor, sizeof(valor), "%d/%d (%d%%)", vitorias, total, (vitorias * 100) / total);
            uiBoxMid("Taxa de sucesso", valor, vitorias == total ? UI_GREEN : UI_YELLOW);
            snprintf(valor, sizeof(valor), "%.2f", mediaTentativas);
            uiBoxMid("Media tent.", valor, UI_WHITE);
            snprintf(valor, sizeof(valor), "%.2f", desvioPadrao);
            uiBoxMid("Desvio padrao", valor, UI_MAGENTA);
            snprintf(valor, sizeof(valor), "%s (%d)", melhorCaso, melhorTentativas);
            uiBoxMid("Melhor caso", valor, UI_GREEN);
            snprintf(valor, sizeof(valor), "%s (%d)", piorCaso, piorTentativas);
            uiBoxMid("Maior custo", valor, UI_RED);
            if (melhorIndice >= 0) {
                snprintf(valor, sizeof(valor), "%s | %d moedas",
                         sessoes[melhorIndice].casoNome, sessoes[melhorIndice].recompensa);
                uiBoxMid("Destaque melhor", valor, UI_GREEN);
            }
            if (piorIndice >= 0) {
                snprintf(valor, sizeof(valor), "%s | %s",
                         sessoes[piorIndice].casoNome,
                         sessoes[piorIndice].resultado[0] != '\0' ? sessoes[piorIndice].resultado : "FALHOU");
                uiBoxMid("Destaque pior", valor, UI_RED);
            }
            if (totalMetodo >= 3) {
                snprintf(valor, sizeof(valor), "%.2f", mediaPistasAntesChute);
                uiBoxMid("Media pistas/chute", valor,
                         mediaPistasAntesChute >= 2.0 ? UI_YELLOW : UI_GREEN);
            } else {
                snprintf(valor, sizeof(valor), "%d/3 casos", totalMetodo);
                uiBoxMid("Analise metodo", valor, UI_DIM);
            }
            if (totalMetodo >= 5) {
                int melhorFaixa = melhorFaixaPorTaxa(totalFaixa, vitoriasFaixa);
                snprintf(valor, sizeof(valor), "%s", nomeFaixaInvestigacao(melhorFaixa));
                uiBoxMid("Melhor faixa", valor, UI_GREEN);
            } else {
                snprintf(valor, sizeof(valor), "%d/5 casos", totalMetodo);
                uiBoxMid("Analise vies", valor, UI_DIM);
            }
            temAnalise = 1;
        } else {
            uiBoxTop();
            uiBoxText("Nenhuma sessao legivel foi encontrada no arquivo.");
        }
        snprintf(valor, sizeof(valor), "%d moedas", getSaldo());
        uiBoxMid("Saldo atual", valor, UI_MAGENTA);
        snprintf(valor, sizeof(valor), "%d", getConfiancaDelegacia());
        uiBoxMid("Confianca delegacia", valor, UI_CYAN);
        uiBoxBottom();

        if (temAnalise) {
            exibirMentoriaDinamica(total, vitorias, mediaTentativas, desvioPadrao, melhorTentativas, piorTentativas);
            if (totalMetodo >= 3) {
                exibirAnaliseMetodo(totalMetodo, mediaPistasAntesChute, mediaMomentoChute);
            }
            if (totalMetodo >= 5) {
                exibirAnaliseVies(totalMetodo, totalFaixa, vitoriasFaixa);
            }
        }
    }
    free(sessoes);
    pausar();
}
