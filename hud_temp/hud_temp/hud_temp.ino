#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
#include <iarduino_OLED_txt.h>

iarduino_OLED_txt myOLED(0x3C);
extern uint8_t MediumFontRus[];

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
  myOLED.begin();
  myOLED.setFont(MediumFontRus);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) {
    Serial.println("Error reading from DHT");
  } else {
    myOLED.print(F("  Tm: "), 0,     2);
    myOLED.print(int(t));
    myOLED.print(F("'C"));
    myOLED.print(F("  Hd: "), 0,     6);
    myOLED.print(int(h));
    myOLED.print(F("%"));
    
    delay (2000);
    //myOLED.clrScr();
  }

  delay(1000);
}
