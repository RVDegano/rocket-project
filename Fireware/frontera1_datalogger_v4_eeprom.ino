/*
  ============================================================
  FRONTERA 1 - Data Logger de vuelo (v4 - respaldo EEPROM, sin SD)
  ============================================================
  Plan B: como el modulo SD no funciona, este sketch guarda un
  RESUMEN del vuelo en la EEPROM interna del Arduino Nano (1KB),
  en vez del log completo a 20Hz.

  Que guarda (resumen, no serie temporal completa):
    - Apogeo (altitud maxima alcanzada)
    - Aceleracion maxima (magnitud del vector accel)
    - Duracion total del vuelo detectado
    - Temperatura promedio
    - Orientacion (gyro) maxima detectada

  Como funciona:
    - Arranca a loguear en RAM apenas se prende (como el original)
    - Cada muestra actualiza los maximos/minimos en RAM
    - Cada 2 segundos, graba el resumen actual a EEPROM
      (asi si se corta la alimentacion en cualquier momento,
      lo ultimo grabado sigue en la EEPROM)
    - Para LEER los datos despues del vuelo: subir el sketch
      "eeprom_reader.ino" (lo armamos aparte) que solo lee e
      imprime los valores guardados, sin necesidad de tocar
      el sketch de vuelo

  Conexiones (sin cambios, SD ya no se usa):
    BMP280  SDA -> A4      SCL -> A5
    MPU6050 SDA -> A4      SCL -> A5   (mismo bus I2C)
    Todos VCC -> 5V, todos GND -> GND
  ============================================================
*/

#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

const unsigned long SAMPLE_INTERVAL_MS = 50;   // 20 Hz
const unsigned long EEPROM_SAVE_INTERVAL_MS = 2000; // graba a EEPROM cada 2s
const int LED_PIN = LED_BUILTIN;

// Direccion base en EEPROM donde se guarda el resumen
const int EEPROM_ADDR = 0;

// ---------- MPU6050 (registros, sin libreria) ----------
uint8_t mpuAddr = 0x68;
#define MPU_PWR_MGMT_1   0x6B
#define MPU_ACCEL_CONFIG 0x1C
#define MPU_GYRO_CONFIG  0x1B
#define MPU_ACCEL_XOUT_H 0x3B
const float ACCEL_SCALE = 2048.0;
const float GYRO_SCALE  = 65.5;
const float GRAVITY     = 9.80665;
const float GYRO_DEG_TO_RAD = 0.0174533;

Adafruit_BMP280 bmp;

bool bmpOk = false;
bool mpuOk = false;

float seaLevelPressure = 1013.25;

unsigned long lastSampleTime = 0;
unsigned long lastEepromSave = 0;
unsigned long startTime = 0;

// ---------- Estructura del resumen guardado en EEPROM ----------
struct FlightSummary {
  uint32_t magicNumber;     // para verificar que la EEPROM tiene datos validos
  float maxAltitude;
  float maxAccelMagnitude;
  float maxGyroMagnitude;
  float avgTemperature;
  unsigned long flightDurationMs;
  uint32_t sampleCount;
};

FlightSummary summary;
float tempSum = 0;

const uint32_t MAGIC_NUMBER = 0xF12A2026; // "Frontera 1, 2026" como marca

// ---------- Funciones auxiliares MPU6050 ----------
bool mpuWriteReg(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return (Wire.endTransmission() == 0);
}

bool mpuProbe(uint8_t addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

bool mpuInit() {
  if (mpuProbe(0x68)) {
    mpuAddr = 0x68;
  } else if (mpuProbe(0x69)) {
    mpuAddr = 0x69;
  } else {
    return false;
  }
  if (!mpuWriteReg(mpuAddr, MPU_PWR_MGMT_1, 0x00)) return false;
  delay(50);
  mpuWriteReg(mpuAddr, MPU_ACCEL_CONFIG, 0x18);
  mpuWriteReg(mpuAddr, MPU_GYRO_CONFIG, 0x08);
  return true;
}

bool mpuRead(float &ax, float &ay, float &az, float &gx, float &gy, float &gz) {
  Wire.beginTransmission(mpuAddr);
  Wire.write(MPU_ACCEL_XOUT_H);
  if (Wire.endTransmission(false) != 0) return false;
  uint8_t bytesReceived = Wire.requestFrom(mpuAddr, (uint8_t)14);
  if (bytesReceived < 14) return false;
  int16_t rawAx = (Wire.read() << 8) | Wire.read();
  int16_t rawAy = (Wire.read() << 8) | Wire.read();
  int16_t rawAz = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();
  int16_t rawGx = (Wire.read() << 8) | Wire.read();
  int16_t rawGy = (Wire.read() << 8) | Wire.read();
  int16_t rawGz = (Wire.read() << 8) | Wire.read();
  ax = (rawAx / ACCEL_SCALE) * GRAVITY;
  ay = (rawAy / ACCEL_SCALE) * GRAVITY;
  az = (rawAz / ACCEL_SCALE) * GRAVITY;
  gx = (rawGx / GYRO_SCALE) * GYRO_DEG_TO_RAD;
  gy = (rawGy / GYRO_SCALE) * GYRO_DEG_TO_RAD;
  gz = (rawGz / GYRO_SCALE) * GYRO_DEG_TO_RAD;
  return true;
}

void saveSummaryToEeprom() {
  EEPROM.put(EEPROM_ADDR, summary);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.println(F("=== FRONTERA 1 - Data Logger v4 (EEPROM, sin SD) ==="));

  Wire.begin();

  Serial.print(F("Iniciando BMP280... "));
  if (bmp.begin(0x76) || bmp.begin(0x77)) {
    bmpOk = true;
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_1);
    Serial.println(F("OK"));
  } else {
    Serial.println(F("FALLO - revisar conexion I2C"));
  }

  Serial.print(F("Iniciando MPU6050... "));
  if (mpuInit()) {
    mpuOk = true;
    Serial.print(F("OK (direccion 0x"));
    Serial.print(mpuAddr, HEX);
    Serial.println(F(")"));
  } else {
    Serial.println(F("FALLO - revisar conexion I2C"));
  }

  if (bmpOk) {
    delay(500);
    seaLevelPressure = bmp.readPressure() / 100.0F;
    Serial.print(F("Presion de referencia (suelo): "));
    Serial.print(seaLevelPressure);
    Serial.println(F(" hPa"));
  }

  // --- Inicializar resumen en RAM ---
  summary.magicNumber = MAGIC_NUMBER;
  summary.maxAltitude = -1000;
  summary.maxAccelMagnitude = 0;
  summary.maxGyroMagnitude = 0;
  summary.avgTemperature = 0;
  summary.flightDurationMs = 0;
  summary.sampleCount = 0;
  tempSum = 0;

  if (!bmpOk && !mpuOk) {
    Serial.println(F("ADVERTENCIA: ningun sensor disponible."));
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  } else {
    Serial.println(F("Todo OK. Iniciando logging a EEPROM (resumen)..."));
    digitalWrite(LED_PIN, HIGH);
  }

  startTime = millis();
  lastSampleTime = startTime;
  lastEepromSave = startTime;
}

void loop() {
  unsigned long now = millis();

  if (now - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = now;
    processSample(now - startTime);
  }

  // Grabar a EEPROM periodicamente (no en cada muestra, para no
  // desgastar la EEPROM: tiene vida util limitada de ~100,000 escrituras)
  if (now - lastEepromSave >= EEPROM_SAVE_INTERVAL_MS) {
    lastEepromSave = now;
    saveSummaryToEeprom();
  }
}

void processSample(unsigned long elapsedMs) {
  float altitude = NAN, temperature = NAN;
  float ax = NAN, ay = NAN, az = NAN, gx = NAN, gy = NAN, gz = NAN;

  if (bmpOk) {
    temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F;
    altitude = 44330.0 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
  }

  if (mpuOk) {
    mpuRead(ax, ay, az, gx, gy, gz);
  }

  // --- Actualizar maximos/resumen ---
  if (bmpOk) {
    if (altitude > summary.maxAltitude) summary.maxAltitude = altitude;
    tempSum += temperature;
  }

  if (mpuOk) {
    float accelMag = sqrt(ax * ax + ay * ay + az * az);
    float gyroMag = sqrt(gx * gx + gy * gy + gz * gz);
    if (accelMag > summary.maxAccelMagnitude) summary.maxAccelMagnitude = accelMag;
    if (gyroMag > summary.maxGyroMagnitude) summary.maxGyroMagnitude = gyroMag;
  }

  summary.sampleCount++;
  summary.flightDurationMs = elapsedMs;
  if (summary.sampleCount > 0) {
    summary.avgTemperature = tempSum / summary.sampleCount;
  }

  // --- Debug por Serial (para ver en vivo mientras esta conectado por USB) ---
  Serial.print(F("t="));
  Serial.print(elapsedMs);
  Serial.print(F("ms alt="));
  Serial.print(altitude, 2);
  Serial.print(F("m accel="));
  Serial.print(sqrt(ax * ax + ay * ay + az * az), 2);
  Serial.println(F("m/s2"));
}

