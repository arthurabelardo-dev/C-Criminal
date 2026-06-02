"use strict";

const STORAGE_KEYS = {
  profile: "ccriminal.gui.profile",
  history: "ccriminal.gui.history"
};

const CASES = [
  {
    id: 1,
    shortTitle: "Magnata",
    title: "O Ultimo Suspiro do Magnata",
    codename: "COFRE-FANTASMA",
    difficulty: "FACIL",
    color: "green",
    target: "codigo do cofre",
    min: 1,
    max: 50,
    attempts: 7,
    reward: 100,
    briefing: "A assinatura termica do cofre esta sumindo. Encontre o codigo antes que o rastro esfrie.",
    scene: "Biblioteca Field, 23:17. Fita UV no teclado, cofre aberto e calor residual desaparecendo.",
    clues: [
      "Pericia UV: o codigo parece seguir um padrao par.",
      "Diario da vitima: combinacoes divisiveis por 3 eram usadas em noites de contrato.",
      "Analise do teclado: a tecla final pode ser 0 ou 5.",
      "Relato da secretaria: a vitima gostava de numeros primos no cofre antigo."
    ],
    suspects: [
      ["Mordomo Estevao", "Cuidava da biblioteca e conhecia a rotina de troca de senha."],
      ["Helena Field", "Saiu cedo, mas evitou detalhar o trajeto ate a garagem."],
      ["Vigia Ramos", "Alegou falha de camera no periodo do acesso ao cofre."]
    ]
  },
  {
    id: 2,
    shortTitle: "Cassino",
    title: "Frequencia de Fuga no Cassino",
    codename: "RADIO-OURO",
    difficulty: "MEDIO",
    color: "yellow",
    target: "frequencia de radio",
    min: 1,
    max: 100,
    attempts: 6,
    reward: 150,
    briefing: "A gangue escapou com radios criptografados. Sintonize a frequencia antes do helicoptero de fuga.",
    scene: "Cassino Santo Amaro. Roleta parada, antena quebrada e um sinal curto no radio.",
    clues: [
      "Mesa de som: a frequencia final tem chance alta de ser par.",
      "Caderno VIP: a gangue testava canais em saltos de tres.",
      "Rota tatica: o canal pode estar em bloco multiplo de 7."
    ],
    suspects: [
      ["Dealer Nando", "Disse que o piloto ja estava pronto antes do assalto comecar."],
      ["Cantora Lia", "Ouviu dois codigos diferentes no retorno do palco."],
      ["Seguranca Tito", "Desligou a camera da doca por 4 minutos sem autorizacao."]
    ]
  },
  {
    id: 3,
    shortTitle: "Apocalipse",
    title: "Protocolo Apocalipse",
    codename: "PORTA-ZERO",
    difficulty: "DIFICIL",
    color: "red",
    target: "porta infectada",
    min: 1,
    max: 200,
    attempts: 5,
    reward: 220,
    briefing: "Um malware entrou pelo backbone estadual. Descubra a porta de comando antes do colapso.",
    scene: "SOC central, 03:06. Logs truncados, mapa piscando e acesso negado em cascata.",
    clues: [
      "SOC limpo: a porta infectada pode seguir padrao de numero primo.",
      "Telemetria: a porta responde melhor em varreduras pares.",
      "Firewall legado: a assinatura pode terminar em 0 ou 5."
    ],
    suspects: [
      ["Analista Iara", "Encontrou tres relatorios com checksum diferente."],
      ["Invasor CH", "Mistura detalhes reais com provocacao durante o interrogatorio."],
      ["Engenheira Celia", "Alterou regra de firewall sem chamado oficial."]
    ]
  }
];

const STORE_ITEMS = [
  {
    id: "segunda_chance",
    name: "Segunda Chance",
    price: 50,
    icon: "shield",
    description: "Ignora uma leitura errada.",
    useLabel: "Armar chance"
  },
  {
    id: "analise_extra",
    name: "Analise Extra",
    price: 30,
    icon: "search",
    description: "Revela uma pista adicional.",
    useLabel: "Analisar pista"
  },
  {
    id: "scanner_forense",
    name: "Scanner Forense",
    price: 60,
    icon: "scan-line",
    description: "Confere ruido no conjunto de pistas.",
    useLabel: "Usar scanner",
    requiresTrust: 50
  },
  {
    id: "intuicao",
    name: "Intuicao",
    price: 80,
    icon: "brain",
    description: "Mostra uma faixa aproximada do alvo.",
    useLabel: "Usar intuicao"
  },
  {
    id: "mais_2_tentativas",
    name: "+2 Tentativas",
    price: 40,
    icon: "plus-circle",
    description: "Adiciona duas varreduras ao caso.",
    useLabel: "Adicionar +2"
  },
  {
    id: "mais_1_interrogatorio",
    name: "+1 Interrogatorio",
    price: 25,
    icon: "messages-square",
    description: "Libera uma consulta extra.",
    useLabel: "Liberar consulta"
  }
];

const ASSET_LINKS = [
  ["OpenGameArt - detective", "https://opengameart.org/art-search-advanced?keys=detective"],
  ["Kenney - UI Pack", "https://kenney.nl/assets/ui-pack"],
  ["Kenney - Input Prompts", "https://kenney.nl/assets/input-prompts"],
  ["Game-icons.net - lupa", "https://game-icons.net/"],
  ["itch.io - free detective assets", "https://itch.io/game-assets/free/tag-detective"],
  ["Google Imagens - detective sprite png", "https://www.google.com/search?tbm=isch&q=detective+sprite+png+transparent"]
];

const spriteSources = {
  idle: ["/assets/detetive.png"],
  thinking: ["./assets/detective-thinking.png"],
  success: ["./assets/detective-success.png"],
  warning: ["./assets/detective-warning.png"]
};

const loadedSprites = {};

// Ganchos de integracao: troque estes retornos por chamadas HTTP, WebAssembly ou ponte nativa.
// A interface continua funcionando pelo simulador local enquanto o backend C nao expuser uma API sem terminal.
const backendHooks = {
  async login(credentials) {
    return null;
  },
  async startCase(caseId) {
    return null;
  },
  async submitGuess(payload) {
    return null;
  },
  async requestClue(payload) {
    return null;
  },
  async interrogate(payload) {
    return null;
  },
  async buyItem(itemId) {
    return null;
  },
  async loadHistory() {
    return null;
  }
};

window.CCriminalBackendHooks = backendHooks;
window.CCriminalAssets = { spriteSources, preloadPngAssets };

const defaultInventory = STORE_ITEMS.reduce((inventory, item) => {
  inventory[item.id] = 0;
  return inventory;
}, {});

const state = {
  screen: "login",
  loginAttempts: 3,
  busy: false,
  busyLabel: "",
  mood: "idle",
  detectiveMessage: "Mesa fria, cafe quente. Informe a credencial para abrir a sala de evidencias.",
  selectedCaseId: null,
  session: null,
  result: null,
  confirm: null,
  overlay: null,
  toast: null,
  profile: loadProfile(),
  history: loadHistory()
};

document.addEventListener("DOMContentLoaded", () => {
  preloadPngAssets();
  document.addEventListener("click", handleClick);
  document.addEventListener("submit", handleSubmit);
  render();
});

async function preloadPngAssets(customSources = {}) {
  Object.assign(spriteSources, customSources);
  await Promise.all(Object.entries(spriteSources).map(async ([mood, sources]) => {
    loadedSprites[mood] = await loadFirstImage(sources);
  }));
  syncDetectiveImage();
}

function loadFirstImage(sources) {
  return new Promise((resolve) => {
    let index = 0;

    function tryNext() {
      if (index >= sources.length) {
        resolve(null);
        return;
      }

      const image = new Image();
      image.onload = () => resolve(image.src);
      image.onerror = () => {
        index += 1;
        tryNext();
      };
      image.src = sources[index];
    }

    tryNext();
  });
}

function render() {
  const app = document.querySelector("#app");
  app.innerHTML = `
    <div class="shell">
      ${renderDetectivePanel()}
      <main class="main">
        ${state.busy ? renderBusyStrip() : ""}
        ${renderScreen()}
      </main>
      ${renderConfirm()}
      ${renderOverlay()}
      ${renderToast()}
    </div>
  `;
  refreshIcons();
  syncDetectiveImage();
}

function renderDetectivePanel() {
  return `
    <aside class="detective-panel">
      <div class="brand">
        <div>
          <span class="brand-kicker">Departamento C-Criminal</span>
          <h1>Sala de Evidencias</h1>
        </div>
        <span class="badge"><i data-lucide="radio"></i>PLANTAO</span>
      </div>

      <section class="detective-card" aria-label="Detetive guia">
        <div class="detective-sprite" data-mood="${state.mood}">
          <img id="detectiveImage" alt="Detetive guia" hidden>
          <div class="detective-fallback" aria-hidden="true"></div>
        </div>
        <div class="speech">${escapeHtml(state.detectiveMessage)}</div>
      </section>

      <section class="panel-card">
        <p class="panel-title"><i data-lucide="activity"></i>Quadro do detetive</p>
        <div class="stat-grid">
          <div class="stat"><span>Moedas</span><strong>${state.profile.coins}</strong></div>
          <div class="stat"><span>Reputacao</span><strong>${state.profile.reputation}</strong></div>
          <div class="stat"><span>Confianca</span><strong>${state.profile.trust}</strong></div>
          <div class="stat"><span>Sessoes</span><strong>${state.history.length}</strong></div>
        </div>
      </section>

      ${renderCaseBriefPanel()}
    </aside>
  `;
}

function renderCaseBriefPanel() {
  const activeCase = state.session
    ? getCaseById(state.session.caseId)
    : state.selectedCaseId
      ? getCaseById(state.selectedCaseId)
      : null;

  if (!activeCase) {
    return `
      <section class="panel-card dossier-mini">
        <p class="panel-title"><i data-lucide="folder"></i>Ficha na mesa</p>
        <p class="muted">Nenhum caso aberto. O mural esta aguardando triagem.</p>
      </section>
    `;
  }

  return `
    <section class="panel-card dossier-mini">
      <p class="panel-title"><i data-lucide="folder-open"></i>Ficha na mesa</p>
      <strong>${activeCase.shortTitle}</strong>
      <span>${activeCase.codename}</span>
      <span>${activeCase.target} | ${activeCase.min}-${activeCase.max}</span>
    </section>
  `;
}

function renderBusyStrip() {
  return `
    <div class="busy-strip" role="status">
      <span class="spinner" aria-hidden="true"></span>
      <span>${escapeHtml(state.busyLabel)}</span>
    </div>
  `;
}

function renderScreen() {
  if (state.screen === "login") return renderLogin();
  if (state.screen === "dashboard") return renderDashboard();
  if (state.screen === "caseConfirm") return renderCaseConfirm();
  if (state.screen === "case") return renderCase();
  if (state.screen === "history") return renderHistory();
  if (state.screen === "store") return renderStore();
  if (state.screen === "result") return renderResult();
  if (state.screen === "locked") return renderLocked();
  return renderDashboard();
}

function renderLogin() {
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Canal criptografado</p>
          <h2>Porta da delegacia</h2>
          <p class="screen-subtitle">So entra quem sabe o codigo da madrugada.</p>
        </div>
        <span class="badge"><i data-lucide="key-round"></i>${state.loginAttempts} tentativas</span>
      </header>

      <div class="screen-card login-card">
        <form class="form-grid" data-form="login">
          <label class="field">
            <span>Usuario</span>
            <input name="username" autocomplete="username" value="detetive" required>
          </label>
          <label class="field">
            <span>Senha</span>
            <input name="password" type="password" autocomplete="current-password" placeholder="1234" required autofocus>
          </label>
          <div class="actions">
            <button class="button primary" type="submit" ${state.busy ? "disabled" : ""}>
              <i data-lucide="log-in"></i>Entrar
            </button>
          </div>
        </form>
      </div>
    </section>
  `;
}

function renderDashboard() {
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Plantao forense</p>
          <h2>Mural de casos</h2>
          <p class="screen-subtitle">Tres fichas presas no quadro. Cada uma cobra sangue frio e leitura fina.</p>
        </div>
        <button class="button ghost" data-action="logout"><i data-lucide="power"></i>Sair</button>
      </header>

      <div class="evidence-tape">AREA RESTRITA - CADEIA DE CUSTODIA ATIVA</div>

      <div class="case-grid">
        ${CASES.map(renderCaseCard).join("")}
      </div>

      <div class="tool-grid">
        <button class="button" data-action="history"><i data-lucide="archive"></i>Arquivo morto</button>
        <button class="button" data-action="store"><i data-lucide="shopping-bag"></i>Arsenal</button>
      </div>
    </section>
  `;
}

function renderCaseCard(gameCase) {
  return `
    <article class="case-card case-card-${gameCase.id}">
      <span class="case-number">CASO 0${gameCase.id}</span>
      <div class="meta-line">
        <span class="pill ${gameCase.color}">${gameCase.difficulty}</span>
        <span class="pill cyan">${gameCase.codename}</span>
      </div>
      <h3>${gameCase.shortTitle}</h3>
      <p class="muted">${gameCase.briefing}</p>
      <div class="case-scene">${gameCase.scene}</div>
      <div class="actions">
        <button class="button primary" data-action="selectCase" data-id="${gameCase.id}">
          <i data-lucide="folder-search"></i>Investigar
        </button>
      </div>
    </article>
  `;
}

function renderCaseConfirm() {
  const gameCase = getSelectedCase();
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Confirmacao tatica</p>
          <h2>${gameCase.title}</h2>
          <p class="screen-subtitle">${gameCase.briefing}</p>
        </div>
        <button class="button ghost" data-action="dashboard"><i data-lucide="arrow-left"></i>Voltar</button>
      </header>

      <div class="screen-card case-file">
        <div class="meta-line">
          <span class="pill ${gameCase.color}">${gameCase.difficulty}</span>
          <span class="pill">Alvo: ${gameCase.target}</span>
          <span class="pill">Faixa: ${gameCase.min} a ${gameCase.max}</span>
          <span class="pill">Tentativas: ${gameCase.attempts}</span>
          <span class="pill green">Recompensa: ${gameCase.reward}</span>
        </div>
        <p class="screen-subtitle">${gameCase.scene}</p>
        <div class="actions">
          <button class="button primary" data-action="startCase" ${state.busy ? "disabled" : ""}>
            <i data-lucide="play"></i>Iniciar investigacao
          </button>
          <button class="button ghost" data-action="dashboard">
            <i data-lucide="x"></i>Cancelar
          </button>
        </div>
      </div>
    </section>
  `;
}

function renderCase() {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);
  const attemptsPct = Math.max(0, (session.attemptsLeft / session.maxAttempts) * 100);
  const cluePct = Math.max(0, ((gameCase.clues.length - session.clues.length) / gameCase.clues.length) * 100);
  const interrogationPct = Math.max(0, ((session.maxInterrogations - session.interrogations.length) / session.maxInterrogations) * 100);

  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Investigacao ativa</p>
          <h2>${gameCase.shortTitle}</h2>
          <p class="screen-subtitle">${session.feedback}</p>
        </div>
        <button class="button danger" data-action="askAbort"><i data-lucide="x-octagon"></i>Encerrar</button>
      </header>

      <div class="game-layout">
        <div class="screen-card command-panel case-file">
          <div class="meter-grid">
            ${renderMeter("Integridade", session.attemptsLeft, session.maxAttempts, attemptsPct, "var(--ok)")}
            ${renderMeter("Pistas de campo", gameCase.clues.length - session.clues.length, gameCase.clues.length, cluePct, "var(--brass)")}
            ${renderMeter("Interrogatorios", session.maxInterrogations - session.interrogations.length, session.maxInterrogations, interrogationPct, "var(--violet)")}
          </div>

          <form class="guess-row" data-form="guess">
            <label class="field">
              <span>Palpite (${gameCase.min} a ${gameCase.max})</span>
              <input name="guess" type="number" min="${gameCase.min}" max="${gameCase.max}" inputmode="numeric" required autofocus>
            </label>
            <button class="button primary" type="submit" ${state.busy ? "disabled" : ""}>
              <i data-lucide="crosshair"></i>Testar
            </button>
          </form>

          <div class="actions">
            <button class="button" data-action="requestClue" ${session.clues.length >= gameCase.clues.length ? "disabled" : ""}>
              <i data-lucide="search"></i>Pista
            </button>
            <button class="button" data-action="interrogate" ${session.interrogations.length >= session.maxInterrogations ? "disabled" : ""}>
              <i data-lucide="messages-square"></i>Interrogar
            </button>
            <button class="button" data-action="evidence"><i data-lucide="clipboard-list"></i>Evidencias</button>
            <button class="button" data-action="victimDossier"><i data-lucide="file-text"></i>Dossie</button>
          </div>

          ${renderInventoryActions()}
        </div>

        <div class="screen-card logbook">
          <p class="panel-title"><i data-lucide="scan-line"></i>Log de varreduras</p>
          ${session.guesses.length ? `
            <ul class="log-list">
              ${session.guesses.slice().reverse().map((entry) => `
                <li class="log-item">
                  <strong>${entry.value}</strong> - ${entry.reading}
                </li>
              `).join("")}
            </ul>
          ` : `<div class="empty">Nenhuma leitura registrada.</div>`}
        </div>
      </div>
    </section>
  `;
}

function renderInventoryActions() {
  const usableItems = STORE_ITEMS.filter((item) => state.profile.inventory[item.id] > 0);
  if (!usableItems.length) {
    return `<div class="empty">Nenhum item estrategico disponivel.</div>`;
  }

  return `
    <div>
      <p class="panel-title"><i data-lucide="briefcase"></i>Inventario</p>
      <div class="actions">
        ${usableItems.map((item) => `
          <button class="button small" data-action="useItem" data-id="${item.id}">
            <i data-lucide="${item.icon}"></i>${item.useLabel} (${state.profile.inventory[item.id]})
          </button>
        `).join("")}
      </div>
    </div>
  `;
}

function renderMeter(label, value, max, percent, fill) {
  return `
    <div class="meter">
      <div class="meter-head"><span>${label}</span><strong>${value}/${max}</strong></div>
      <div class="meter-bar"><span class="meter-fill" style="--value:${percent}%;--fill:${fill}"></span></div>
    </div>
  `;
}

function renderHistory() {
  const stats = calculateHistoryStats();
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Arquivo morto</p>
          <h2>Dossie e mentoria</h2>
          <p class="screen-subtitle">Registros do plantao, taxa de acerto e custo de cada investigacao.</p>
        </div>
        <button class="button ghost" data-action="dashboard"><i data-lucide="arrow-left"></i>Voltar</button>
      </header>

      <div class="screen-card case-file">
        <div class="meta-line">
          <span class="pill cyan">${stats.total} sessoes</span>
          <span class="pill green">${stats.wins} resolvidas</span>
          <span class="pill yellow">Media ${stats.averageAttempts}</span>
        </div>
      </div>

      ${state.history.length ? `
        <div class="history-list">
          ${state.history.slice().reverse().map((entry) => `
            <article class="history-row">
              <div>
                <strong>${entry.caseName}</strong>
                <span class="muted">${entry.difficulty} - alvo ${entry.secret} - ${entry.attemptsUsed} tentativa(s)</span>
              </div>
              <span class="pill ${entry.won ? "green" : "red"}">${entry.won ? "RESOLVIDO" : "ARQUIVO FRIO"}</span>
            </article>
          `).join("")}
        </div>
      ` : `<div class="empty">Nenhuma sessao registrada neste plantao.</div>`}
    </section>
  `;
}

function renderStore() {
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Pre-caso</p>
          <h2>Arsenal da delegacia</h2>
          <p class="screen-subtitle">Itens de campo custam moedas e precisam de confirmacao do responsavel.</p>
        </div>
        <button class="button ghost" data-action="dashboard"><i data-lucide="arrow-left"></i>Voltar</button>
      </header>

      <div class="store-grid">
        ${STORE_ITEMS.map(renderStoreItem).join("")}
      </div>
    </section>
  `;
}

function renderStoreItem(item) {
  const blocked = item.requiresTrust && state.profile.trust < item.requiresTrust;
  const insufficient = state.profile.coins < item.price;
  const disabled = blocked || insufficient;
  const status = blocked ? `Confianca minima: ${item.requiresTrust}` : insufficient ? "Saldo insuficiente" : "Disponivel";

  return `
    <article class="store-card">
      <div class="meta-line">
        <span class="pill cyan"><i data-lucide="${item.icon}"></i>${item.price} moedas</span>
        <span class="pill">Estoque: ${state.profile.inventory[item.id]}</span>
      </div>
      <h3>${item.name}</h3>
      <p class="muted">${item.description}</p>
      <span class="pill ${disabled ? "yellow" : "green"}">${status}</span>
      <button class="button ${disabled ? "" : "primary"}" data-action="buyItem" data-id="${item.id}" ${disabled ? "disabled" : ""}>
        <i data-lucide="shopping-cart"></i>Comprar
      </button>
    </article>
  `;
}

function renderResult() {
  const result = state.result;
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Relatorio final</p>
          <h2>${result.won ? "Caso resolvido" : "Arquivo frio"}</h2>
          <p class="screen-subtitle">${result.message}</p>
        </div>
      </header>

      <div class="screen-card case-file">
        <div class="meta-line">
          <span class="pill ${result.won ? "green" : "red"}">${result.caseName}</span>
          <span class="pill">Alvo: ${result.secret}</span>
          <span class="pill">Tentativas: ${result.attemptsUsed}</span>
          <span class="pill green">Recompensa: ${result.reward}</span>
        </div>
        <div class="actions">
          <button class="button primary" data-action="dashboard"><i data-lucide="layout-dashboard"></i>Voltar ao mural</button>
          <button class="button" data-action="history"><i data-lucide="archive"></i>Ver dossie</button>
        </div>
      </div>
    </section>
  `;
}

function renderLocked() {
  return `
    <section class="screen">
      <header class="screen-header">
        <div>
          <p class="screen-kicker">Acesso bloqueado</p>
          <h2>Terminal travado</h2>
          <p class="screen-subtitle">Muitas credenciais invalidas foram enviadas.</p>
        </div>
      </header>
    </section>
  `;
}

function renderConfirm() {
  if (!state.confirm) return "";
  return `
    <div class="overlay" role="presentation">
      <section class="modal" role="dialog" aria-modal="true" aria-labelledby="confirmTitle">
        <h3 id="confirmTitle">${state.confirm.title}</h3>
        <p>${state.confirm.message}</p>
        <div class="actions">
          <button class="button ${state.confirm.danger ? "danger" : "primary"}" data-action="confirmAccept">
            <i data-lucide="${state.confirm.danger ? "alert-triangle" : "check"}"></i>${state.confirm.confirmLabel}
          </button>
          <button class="button ghost" data-action="confirmCancel">
            <i data-lucide="x"></i>Cancelar
          </button>
        </div>
      </section>
    </div>
  `;
}

function renderOverlay() {
  if (!state.overlay) return "";

  if (state.overlay === "evidence") {
    const session = state.session;
    return modalMarkup("Arquivo de evidencias", `
      <p>Pistas e depoimentos coletados ate agora.</p>
      <p class="panel-title"><i data-lucide="search"></i>Pistas</p>
      ${session.clues.length ? listMarkup(session.clues, "evidence-item") : `<div class="empty">Nenhuma pista coletada.</div>`}
      <p class="panel-title"><i data-lucide="messages-square"></i>Interrogatorios</p>
      ${session.interrogations.length ? listMarkup(session.interrogations, "evidence-item") : `<div class="empty">Nenhum suspeito ouvido.</div>`}
    `);
  }

  if (state.overlay === "victimDossier") {
    const gameCase = getCaseById(state.session.caseId);
    return modalMarkup(`Dossie: ${gameCase.shortTitle}`, `
      <p>${gameCase.briefing}</p>
      <div class="case-scene">${gameCase.scene}</div>
      <p class="panel-title"><i data-lucide="users"></i>Suspeitos</p>
      ${listMarkup(gameCase.suspects.map(([name, text]) => `<strong>${name}</strong><br>${text}`), "suspect-item", false)}
    `);
  }

  if (state.overlay === "interrogate") {
    const gameCase = getCaseById(state.session.caseId);
    return modalMarkup("Rede de interrogados", `
      <p>Escolha um suspeito ainda nao ouvido.</p>
      <ul class="suspect-list">
        ${gameCase.suspects.map(([name, text], index) => {
          const alreadyHeard = state.session.interrogations.some((line) => line.includes(name));
          return `
            <li class="suspect-item">
              <strong>${name}</strong><br>
              <span class="muted">${text}</span>
              <div class="actions">
                <button class="button small" data-action="hearSuspect" data-id="${index}" ${alreadyHeard ? "disabled" : ""}>
                  <i data-lucide="message-circle"></i>Ouvir
                </button>
              </div>
            </li>
          `;
        }).join("")}
      </ul>
    `);
  }

  if (state.overlay === "assets") {
    return modalMarkup("Curadoria de assets", `
      <p>Fontes publicas para sprites PNG do detetive, icones de lupa, arquivos, radio e itens.</p>
      <ul class="evidence-list">
        ${ASSET_LINKS.map(([label, url]) => `<li class="evidence-item"><a href="${url}" target="_blank" rel="noreferrer">${label}</a></li>`).join("")}
      </ul>
    `);
  }

  return "";
}

function modalMarkup(title, content) {
  return `
    <div class="overlay" role="presentation">
      <section class="modal" role="dialog" aria-modal="true">
        <h3>${title}</h3>
        ${content}
        <div class="actions">
          <button class="button ghost" data-action="closeOverlay"><i data-lucide="x"></i>Fechar</button>
        </div>
      </section>
    </div>
  `;
}

function listMarkup(items, className, escape = true) {
  return `
    <ul class="evidence-list">
      ${items.map((item) => `<li class="${className}">${escape ? escapeHtml(item) : item}</li>`).join("")}
    </ul>
  `;
}

function renderToast() {
  if (!state.toast) return "";
  return `<div class="toast ${state.toast.type}" role="status">${state.toast.message}</div>`;
}

async function handleSubmit(event) {
  const form = event.target.closest("form[data-form]");
  if (!form) return;
  event.preventDefault();

  if (form.dataset.form === "login") {
    await handleLogin(form);
  }

  if (form.dataset.form === "guess") {
    await handleGuess(form);
  }
}

async function handleClick(event) {
  const button = event.target.closest("[data-action]");
  if (!button || state.busy) return;

  const action = button.dataset.action;
  const id = button.dataset.id;

  if (action === "dashboard") goDashboard();
  if (action === "history") goHistory();
  if (action === "store") goStore();
  if (action === "selectCase") selectCase(Number(id));
  if (action === "startCase") await startCase();
  if (action === "requestClue") await requestClue();
  if (action === "interrogate") openOverlay("interrogate");
  if (action === "hearSuspect") await hearSuspect(Number(id));
  if (action === "evidence") openOverlay("evidence");
  if (action === "victimDossier") openOverlay("victimDossier");
  if (action === "assetOverlay") openOverlay("assets");
  if (action === "closeOverlay") closeOverlay();
  if (action === "buyItem") askBuyItem(id);
  if (action === "useItem") useItem(id);
  if (action === "askAbort") askAbortCase();
  if (action === "confirmAccept") acceptConfirm();
  if (action === "confirmCancel") cancelConfirm();
  if (action === "logout") askLogout();
}

async function handleLogin(form) {
  const formData = new FormData(form);
  const credentials = {
    username: String(formData.get("username") || "").trim(),
    password: String(formData.get("password") || "").trim()
  };

  const backendResult = await withProcessing("Conferindo credencial no arquivo restrito", () => backendHooks.login(credentials));
  const authenticated = backendResult ? backendResult.ok : credentials.username === "detetive" && credentials.password === "1234";

  if (authenticated) {
    state.screen = "dashboard";
    setDetective("success", "A porta abriu. O quadro de casos esta sob sua responsabilidade.");
    showToast("Credencial aceita.", "success");
  } else {
    state.loginAttempts -= 1;
    if (state.loginAttempts <= 0) {
      state.screen = "locked";
      setDetective("warning", "Terminal bloqueado por seguranca.");
    } else {
      setDetective("warning", `Credencial recusada. Restam ${state.loginAttempts} tentativa(s) antes do lacre.`);
      showToast("Usuario ou senha invalidos.", "danger");
    }
  }

  render();
}

async function handleGuess(form) {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);
  const value = Number(new FormData(form).get("guess"));

  if (!Number.isInteger(value) || value < gameCase.min || value > gameCase.max) {
    setDetective("warning", `Valor fora da faixa valida: ${gameCase.min} a ${gameCase.max}.`);
    showToast("Palpite bloqueado para prevenir erro de entrada.", "warning");
    render();
    return;
  }

  await withProcessing("Revelando leitura no laboratorio", () => backendHooks.submitGuess({ caseId: gameCase.id, value }));
  processGuess(value);
}

async function startCase() {
  const gameCase = getSelectedCase();
  const backendResult = await withProcessing("Abrindo envelope lacrado", () => backendHooks.startCase(gameCase.id));
  const secret = backendResult && Number.isInteger(backendResult.secret)
    ? backendResult.secret
    : randomInt(gameCase.min, gameCase.max);

  state.session = {
    caseId: gameCase.id,
    secret,
    maxAttempts: gameCase.attempts,
    attemptsLeft: gameCase.attempts,
    maxInterrogations: 2,
    attemptsUsed: 0,
    clues: [],
    interrogations: [],
    guesses: [],
    secondChanceArmed: false,
    feedback: `Alvo: ${gameCase.target}. Faixa valida de ${gameCase.min} a ${gameCase.max}.`
  };
  state.screen = "case";
  state.overlay = null;
  setDetective("idle", "Ficha aberta. Va com calma: cada leitura deixa rastro.");
  render();
}

function processGuess(value) {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);

  if (value === session.secret) {
    session.attemptsUsed += 1;
    session.guesses.push({ value, reading: "ALVO CONFIRMADO" });
    completeCase(true);
    return;
  }

  const direction = value < session.secret ? "MAIOR" : "MENOR";
  const distance = Math.abs(value - session.secret);
  const reading = `${direction}. ${temperatureLabel(distance, gameCase.max - gameCase.min)}.`;
  session.guesses.push({ value, reading });

  if (session.secondChanceArmed) {
    session.secondChanceArmed = false;
    session.feedback = `${reading} Segunda Chance absorveu o erro.`;
    setDetective("thinking", "Boa cobertura. A tentativa ficou fora do relatorio.");
    showToast("Segunda Chance consumida.", "success");
    render();
    return;
  }

  session.attemptsUsed += 1;
  session.attemptsLeft -= 1;

  if (session.attemptsLeft <= 0) {
    completeCase(false);
    return;
  }

  session.feedback = reading;
  setDetective("thinking", `Leitura no quadro: ${reading}`);
  render();
}

async function requestClue() {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);

  if (session.clues.length >= gameCase.clues.length) {
    showToast("Todas as pistas de campo ja foram coletadas.", "warning");
    return;
  }

  await withProcessing("Chamando a pericia de campo", () => backendHooks.requestClue({ caseId: gameCase.id }));
  const clue = gameCase.clues[session.clues.length];
  session.clues.push(clue);
  session.feedback = clue;
  setDetective("success", "Pista grampeada no quadro. Compare antes de apostar alto.");
  render();
}

async function hearSuspect(index) {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);
  const suspect = gameCase.suspects[index];

  if (!suspect || session.interrogations.length >= session.maxInterrogations) {
    showToast("Limite de interrogatorios atingido.", "warning");
    return;
  }

  await withProcessing("Ligando gravador da sala de interrogatorio", () => backendHooks.interrogate({ caseId: gameCase.id, suspectIndex: index }));
  const line = `${suspect[0]}: ${buildStatementForSecret(session.secret)}`;
  session.interrogations.push(line);
  session.feedback = line;
  state.overlay = null;
  setDetective("thinking", "Depoimento gravado. Nem toda voz na sala joga limpo.");
  render();
}

function askBuyItem(itemId) {
  const item = STORE_ITEMS.find((entry) => entry.id === itemId);
  if (!item) return;

  state.confirm = {
    title: `Comprar ${item.name}?`,
    message: `Esta compra consome ${item.price} moedas do caixa da investigacao.`,
    confirmLabel: "Confirmar compra",
    danger: false,
    onConfirm: async () => {
      await withProcessing("Carimbando requisicao no almoxarifado", () => backendHooks.buyItem(itemId));
      state.profile.coins -= item.price;
      state.profile.inventory[item.id] += 1;
      saveProfile();
      setDetective("success", `${item.name} entrou na maleta.`);
      showToast("Item adquirido.", "success");
      render();
    }
  };
  render();
}

function useItem(itemId) {
  const session = state.session;
  const item = STORE_ITEMS.find((entry) => entry.id === itemId);
  if (!session || !item || state.profile.inventory[itemId] <= 0) return;

  state.profile.inventory[itemId] -= 1;

  if (itemId === "segunda_chance") {
    session.secondChanceArmed = true;
    session.feedback = "Segunda Chance armada para a proxima leitura incorreta.";
  }

  if (itemId === "analise_extra") {
    const gameCase = getCaseById(session.caseId);
    const clue = gameCase.clues[session.clues.length] || "Analise extra: nenhuma pista nova disponivel.";
    session.clues.push(clue);
    session.feedback = clue;
  }

  if (itemId === "scanner_forense") {
    session.feedback = "Scanner Forense: nenhuma pista falsa ativa foi detectada nesta rodada.";
  }

  if (itemId === "intuicao") {
    const gameCase = getCaseById(session.caseId);
    const low = Math.max(gameCase.min, session.secret - 10);
    const high = Math.min(gameCase.max, session.secret + 10);
    session.feedback = `Intuicao: o alvo esta entre ${low} e ${high}.`;
  }

  if (itemId === "mais_2_tentativas") {
    session.maxAttempts += 2;
    session.attemptsLeft += 2;
    session.feedback = "Duas varreduras adicionais foram liberadas.";
  }

  if (itemId === "mais_1_interrogatorio") {
    session.maxInterrogations += 1;
    session.feedback = "Uma consulta extra de interrogatorio foi liberada.";
  }

  saveProfile();
  setDetective("success", `${item.name} aplicado ao caso atual.`);
  render();
}

function askAbortCase() {
  state.confirm = {
    title: "Encerrar investigacao?",
    message: "A ficha sera encerrada como arquivo frio. Confirme para evitar um abandono acidental.",
    confirmLabel: "Encerrar caso",
    danger: true,
    onConfirm: () => completeCase(false)
  };
  render();
}

function askLogout() {
  state.confirm = {
    title: "Encerrar sessao?",
    message: "Voce voltara para a porta da delegacia.",
    confirmLabel: "Sair",
    danger: false,
    onConfirm: () => {
      state.screen = "login";
      state.session = null;
      state.result = null;
      state.loginAttempts = 3;
      setDetective("idle", "Luzes apagadas. A delegacia aguarda nova credencial.");
      render();
    }
  };
  render();
}

function completeCase(won) {
  const session = state.session;
  const gameCase = getCaseById(session.caseId);
  const reward = won ? calculateReward(gameCase, session) : 0;
  const reputationDelta = won ? 8 : -7;
  const trustDelta = won ? 5 : -4;

  state.profile.coins += reward;
  state.profile.reputation = clamp(state.profile.reputation + reputationDelta, 0, 100);
  state.profile.trust = clamp(state.profile.trust + trustDelta, 0, 100);

  const result = {
    id: Date.now(),
    caseName: gameCase.title,
    difficulty: gameCase.difficulty,
    secret: session.secret,
    attemptsUsed: session.attemptsUsed,
    won,
    reward,
    message: won
      ? "A cadeia de evidencias foi preservada e a delegacia recebeu o relatorio."
      : "A investigacao foi encerrada sem confirmacao do alvo."
  };

  state.history.push(result);
  state.result = result;
  state.session = null;
  state.screen = "result";
  state.overlay = null;
  state.confirm = null;
  saveProfile();
  saveHistory();
  setDetective(won ? "success" : "warning", result.message);
  render();
}

function acceptConfirm() {
  const action = state.confirm && state.confirm.onConfirm;
  state.confirm = null;
  if (typeof action === "function") {
    action();
  } else {
    render();
  }
}

function cancelConfirm() {
  state.confirm = null;
  setDetective("idle", "Acao cancelada. A ficha continua sob controle.");
  render();
}

function goDashboard() {
  state.screen = "dashboard";
  state.selectedCaseId = null;
  state.overlay = null;
  setDetective("idle", "Mural limpo. Escolha a proxima pista antes que esfrie.");
  render();
}

function goHistory() {
  state.screen = "history";
  state.overlay = null;
  setDetective("thinking", "Arquivo morto na mesa. Procure padroes nas tentativas.");
  render();
}

function goStore() {
  state.screen = "store";
  state.overlay = null;
  setDetective("idle", "Arsenal aberto. Nada sai do caixa sem confirmacao.");
  render();
}

function selectCase(caseId) {
  state.selectedCaseId = caseId;
  state.screen = "caseConfirm";
  const gameCase = getCaseById(caseId);
  setDetective("thinking", `Ficha ${gameCase.codename} separada para revisao.`);
  render();
}

function openOverlay(name) {
  state.overlay = name;
  render();
}

function closeOverlay() {
  state.overlay = null;
  render();
}

async function withProcessing(label, task) {
  state.busy = true;
  state.busyLabel = label;
  setDetective("thinking", label);
  render();
  await delay(420);
  const result = await task();
  state.busy = false;
  state.busyLabel = "";
  return result;
}

function showToast(message, type = "success") {
  state.toast = { message, type };
  window.clearTimeout(showToast.timer);
  showToast.timer = window.setTimeout(() => {
    state.toast = null;
    render();
  }, 2600);
}

function setDetective(mood, message) {
  state.mood = mood;
  state.detectiveMessage = message;
}

function syncDetectiveImage() {
  const image = document.querySelector("#detectiveImage");
  if (!image) return;

  const src = loadedSprites[state.mood] || loadedSprites.idle;
  if (!src) {
    image.hidden = true;
    return;
  }

  image.src = src;
  image.hidden = false;
}

function refreshIcons() {
  if (window.lucide) {
    window.lucide.createIcons();
  }
}

function getSelectedCase() {
  return getCaseById(state.selectedCaseId);
}

function getCaseById(caseId) {
  return CASES.find((gameCase) => gameCase.id === Number(caseId)) || CASES[0];
}

function calculateReward(gameCase, session) {
  const efficiency = Math.max(1, session.maxAttempts - session.attemptsUsed + 1);
  return Math.round(gameCase.reward + efficiency * 8);
}

function calculateHistoryStats() {
  const total = state.history.length;
  const wins = state.history.filter((entry) => entry.won).length;
  const attempts = state.history.reduce((sum, entry) => sum + entry.attemptsUsed, 0);
  return {
    total,
    wins,
    averageAttempts: total ? (attempts / total).toFixed(2) : "0.00"
  };
}

function buildStatementForSecret(secret) {
  if (secret % 2 === 0) return "O rastro aponta para um valor par.";
  if (secret % 5 === 0) return "Ouvi referencia a final 0 ou 5.";
  if (secret % 3 === 0) return "A sequencia parecia divisivel por tres.";
  return "O suspeito nao confirmou padrao forte, mas ficou nervoso.";
}

function temperatureLabel(distance, range) {
  const ratio = distance / Math.max(1, range);
  if (ratio <= 0.05) return "QUENTE";
  if (ratio <= 0.15) return "MORNO";
  if (ratio <= 0.32) return "FRIO";
  return "GELADO";
}

function loadProfile() {
  const fallback = {
    coins: 120,
    reputation: 50,
    trust: 60,
    inventory: { ...defaultInventory }
  };

  try {
    const saved = JSON.parse(localStorage.getItem(STORAGE_KEYS.profile));
    if (!saved) return fallback;
    return {
      coins: Number.isInteger(saved.coins) ? saved.coins : fallback.coins,
      reputation: Number.isInteger(saved.reputation) ? saved.reputation : fallback.reputation,
      trust: Number.isInteger(saved.trust) ? saved.trust : fallback.trust,
      inventory: { ...defaultInventory, ...(saved.inventory || {}) }
    };
  } catch (error) {
    return fallback;
  }
}

function loadHistory() {
  try {
    const saved = JSON.parse(localStorage.getItem(STORAGE_KEYS.history));
    return Array.isArray(saved) ? saved : [];
  } catch (error) {
    return [];
  }
}

function saveProfile() {
  localStorage.setItem(STORAGE_KEYS.profile, JSON.stringify(state.profile));
}

function saveHistory() {
  localStorage.setItem(STORAGE_KEYS.history, JSON.stringify(state.history));
}

function randomInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function clamp(value, min, max) {
  return Math.min(max, Math.max(min, value));
}

function delay(ms) {
  return new Promise((resolve) => window.setTimeout(resolve, ms));
}

function escapeHtml(value) {
  return String(value)
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#039;");
}
