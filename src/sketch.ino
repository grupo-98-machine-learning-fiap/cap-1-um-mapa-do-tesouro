#include <DHT.h>

// ── Pinos ─────────────────────────────────────────────────
#define DHTPIN     4
#define DHTTYPE    DHT22
#define LDR_PIN    34
#define BTN_N      18
#define BTN_P      19
#define BTN_K      21
#define RELAY_PIN  23
#define LED_PIN    2

// ── Limiares ──────────────────────────────────────────────
#define UMIDADE_SECA   40.0f
#define PH_MIN          5.5f
#define PH_MAX          6.5f
#define NUTRIENTES_MIN  2

// ── Temporização ──────────────────────────────────────────
#define DEBOUNCE_MS   50UL
#define LEITURA_MS  2000UL

DHT dht(DHTPIN, DHTTYPE);

struct Botao {
  uint8_t       pino;
  const char*   nome;
  bool          ativo;
  bool          leituraAnterior;
  bool          confirmado;
  unsigned long tDebounce;
};

Botao npk[3] = {
  { BTN_N, "Nitrogenio (N)", false, true, true, 0 },
  { BTN_P, "Fosforo    (P)", false, true, true, 0 },
  { BTN_K, "Potassio   (K)", false, true, true, 0 }
};

void atualizarBotoes();
float adcParaPH(int adc);
void setIrrigacao(bool ligar);
void cicloIrrigacao();

// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BTN_N,     INPUT_PULLUP);
  pinMode(BTN_P,     INPUT_PULLUP);
  pinMode(BTN_K,     INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN,   OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN,   LOW);

  Serial.println("\n+=====================================+");
  Serial.println(  "|   IRRIGACAO INTELIGENTE - CAFE IoT  |");
  Serial.println(  "+=====================================+");
  Serial.println(  "| Clique nos botoes para ativar ou    |");
  Serial.println(  "| desativar cada nutriente (toggle).  |");
  Serial.println(  "+=====================================+\n");
}

// ══════════════════════════════════════════════════════════
void loop() {
  atualizarBotoes();

  static unsigned long tUltimo = 0;
  if (millis() - tUltimo >= LEITURA_MS) {
    tUltimo = millis();
    cicloIrrigacao();
  }
}

// ══════════════════════════════════════════════════════════
void atualizarBotoes() {
  for (int i = 0; i < 3; i++) {
    bool leituraFisica = (digitalRead(npk[i].pino) == LOW);

    if (leituraFisica != npk[i].leituraAnterior) {
      npk[i].tDebounce = millis();
    }

    if ((millis() - npk[i].tDebounce) >= DEBOUNCE_MS) {
      bool novoConfirmado = leituraFisica;

      if (novoConfirmado && !npk[i].confirmado) {
        npk[i].ativo = !npk[i].ativo;
        Serial.println("+-------------------------------------+");
        Serial.printf( "| BOTAO  >> %-26s|\n", npk[i].nome);
        Serial.printf( "| ESTADO >> %-26s|\n", npk[i].ativo ? "ATIVADO" : "DESATIVADO");
        Serial.println("+-------------------------------------+\n");
      }

      npk[i].confirmado = novoConfirmado;
    }

    npk[i].leituraAnterior = leituraFisica;
  }
}

// ══════════════════════════════════════════════════════════
float adcParaPH(int adc) {
  return (4095 - adc) / 4095.0f * 14.0f;
}

void setIrrigacao(bool ligar) {
  digitalWrite(RELAY_PIN, ligar ? HIGH : LOW);
  digitalWrite(LED_PIN,   ligar ? HIGH : LOW);
}

// ══════════════════════════════════════════════════════════
void cicloIrrigacao() {
  float umidade     = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("+-------------------------------------+");
    Serial.println("|  ERRO: falha na leitura do DHT22.  |");
    Serial.println("+-------------------------------------+\n");
    return;
  }

  int   adcLDR = analogRead(LDR_PIN);
  float ph     = adcParaPH(adcLDR);

  int totalNPK = 0;
  for (int i = 0; i < 3; i++) {
    if (npk[i].ativo) totalNPK++;
  }

  bool soloSeco    = (umidade  <  UMIDADE_SECA);
  bool phAdequado  = (ph >= PH_MIN && ph <= PH_MAX);
  bool npkAdequado = (totalNPK >= NUTRIENTES_MIN);

  // ── Leituras ────────────────────────────────────────────
  Serial.println("+=====================================+");
  Serial.println("|            LEITURAS                 |");
  Serial.println("+-------------------------------------+");
  Serial.printf( "|  Umidade    : %5.1f%%  %-13s|\n",
    umidade, soloSeco ? "(SOLO SECO)" : "(SOLO UMIDO)");
  Serial.printf( "|  Temperatura: %5.1f C                |\n", temperatura);
  Serial.printf( "|  pH         : %5.2f   %-13s|\n",
    ph, phAdequado ? "(ADEQUADO)" : "(INADEQUADO)");

  // ── Nutrientes ──────────────────────────────────────────
  Serial.println("+-------------------------------------+");
  Serial.println("|            NUTRIENTES               |");
  Serial.println("+-------------------------------------+");
  for (int i = 0; i < 3; i++) {
    Serial.printf("|  %-20s  [ %-3s ]       |\n",
      npk[i].nome, npk[i].ativo ? "ON" : "OFF");
  }
  Serial.printf( "|  Total ativos: %d de 3               |\n", totalNPK);

  // ── Decisão ─────────────────────────────────────────────
  Serial.println("+-------------------------------------+");
  Serial.println("|            DECISAO                  |");
  Serial.println("+-------------------------------------+");

  if (!soloSeco) {
    Serial.println("|  Solo umido.                        |");
    Serial.println("|  >> Irrigacao nao necessaria.       |");
    setIrrigacao(false);
  } else {
    Serial.println("|  Solo seco - verificando...         |");
    if (phAdequado && npkAdequado) {
      Serial.println("|  >> IRRIGACAO ATIVADA!              |");
      setIrrigacao(true);
    } else {
      Serial.println("|  >> Irrigacao bloqueada:            |");
      if (!phAdequado)  Serial.println("|     - pH fora da faixa ideal.       |");
      if (!npkAdequado) Serial.printf( "|     - Nutri. insuf. (%d/3 ativos). |\n", totalNPK);
      setIrrigacao(false);
    }
  }

  Serial.println("+=====================================+\n");
}
