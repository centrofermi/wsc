// vim: filetype=cpp

#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

auto constexpr PID = 0x0EEE;
auto constexpr speed = 9600;

void setup() {
  Serial.begin(speed);
  auto const bme_status = bme.begin();

  if (bme_status == 0) {
    Serial.println("BME error");
  }
}

void loop() {
  if (Serial.available() > 0) {
    auto const order = Serial.read();

    if (order == 't') {
      Serial.print("T = "); Serial.println(bme.readTemperature());
    } else if (order == 'h') {
      Serial.print("H = "); Serial.println(bme.readHumidity());
    } else if (order == 'p') {
      Serial.print("P = "); Serial.println(bme.readPressure());
    } else if (order == 'e') {
      Serial.println(PID, HEX);
    } else {
      Serial.println(
          "Usage:\n"                 \
          " - 't' for inside temperature\n" \
          " - 'h' for humidity\n"    \
          " - 'p' for pressure"
      );
    }
  }
}
