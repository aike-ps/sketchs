#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
#define LED_BLUE  5
#define LED_RED  6
#define LED_GREEN  7
#define BUTTON  16

unsigned long startTime = millis();

/*

  co2 - A0, A1
  dust - D15,a13
  button - D16
  led - D5 B, D6 R, D7 G
  dht - D2
  lcd - D8-D12

  CO2
    RX  18
    TX  16
    PWM 20
*/

#include <Nokia_LCD.h>
Nokia_LCD lcd(8/* CLK */, 9 /* DIN */, 10 /* DC */, 11 /* CE */, 12 /* RST */);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  dht.begin();
  Serial.begin(9600);
  lcd.begin();
  lcd.setContrast(50);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading... ");

  ledRed();
  delay(1000);
  ledYellow();
  delay(1000);
  ledGreen();
  delay(1000);

  Serial3.begin(9600); // RX 14, TX 15

  // byte autoCaibrationOnCmd[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
  // Serial3.write(autoCaibrationOnCmd, 9); //request PPM CO2

  //  byte autoCaibrationOffCmd[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};
  //  Serial3.write(autoCaibrationoOffCmd, 9); //request PPM CO2

  delay(2000);
}

void loop() {

  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int ppm_uart = readCO2UART();

  Serial.print("Hud ");
  Serial.print(h);
  Serial.print("Tmp ");
  Serial.println(t);
  Serial.print("CO2 ");
  Serial.println(ppm_uart);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("|Hud: ");
  lcd.print(h);
  lcd.setCursor(0, 2);
  lcd.print("CO2: ");
  lcd.print(ppm_uart);
  lcd.println("   ");

  if (ppm_uart <= 800) {
    ledGreen();
  }

  if (ppm_uart <= 1200 and ppm_uart > 800) {
    ledYellow();
  }

  if (ppm_uart >= 1200 and ppm_uart <= 1500 ) {
    ledRed();
  }

  if (ppm_uart > 1500) {
    delay(250);
    ledOff();
    delay(250);
    ledRed();
    delay(250);
    ledOff();
    delay(250);
    ledRed();
    delay(250);
    ledOff();
    delay(250);
    ledRed();
    delay(250);
    ledOff();
    delay(250);
    ledRed();
  }

  delay(5000);
}

void ledOff()
{
  analogWrite(LED_RED, 0);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 0);
}

void ledRed()
{
  analogWrite(LED_RED, 250);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 0);
}

void ledGreen()
{
  analogWrite(LED_RED, 0);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 250);
}

void ledYellow()
{
  analogWrite(LED_RED, 120);
  analogWrite(LED_BLUE, 40);
  analogWrite(LED_GREEN, 125);
}

int readCO2UART() {

  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  byte response[9] = {0}; // for answer
  Serial3.write(cmd, 9); //request PPM CO2
  Serial3.readBytes(response, 9);
  // ppm
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  int ppm_uart = (256 * responseHigh) + responseLow;
  Serial.print("PPM UART: ");
  Serial.println(ppm_uart);

  return ppm_uart;
}
