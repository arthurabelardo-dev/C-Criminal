# Frontend Web do C-Criminal

Esta pasta contem uma interface web funcional para demonstrar a camada visual do projeto sem alterar o jogo em C do terminal.

## Como abrir

Abra `frontend/index.html` no navegador. Login padrao:

- Usuario: `detetive`
- Senha: `1234`

## Como integrar com o backend C

O arquivo `app.js` expoe `window.CCriminalBackendHooks`. Hoje esses hooks retornam `null`, entao a interface usa um simulador local para continuar jogavel. Para plugar o C, substitua os hooks por chamadas para uma camada sem `printf`, `fgets` ou `getchar`, por exemplo:

- `login(credentials)`
- `startCase(caseId)`
- `submitGuess({ caseId, value })`
- `requestClue({ caseId })`
- `interrogate({ caseId, suspectIndex })`
- `buyItem(itemId)`
- `loadHistory()`

O caminho ideal e extrair de `src/jogo.c` e `src/historico.c` funcoes que retornem dados estruturados, mantendo `src/tui.c` apenas para o terminal.

## Heuristicas de Nielsen aplicadas

- Visibilidade do status: o painel do detetive muda para estados `idle`, `thinking`, `success` e `warning`, e a faixa de processamento aparece durante acoes demoradas.
- Correspondencia com o mundo real: casos, dossies, loja, lupa, arquivo e scanner seguem a linguagem investigativa do projeto.
- Controle e liberdade: telas principais possuem voltar/cancelar, e encerrar caso ou sair pede confirmacao.
- Prevencao de erros: palpites fora da faixa sao bloqueados antes de consumir tentativa, compras e encerramento usam modal.
- Estetica minimalista: layout noir com papel envelhecido, dourado de evidencia, vermelho de alerta e verde de sucesso.

## Assets publicos sugeridos

- OpenGameArt: https://opengameart.org/art-search-advanced?keys=detective
- Kenney UI Pack: https://kenney.nl/assets/ui-pack
- Kenney Input Prompts: https://kenney.nl/assets/input-prompts
- Game-icons.net: https://game-icons.net/
- itch.io assets gratuitos: https://itch.io/game-assets/free/tag-detective
- Google Imagens: https://www.google.com/search?tbm=isch&q=detective+sprite+png+transparent

## Sprites PNG

Coloque arquivos opcionais nesta estrutura para substituir o detetive desenhado em CSS:

- `frontend/assets/detective-idle.png`
- `frontend/assets/detective-thinking.png`
- `frontend/assets/detective-success.png`
- `frontend/assets/detective-warning.png`

Tambem e possivel sobrescrever via JavaScript:

```js
window.CCriminalAssets.preloadPngAssets({
  idle: ["./meus-assets/detetive-idle.png"],
  thinking: ["./meus-assets/detetive-pensando.png"]
});
```
