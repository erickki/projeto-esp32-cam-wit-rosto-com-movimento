#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

// ============================================================
// I2C CUSTOM (ESP32-CAM)
// ============================================================
#define SDA_PIN 14
#define SCL_PIN 15

// ============================================================
// DISPLAY
// ============================================================
#define LARGURA 128
#define ALTURA 64

Adafruit_SSD1306 display(LARGURA, ALTURA, &Wire, -1);

// ============================================================
// SENSOR ADXL345
// ============================================================
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// ============================================================
// ROSTO
// ============================================================
#define ROSTO_CX   64
#define ROSTO_CY   32
#define ROSTO_R    30

#define OLHO_OFFSET_Y   -9
#define OLHO_DIST_X     11
#define ORBITA_R         7
#define PUPILA_R         3
#define MOVIMENTO_MAX    4

#define BOCA_OFFSET_Y   14
#define BOCA_LARGURA    13
#define BOCA_ALTURA      5

#define SENSIBILIDADE  1.5

#define INTERVALO_PISCAR  5000
#define TEMPO_PISCAR       150

bool piscando = false;
unsigned long ultimaPisca = 0;
unsigned long inicioPisca = 0;

// ============================================================

float limitar(float valor, float minVal, float maxVal) {
  if (valor < minVal) return minVal;
  if (valor > maxVal) return maxVal;
  return valor;
}

void desenharOlho(int cx, int cy, bool fechado, int px, int py) {
  if (fechado) {
    display.drawFastHLine(cx - ORBITA_R, cy, ORBITA_R * 2, SSD1306_WHITE);
  } else {
    display.fillCircle(cx, cy, ORBITA_R, SSD1306_WHITE);
    display.fillCircle(px, py, PUPILA_R, SSD1306_BLACK);
  }
}

// ============================================================

void setup() {
  Serial.begin(115200);

  // I2C custom
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED não encontrado!");
    while (true);
  }

  // ADXL345
  if (!accel.begin()) {
    Serial.println("ADXL345 não encontrado!");
    while (true);
  }

  accel.setRange(ADXL345_RANGE_2_G);

  ultimaPisca = millis();
}

// ============================================================

void loop() {
  unsigned long agora = millis();

  // Piscada
  if (!piscando && (agora - ultimaPisca >= INTERVALO_PISCAR)) {
    piscando = true;
    inicioPisca = agora;
  }

  if (piscando && (agora - inicioPisca >= TEMPO_PISCAR)) {
    piscando = false;
    ultimaPisca = agora;
  }

  // Leitura ADXL345
  sensors_event_t event;
  accel.getEvent(&event);

  float ax = -(event.acceleration.y / 9.8) * SENSIBILIDADE;
  float ay = -(event.acceleration.x / 9.8) * SENSIBILIDADE;

  ax = limitar(ax, -1.0, 1.0);
  ay = limitar(ay, -1.0, 1.0);

  float dx = ax * MOVIMENTO_MAX;
  float dy = ay * MOVIMENTO_MAX;

  float dist = sqrt(dx * dx + dy * dy);
  if (dist > MOVIMENTO_MAX) {
    dx = dx / dist * MOVIMENTO_MAX;
    dy = dy / dist * MOVIMENTO_MAX;
  }

  int olhoEsqX = ROSTO_CX - OLHO_DIST_X;
  int olhoDirX = ROSTO_CX + OLHO_DIST_X;
  int olhoY = ROSTO_CY + OLHO_OFFSET_Y;

  int bocaCY = ROSTO_CY + BOCA_OFFSET_Y;
  int limiteInferior = (bocaCY - PUPILA_R - 1) - olhoY;

  float dyFinal = (dy > limiteInferior) ? limiteInferior : dy;

  int pEsqX = olhoEsqX + (int)dx;
  int pEsqY = olhoY + (int)dyFinal;
  int pDirX = olhoDirX + (int)dx;
  int pDirY = olhoY + (int)dyFinal;

  display.clearDisplay();

  display.drawCircle(ROSTO_CX, ROSTO_CY, ROSTO_R, SSD1306_WHITE);

  desenharOlho(olhoEsqX, olhoY, piscando, pEsqX, pEsqY);
  desenharOlho(olhoDirX, olhoY, piscando, pDirX, pDirY);

  for (int i = -BOCA_LARGURA; i <= BOCA_LARGURA; i++) {
    int curva = (i * i * BOCA_ALTURA) / (BOCA_LARGURA * BOCA_LARGURA);
    int xb = ROSTO_CX + i;
    int yb = bocaCY + BOCA_ALTURA - curva;
    display.drawPixel(xb, yb, SSD1306_WHITE);
  }

  display.display();
  delay(20);
}