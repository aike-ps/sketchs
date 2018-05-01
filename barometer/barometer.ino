#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // Устанавливаем дисплей
Adafruit_BME280 bme;

unsigned long delayTime;

void setup() {
	lcd.init();                     
	lcd.backlight();
	lcd.print("Tmp Pres Hudm");
    Serial.begin(9600);
    Serial.println(F("BME280 test"));
    bool status;
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("-- Default Test --");
    delayTime = 1000;
    Serial.println();
}


void loop() { 
    printValues();
    delay(delayTime);
}


void printValues() {
	lcd.setCursor(0, 1);
	int tmp = round(bme.readTemperature()); 
	int pres = round(bme.readPressure() / 133.3); 
	int hud = round(bme.readHumidity());
	
	//float tmp = bme.readTemperature(); 
	//float pres = bme.readPressure() / 133.33; 
	//float hud = bme.readHumidity();
	
	lcd.print(tmp);
	lcd.print("  ");
	lcd.print(pres);
	lcd.print("  ");
	lcd.print(hud);
	lcd.print(" ");
	
}