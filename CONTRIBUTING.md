# Contribuindo

Obrigado por querer contribuir com este projeto.

Siga este guia objetivo para configurar o ambiente, executar o projeto, testar alterações e enviar pull requests.

## Requisitos

- Git
- `make` (ou ambiente capaz de executar o `Makefile` presente)
- Compilador C (ex: `gcc`)

## Clonar o repositório

```bash
git clone <URL_DO_REPO>
cd C-Criminal
```

Substitua `<URL_DO_REPO>` pela URL do repositório remoto (HTTPS ou SSH).

## Montando o ambiente (Windows)

1. Instale o Git: https://git-scm.com/
2. Instale o MinGW-w64 ou outro compilador C e adicione ao `PATH`.
3. Instale o `make` (por exemplo através do MSYS2 ou Chocolatey):

```powershell
choco install make
```

4. Abra um terminal (PowerShell ou Git Bash) dentro da pasta do projeto.

## Montando o ambiente (Linux / macOS)

```bash
# Debian/Ubuntu
sudo apt update && sudo apt install build-essential make git

# macOS (com Homebrew)
brew install gcc make git
```

## Compilar e executar

O projeto inclui um `Makefile` na raiz. Para compilar e executar:

```bash
make
./bin/main || ./main
```

Se o `Makefile` gerar o binário em outro local, siga as instruções mostradas no terminal.

## Executar testes

Se existirem testes (por exemplo `reputacao_test.c`), compile e execute-os manualmente:

```bash
gcc -o reputacao_test src/reputacao_test.c src/utils.c src/historico.c -Iinclude
./reputacao_test
```

Adapte os caminhos de acordo com a organização do projeto.

## Fluxo de contribuição

1. Faça um fork do repositório (se aplicável).
2. Crie uma branch a partir de `main` com nome claro:

```bash
git checkout -b feature/ISSUE-123-descricao-curta
```

3. Faça commits pequenos e significativos. Mensagens recomendadas:

```
Tipo: Descrição curta

Exemplos:
feat: adicionar validação de entrada em jogo
fix: corrigir leak em historico
docs: atualizar README
```

4. Mantenha o código formatado e legível. Siga as convenções já existentes no projeto.

5. Atualize ou adicione testes quando aplicar mudanças lógicas.

6. Rebase ou atualize sua branch com `main` antes de abrir o PR:

```bash
git fetch origin
git checkout main
git pull
git checkout feature/…
git rebase main
```

## Abrir Pull Request

- Envie o PR apontando para a branch `main` do repositório original.
- Descreva claramente o que foi feito, por que e como testar.
- Inclua capturas de tela ou logs quando relevantes.

## Revisão e CI

Se houver integração contínua (CI), aponte o PR para que os checks rodem automaticamente. Resolva problemas apontados pelos checks e responda a comentários de revisores.

## Convenções de código

- Use nomes de variáveis descritivos.
- Evite funções muito longas; extraia helpers quando necessário.
- Prefira commits pequenos e reversíveis.

## Licença e direitos

Ao contribuir, você concorda que sua contribuição será incorporada sob a licença do projeto (veja o `LICENSE` se existir). Se for necessário um CLA, os mantenedores informarão no PR.

## Contato

Se tiver dúvidas, abra uma issue descrevendo o problema ou pergunta. Mantenedores responderão conforme disponibilidade.

---

Obrigado por colaborar — contribuições pequenas ou grandes são bem-vindas!
