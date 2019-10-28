#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22

/*

  co2 - A0, A1
  dust - D15,a13
  button - D14
  led - D5 B, D6 R, D7 G
  dht - D2
  lcd - D8-D12

*/

#include <Nokia_LCD.h>
Nokia_LCD lcd(8/* CLK */, 9 /* DIN */, 10 /* DC */, 11 /* CE */, 12 /* RST */);

DHT dht(DHTPIN, DHTTYPE);

#include <SoftwareSerial.h>
#include <MHZ.h>
#define MH_Z19_RX 31  // D7
#define MH_Z19_TX 30  // D6

MHZ co2(MH_Z19_RX, MH_Z19_TX, 0, MHZ19B);

void setup() {

  dht.begin();
  Serial.begin(9600);

  lcd.begin();
  lcd.setContrast(50);
  lcd.clear();
  delay(2000);

  if (co2.isPreHeating()) {
    Serial.print("Preheating");
   // while (co2.isPreHeating()) {
      Serial.print(".");
      delay(5000);
   // }
    Serial.println();
  }
}

void loop() {
  lcd.clear();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Hud ");
  Serial.println(h);
  Serial.print("Tmp ");
  Serial.println(t);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Hud: ");
  lcd.print(h);

  Serial.print("\n----- Time from start: ");
  Serial.print(millis() / 1000);
  Serial.println(" s");

  int ppm_uart = co2.readCO2UART();
  Serial.print("PPMuart: ");

  if (ppm_uart > 0) {
    Serial.print(ppm_uart);
  } else {
    Serial.print("n/a");
  }

  int ppm_pwm = co2.readCO2PWM();
  Serial.print(", PPMpwm: ");
  Serial.print(ppm_pwm);
  lcd.setCursor(0, 2);
  lcd.print("co2: ");
  lcd.print(ppm_pwm);
  int temperature = co2.getLastTemperature();
  Serial.print(", Temperature: ");

  if (temperature > 0) {
    Serial.println(temperature);
  } else {
    Serial.println("n/a");
  }

  Serial.println("\n------------------------------");
  //delay(5000);

  delay(10000);
}
