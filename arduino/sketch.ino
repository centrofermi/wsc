// vim: filetype=cpp

#include <Adafruit_BME280.h>
#include <DallasTemperature.h>

Adafruit_BME280 bme;
OneWire owds(6);
DallasTemperature ds(&owds);

auto constexpr PID = 0x0EEE;
auto constexpr speed = 9600;

void setup() {
  Serial.begin(speed);
  auto const bme_status = bme.begin();
  ds.begin();

  if (bme_status == 0) {
    Serial.println("BME error");
  }

  auto const ds_count = ds.getDeviceCount();

  if (ds_count == 0) {
    Serial.println("DS18B20 error");
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
    } else if (order == 'o') {
      ds.requestTemperaturesByIndex(0);
      Serial.print("O = "); Serial.println(ds.getTempCByIndex(0));
    } else {
      Serial.println(
          "Usage:\n"                 \
          " - 't' for inside temperature\n" \
          " - 'o' for outside temperature\n" \
          " - 'h' for humidity\n"    \
          " - 'p' for pressure"
      );
    }
  }
}
