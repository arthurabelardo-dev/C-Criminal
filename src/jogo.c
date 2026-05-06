#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"
#include "historico.h"
#include "utils.h"

static void exibirRelatorio(int idCaso) {
    limparTela();
    printf("\n");
    printf(VERMELHO "  CASO %02d: ", idCaso);
    
    if (idCaso == 1) {
        printf("O ULTIMO SUSPIRO DO MAGNATA\n\n" RESET);
        printf("  Dificuldade: Facil\n");
        printf("  Tentativas disponiveis: 7\n\n");
        printf("  RELATORIO DE CAMPO: \"Detetive, a cena e caotica. O bilionario Gaga de\n");
        printf("  Big Field foi encontrado morto em sua biblioteca particular na sua\n");
        printf("  Mansao. O cofre da parede esta aberto e vazio, mas ha um detalhe: o\n");
        printf("  assassino, conhecido como Alinho do Goita, nao levou as barras de\n");
        printf("  ouro, apenas um pendrive contendo as chaves de acesso das Industrias\n");
        printf("  Gaga. O sistema de seguranca da mansao registrou um acesso forcado no\n");
        printf("  teclado numerico do cofre momentos antes do crime. Usamos luz\n");
        printf("  ultravioleta e detectamos uma assinatura de calor residual em um\n");
        printf("  codigo especifico. Infelizmente, o teclado e de tecnologia antiga e\n");
        printf("  so aceita codigos entre 1 e 50.\n\n");
        printf("  A MISSAO: A digital esta desaparecendo conforme o teclado esfria.\n");
        printf("  Voce precisa descobrir o codigo exato que o assassino usou para abrir\n");
        printf("  o cofre. Se falhar apos 7 tentativas, o sistema travara\n");
        printf("  permanentemente e a trilha do assassino ficara fria para sempre.\"\n");
    } 
    else if (idCaso == 2) {
        printf("FREQUENCIA DE FUGA NO CASSINO\n\n" RESET);
        printf("  Dificuldade: Medio\n");
        printf("  Tentativas disponiveis: 6 | Intervalo: 1 a 100\n\n");
        printf("  RELATORIO DE CAMPO: \"Estamos no meio de um assalto em andamento em\n");
        printf("  Santo Amaro. Uma gangue de alta tecnologia, liderada por mopretu,\n");
        printf("  invadiu a central de seguranca e instalou um bloqueador de sinal\n");
        printf("  potente. Neste exato momento, eles estao no heliponto do predio,\n");
        printf("  esperando o helicoptero de resgate que chegara em poucos minutos.\n");
        printf("  Nossas comunicacoes estao cortadas. Para derrubar o bloqueador e\n");
        printf("  pedir reforcos aereos, voce deve usar o scanner de radio portatil\n");
        printf("  da policia e sintonizar na frequencia exata que a gangue esta\n");
        printf("  usando para coordenar a fuga. Nossos tecnicos confirmaram que o\n");
        printf("  sinal opera em uma faixa de 1 a 100 MHz.\n\n");
        printf("  A MISSAO: O tempo e o seu pior inimigo. O helicoptero de fuga\n");
        printf("  pousara em breve. Se voce nao encontrar a frequencia correta em\n");
        printf("  ate 6 tentativas, o sinal sera criptografado novamente e os\n");
        printf("  criminosos escaparao com milhoes em diamantes.\"\n");
    } 
    else {
        printf("PROTOCOLO APOCALIPSE\n\n" RESET);
        printf("  Dificuldade: Dificil\n");
        printf("  Tentativas disponiveis: 5 | Intervalo: 1 a 200\n\n");
        printf("  RELATORIO DE CAMPO: \"Situacao de emergencia maxima, detetive. O\n");
        printf("  centro de controle da rede eletrica do Pina foi invadido por um\n");
        printf("  cyber-terrorista conhecido como 'CH do Pina'. Ele injetou um\n");
        printf("  malware de autodestruicao que causara um apagao total em todo o\n");
        printf("  estado em menos de 10 minutos. O virus esta escondido em uma\n");
        printf("  porta de rede (Port) camuflada dentro do servidor central. O\n");
        printf("  firewall do sistema foi corrompido, mas o agente Abedalama\n");
        printf("  conseguiu isolar a ameaca em um range de portas entre 1 e 200.\n");
        printf("  Cada tentativa errada dispara um pulso de sobrecarga que queima\n");
        printf("  os fusveis da central.\n\n");
        printf("  MISSAO: Voce tem apenas 5 tentativas antes que o hardware do\n");
        printf("  servidor derreta por completo, mergulhando a cidade na escuridao\n");
        printf("  e causando o caos total. O destino da infraestrutura do estado\n");
        printf("  esta nas suas maos. Encontre a porta correta e neutralize o\n");
        printf("  protocolo.\"\n");
    }
    
    printf("\n  " VERDE "[Pressione ENTER para iniciar a investigacao]" RESET);
    getchar();
    
    limparTela();
    printf("\n  Caso carregado. Prepare-se, Detetive.\n");
    printf("\n  " VERDE "[Pressione ENTER para comecar]" RESET);
    getchar();
}

void jogarPartida(int idCaso) {
    if (idCaso < 1 || idCaso > 3) {
        printf(VERMELHO "  Erro: id de caso invalido (%d).\n" RESET, idCaso);
        pausar();
        return;
    }

    exibirRelatorio(idCaso);
    
    int maxVal = (idCaso == 1) ? 50 : (idCaso == 2) ? 100 : 200;
    int maxTentativas = (idCaso == 1) ? 7 : (idCaso == 2) ? 6 : 5;
    
    int secreto = 1 + (rand() % maxVal);
    int tentativas = maxTentativas;
    
    int palpitesDados[10]; 
    int contPalpites = 0;
    char feedback[200] = "Aguardando seu primeiro palpite...";
    
    Sessao s;
    if (idCaso == 1) {
        strcpy(s.casoNome, "O Ultimo Suspiro do Magnata");
        strcpy(s.dificuldade, "Facil");
    } else if (idCaso == 2) {
        strcpy(s.casoNome, "Frequencia de Fuga no Cassino");
        strcpy(s.dificuldade, "Medio");
    } else {
        strcpy(s.casoNome, "Protocolo Apocalipse");
        strcpy(s.dificuldade, "Dificil");
    }
    s.secreto = secreto;
    s.venceu = 0;

    int palpite = 0;

    while (tentativas > 0) {
        limparTela();
        printf("\n");
        printf(VERMELHO "  INVESTIGACAO ATIVA: CASO 0%d\n" RESET, idCaso);
        printf(AMARELO "  ! TENTATIVAS RESTANTES: %d/%d\n" RESET, tentativas, maxTentativas);
        printf("  ============================================================\n\n");
        
        printf(CIANO "  >> Digite o codigo (1 a %d)\n" RESET, maxVal);
        printf(AMARELO "  (Digite 0 para solicitar uma dica do perito)\n\n" RESET);
        
        printf("  FEEDBACK DO SISTEMA:\n");
        printf("  \"%s\"\n\n", feedback);
        printf("  > ");
        
        palpite = lerOpcao(0, maxVal);

        if (palpite == 0) {
            if (secreto % 2 == 0) {
                strcpy(feedback, "DICA DO PERITO: O alvo e um numero PAR.");
            } else {
                strcpy(feedback, "DICA DO PERITO: O alvo e um numero IMPAR.");
            }
            continue;
        }

        palpitesDados[contPalpites] = palpite;
        contPalpites++;

        if (palpite == secreto) {
            s.venceu = 1;
            break;
        } else {
            if (palpite < secreto) {
                strcpy(feedback, "A assinatura termica aponta para cima. O codigo e MAIOR.");
            } else {
                strcpy(feedback, "A assinatura termica esfriou. O codigo e MENOR.");
            }
            tentativas--;
        }
        
        printf("  ============================================================\n\n");
        printf("  LOG DE TENTATIVAS:\n");
        for(int i = 0; i < contPalpites; i++) {
            printf("  [%d] %d\n", i+1, palpitesDados[i]);
        }
        pausar();
    }

    s.tentativasUsadas = contPalpites;

    limparTela();
    printf("\n  ============================================================\n");
    printf(VERMELHO "                  RESULTADO DA INVESTIGACAO\n\n" RESET);
    
    if (s.venceu == 1) {
        printf(VERDE "              [V] *** CASO RESOLVIDO COM SUCESSO ***\n\n" RESET);
        printf("  SUCESSO! O codigo %d foi confirmado. Excelente trabalho!\n\n", secreto);
    } else {
        printf(VERMELHO "              [X] *** INVESTIGACAO FALHOU ***\n\n" RESET);
        printf("  O sistema travou. O codigo verdadeiro era %d. O assassino escapou.\n\n", secreto);
    }

    printf(CIANO "  --- RELATORIO DE DESEMPENHO ---\n" RESET);
    printf("  Caso: 0%d\n", idCaso);
    printf("  Tentativas usadas: %d de %d\n", s.tentativasUsadas, maxTentativas);
    
    printf(VERDE "\n  [V] Registro salvo no banco de dados do departamento.\n" RESET);
    
    salvarSessao(s);
    pausar();
}
