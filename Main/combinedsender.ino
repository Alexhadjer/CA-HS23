#include <Adafruit_BMP280.h>
#include <RH_ASK.h>
#include <Wire.h>
#include <AHT20.h>

AHT20 aht20;

Adafruit_BMP280 bmp280;
RH_ASK driver;

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("BMP280 example");

  Wire.begin(); // Join I2C bus
  bmp280.begin();
  driver.init();
}

void loop()
{
  // Read sensor values
  float temperature = bmp280.readTemperature();
  float pressure_hPa = bmp280.readPressure() / 100.0;
  float altitude = bmp280.readAltitude(1019.66);
  float humidity = aht20.getHumidity();
  // Print values to Serial monitor
  Serial.print(F("Temperature  = "));
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_hPa);
  Serial.println("  hPa");

  Serial.print(F("Approx altitude = "));
  Serial.print(altitude);
  Serial.println("  m");

   Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println("  %");

  // Convert values to strings
  char msgTemperature[10];
  char msgPressure[10];
  char msgAltitude[10];
  char msgHumidity[10];

  dtostrf(temperature, 4, 2, msgTemperature);
  dtostrf(pressure_hPa, 4, 2, msgPressure);
  dtostrf(altitude, 4, 2, msgAltitude);
  dtostrf(humidity, 4, 2, msgHumidity);
  const char msgValues[] = "TPAH";

  // Send sensor readings using RadioHead
  dtostrf(temperature, 4, 2, msgTemperature);
  dtostrf(pressure_hPa, 4, 2, msgPressure);
  dtostrf(altitude, 4, 2, msgAltitude);
  dtostrf(humidity, 4, 2, msgHumidity);

  // Concatenate all messages into one string
  char allMessages[32]; // Adjust the buffer size based on your needs
  sprintf(allMessages, "%s;%s;%s;%s;%s", msgValues, msgTemperature, msgPressure, msgAltitude, msgHumidity);

  // Send the combined message using RadioHead
  driver.send((uint8_t *)allMessages, strlen(allMessages));
  driver.waitPacketSent();


  Serial.println();
  delay(2000);
}
