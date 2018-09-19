
#include <Wire.h>
#include "SSD1306Wire.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "reading_t.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define AMOUNT_OF_HISTORY_ENTRIES 15

SSD1306Wire display(0x3c, D2, D1);
Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);
  Serial.println("\n\nBaromether + Display");

  Serial.println("Init display");

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);

  Serial.println("Init BME280");

  if ( bme.begin(0x76) == false ) {
    String error = "No BME280 found :-(";

    display.drawStringMaxWidth(0, 0, 128, error);
    Serial.println(error);
  }

  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X8, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);

  display.display();
}

reading_t reading;
reading_t readingHistory[AMOUNT_OF_HISTORY_ENTRIES];

unsigned int counter = 0;
unsigned int lineDistance = 10;
unsigned int amountStoreadHistoryEntries = 0;

void loop() {

  if ( counter == 0 || counter > 10 ) {
    counter = 0;

    bme.takeForcedMeasurement();

    reading.temperature = bme.readTemperature();
    reading.pressure = (bme.readPressure() / 100.0F);
    reading.humidity = bme.readHumidity();

    /*
       History management stuff
    */
    if ( amountStoredHistoryEntries == AMOUNT_OF_HISTORY_ENTRIES ) {
      // delete first reading, move every reading one down, add the new reading
      for (int i = 1; i < AMOUNT_OF_HISTORY_ENTRIES
    }

    historyReadings[historyPosition] = reading;

    Serial.println(reading.temperature);
    Serial.println(reading.pressure);
    Serial.println(reading.humidity);
  }

  display.clear();
  display.drawStringMaxWidth(0, lineDistance * 0, 128, formatTemperatureString(reading));
  display.drawStringMaxWidth(0, lineDistance * 1, 128, formatPressureString(reading));
  display.drawStringMaxWidth(0, lineDistance * 2, 128, formatHumidityString(reading));
  /*  display.drawStringMaxWidth(0, lineDistance * 3, 128, "Diff: " + substractAndFormat(temperature, lastTemperature, "°C") + ", " +
                               substractAndFormat(pressure, lastPressure, "hPa") + ", " +
                               substractAndFormat(humidity, lastHumidity, "%"));
  */
  display.drawStringMaxWidth(0, lineDistance * 5, 128, "Update: " + (String)counter + " seconds ago");

  display.display();

  delay(1000);

  counter++;
}

String formatTemperatureString(reading_t reading) {
  return "Temperature: " + (String)reading.temperature + " °C";
}

String formatPressureString(reading_t reading) {
  return "Luftdruck: " + (String)reading.pressure + " hPa";
}

String formatHumidityString(reading_t reading) {
  return "Luftfeuchte: " + (String)reading.humidity + " %";
}

String substractAndFormat(float var1, float var2, String unit) {
  if ( var1 == var2 ) {
    return "0 " + unit;
  }
  else {
    int diff = (int)var1 - var2;

    if ( diff > 0 ) {
      return "+" + String(diff) + " " + unit;
    }
    else {
      return (String)diff + " " + unit;
    }
  }

}
