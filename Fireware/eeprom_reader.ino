/*
  ============================================================
  FRONTERA 1 - Lector de EEPROM (post-vuelo)
  ============================================================
  Subir este sketch DESPUES del vuelo (con el Arduino ya
  desconectado del cohete) para leer el resumen que quedo
  guardado en la EEPROM durante el vuelo.

  IMPORTANTE: subir este sketch DESPUES de aterrizado, nunca
  antes, ya que al subir un sketch nuevo la EEPROM no se borra
  (persiste), pero conviene no arriesgarse a pisar los datos.
  ============================================================
*/

#include <EEPROM.h>

const int EEPROM_ADDR = 0;
const uint32_t MAGIC_NUMBER = 0xF12A2026;

struct FlightSummary {
  uint32_t magicNumber;
  float maxAltitude;
  float maxAccelMagnitude;
  float maxGyroMagnitude;
  float avgTemperature;
  unsigned long flightDurationMs;
  uint32_t sampleCount;
};

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  delay(500);

  FlightSummary summary;
  EEPROM.get(EEPROM_ADDR, summary);

  Serial.println(F("=== FRONTERA 1 - Resumen de vuelo (EEPROM) ==="));

  if (summary.magicNumber != MAGIC_NUMBER) {
    Serial.println(F("No hay datos validos en la EEPROM (magic number no coincide)."));
    Serial.println(F("Puede que el sketch de vuelo nunca haya corrido, o la EEPROM este vacia/corrupta."));
    return;
  }

  Serial.print(F("Apogeo (altitud maxima): "));
  Serial.print(summary.maxAltitude, 2);
  Serial.println(F(" m"));

  Serial.print(F("Aceleracion maxima: "));
  Serial.print(summary.maxAccelMagnitude, 2);
  Serial.println(F(" m/s^2"));

  Serial.print(F("Velocidad angular maxima: "));
  Serial.print(summary.maxGyroMagnitude, 3);
  Serial.println(F(" rad/s"));

  Serial.print(F("Temperatura promedio: "));
  Serial.print(summary.avgTemperature, 2);
  Serial.println(F(" C"));

  Serial.print(F("Duracion total registrada: "));
  Serial.print(summary.flightDurationMs / 1000.0, 2);
  Serial.println(F(" s"));

  Serial.print(F("Cantidad de muestras tomadas: "));
  Serial.println(summary.sampleCount);
}

void loop() {
}

