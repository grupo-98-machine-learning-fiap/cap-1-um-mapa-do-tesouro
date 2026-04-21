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
  { BTN_N, "N", false, true, true, 0 },
  { BTN_P, "P", false, true, true, 0 },
  { BTN_K, "K", false, true, true, 0 }
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

  Serial.println("\n=== Irrigacao Inteligente - Cafe IoT ===");
  Serial.println("Clique nos botoes para ativar/desativar.");
  Serial.println();
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
        Serial.print("[BTN] ");
        Serial.print(npk[i].nome);
        Serial.println(npk[i].ativo ? ": ativado" : ": desativado");
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
    Serial.println("[ERRO] Falha no DHT22.");
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

  // Leituras
  Serial.println("---");
  Serial.print("Umidade: ");     Serial.print(umidade, 1);
  Serial.println(soloSeco ? "% (seco)" : "% (umido)");
  Serial.print("Temp: ");        Serial.print(temperatura, 1); Serial.println(" C");
  Serial.print("pH: ");          Serial.print(ph, 2);
  Serial.println(phAdequado ? " (adequado)" : " (inadequado)");
  Serial.print("Nutrientes: N:");Serial.print(npk[0].ativo ? "ON" : "OFF");
  Serial.print(" P:");           Serial.print(npk[1].ativo ? "ON" : "OFF");
  Serial.print(" K:");           Serial.println(npk[2].ativo ? "ON" : "OFF");

  // Decisão
  if (!soloSeco) {
    Serial.println("Solo umido. Irrigacao desligada.");
    setIrrigacao(false);
    return;
  }

  Serial.println("Solo seco, verificando...");

  if (phAdequado && npkAdequado) {
    Serial.println(">> Irrigacao ATIVADA.");
    setIrrigacao(true);
  } else {
    Serial.print(">> Irrigacao bloqueada: ");
    if (!phAdequado && !npkAdequado) Serial.println("pH e nutrientes inadequados.");
    else if (!phAdequado)            Serial.println("pH inadequado.");
    else                             Serial.println("nutrientes insuficientes.");
    setIrrigacao(false);
  }
}
