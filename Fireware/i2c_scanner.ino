/*
  I2C Scanner - diagnostico simple
  No usa ninguna libreria de sensores, solo Wire.
  Barre todas las direcciones I2C posibles y reporta cuales responden.

  Direcciones esperadas:
    BMP280  -> 0x76 o 0x77
    MPU6050 -> 0x68 o 0x69
*/

#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial) { delay(10); }
  Serial.println("\n=== I2C Scanner ===");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Escaneando bus I2C...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo encontrado en 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Error desconocido en 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No se encontro ningun dispositivo I2C.");
  } else {
    Serial.println("Escaneo completo.\n");
  }

  delay(3000); // repite cada 3 segundos
}

