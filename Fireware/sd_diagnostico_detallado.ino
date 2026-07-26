/*
  Diagnostico SD detallado (bajo nivel) - v2 simplificado
  Usa Sd2Card directamente para obtener informacion mas especifica
  sobre por que falla la inicializacion. Sin SdVolume (no disponible
  en todas las versiones de la libreria SD).
*/

#include <SPI.h>
#include <SD.h>
#include <utility/Sd2Card.h>

const int SD_CS_PIN = 10;

Sd2Card card;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(1000);

  Serial.println(F("=== Diagnostico SD detallado ==="));

  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);

  Serial.println(F("Inicializando tarjeta (SPI_HALF_SPEED)..."));
  if (!card.init(SPI_HALF_SPEED, SD_CS_PIN)) {
    Serial.println(F("FALLO en card.init()"));
    Serial.println(F("Esto es un error a nivel MUY bajo. Causas tipicas:"));
    Serial.println(F("1. Conexion fisica (revisar continuidad de cada cable)"));
    Serial.println(F("2. Modulo SD danado"));
    Serial.println(F("3. Alimentacion insuficiente (probar con 3.3V en vez de 5V)"));
    Serial.println(F("4. Tarjeta danada/incompatible"));
    return;
  }
  Serial.println(F("card.init() OK - la tarjeta responde electricamente."));

  Serial.print(F("Tipo de tarjeta: "));
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println(F("SD1"));
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println(F("SD2"));
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println(F("SDHC"));
      break;
    default:
      Serial.println(F("Desconocido"));
  }

  Serial.println(F("Ahora probando SD.begin() (capa alta, con sistema de archivos)..."));
  if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    Serial.println(F("SD.begin() FALLO."));
    Serial.println(F("La tarjeta responde a nivel electrico (card.init() OK arriba),"));
    Serial.println(F("pero el sistema de archivos no es FAT16/FAT32 valido."));
    Serial.println(F("-> Hace falta formatearla en FAT32."));
  } else {
    Serial.println(F("SD.begin() OK. Todo funcionando."));
  }
}

void loop() {
}
