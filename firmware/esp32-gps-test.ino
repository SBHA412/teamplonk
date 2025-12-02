#include <TinyGPSPlus.h>

TinyGPSPlus gps;
HardwareSerial GPSserial(1);   // use UART1, NOT 2

static const int RXPin = 17;   // ESP32 RX  (goes to GPS TX)
static const int TXPin = 16;   // ESP32 TX  (goes to GPS RX)
static const uint32_t GPSBaud = 9600;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32-C6 GPS Test Starting...");
  Serial.println("Waiting for NMEA sentences...");

  GPSserial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
}

void loop() {
  while (GPSserial.available() > 0) {
    char c = GPSserial.read();
    Serial.print(c);
    gps.encode(c);
  }

  if (gps.location.isUpdated()) {
    Serial.println("\n----- GPS FIX -----");
    Serial.print("Lat: "); Serial.println(gps.location.lat(), 6);
    Serial.print("Lng: "); Serial.println(gps.location.lng(), 6);
    Serial.print("Alt: "); Serial.println(gps.altitude.meters());
    Serial.print("Satellites: "); Serial.println(gps.satellites.value());
  }
}
