#ifndef UTILS_H
#define UTILS_H

#define VERMELHO  "\033[1;31m"
#define RESET     "\033[0m"

void limparTela(void);
void pausar(void);
int  lerOpcao(int min, int max);
int  lerInteiroIntervalo(int min, int max, const char *prompt);

#endif
