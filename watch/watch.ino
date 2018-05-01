#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "U8glib.h"
#include <DS3232RTC.h>
#include <TimeLib.h>
#include <EEPROM.h>

U8GLIB_ST7920_128X64_1X u8g(13, 12, 11);

Adafruit_BMP085 dps;

DS3232RTC  rtc(true);
long now_hour = 0;
long Temperature = 0, Pressure = 0;
int pwm = 3;
unsigned long temp = 0;
unsigned long temp2 = 0;
unsigned long pres = 0;
unsigned long hydim = 0;
int counter = 0;
int counter2 = 0;
int light = 50;
int pressure_counter = 0;
int pressureArray[128];
long previousMillis = 0;
long interval = 1;
tmElements_t tm;

int sensor = A0;
int key = A1;
int backlight = 3;

int current = 128;


void setup()
{
  pinMode(backlight, OUTPUT);

  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet) {
    Serial.println("Unable to sync with the RTC");
  }
  else {
    Serial.println("RTC has set the system time");
  }
 	
  Serial.begin(9600);
  boot_screen();
  setup_bmp();
  getData();
  //clearEEPROM();
  //readFromEEPROM();
  updatePressure();
  show_data();
  previousMillis = tm.Second;
  //set_time();
  pinMode(key, INPUT);
  pinMode(sensor, INPUT);
}

void loop()
{
  RTC.read(tm);

  if (counter == 30) {
    getData();
    counter = 0;
    show_data();
    setLight();
  }

  int currentMillis = tm.Second;

  if (currentMillis == 0) {
    previousMillis = 0;
    show_data();
    counter++;
    pressure_counter++;
  }

  if (currentMillis - previousMillis >= interval) {
    show_data();
    counter++;
    previousMillis = currentMillis;
    pressure_counter++;
  }

  if (pressure_counter == 1800) {
    updatePressure();
	//readFromEEPROM();
	//writeToEEPROM();
    pressure_counter = 0;
    show_data();
  }
  
}

void updatePressure()
{
  int i;
  int tmp_val = pres - 730;
  for (i = 1; i < 128; i++ ) {
    
	if (i == 127) {
      pressureArray[i] = tmp_val;
    } else {
      pressureArray[i] = pressureArray[i + 1];
    }
	
    if (pressureArray[i] > 30) {
      pressureArray[i] = 31;
    }

    if (pressureArray[i] < 1) {
      pressureArray[i] = 1;
    }
	
  }
}

void writeToEEPROM()
{
	for (int i = 0; i < 128; i++ ) {
		EEPROM.put(i, pressureArray[i]);
		Serial.print(i);
		Serial.print(" ");
		Serial.print(pressureArray[i]);
		Serial.print(" ");
		EEPROM.get(i, pressureArray[i]);
		Serial.println(pressureArray[i]);
	} 
}

void clearEEPROM()
{
	for (int i = 0; i < 128; i++ ) {
		EEPROM.put(i, 0);
	} 
}

void readFromEEPROM()
{
	for (int i = 0; i < 128; i++ ) {
		EEPROM.get(i, pressureArray[i]);
		Serial.print(i);
		Serial.print(" ");
		Serial.print(pressureArray[i]);
		Serial.print(" ");
		EEPROM.get(i, pressureArray[i]);
		Serial.println(pressureArray[i]);
	} 
}

void boot_screen()
{
  u8g.setColorIndex(3);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_8x13B);
    u8g.drawStr( 0, 10, "Loading...");
  } while ( u8g.nextPage() );
  setLight();
}

void setup_bmp()
{
  Wire.begin();
  delay(1000);
  dps.begin();
  getData();
  RTC.read(tm);


}

void show_data()
{
  int hour = tm.Hour;
  int minute = tm.Minute;
  int second = tm.Second;
  int year = tm.Year + 1970;
  int month = tm.Month;
  int day = tm.Day;

  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_8x13B);
    u8g.setScale2x2();
    u8g.setPrintPos(0, 10);
    if (hour < 10) {
      u8g.print(0);
    }
    u8g.print(hour);
    u8g.print(":");
    if (minute < 10) {
      u8g.print(0);
    }
    u8g.print(minute);
    u8g.print(":");
    if (second < 10) {
      u8g.print(0);
    }
    u8g.print(second);
    u8g.undoScale();
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(0, 31);
    u8g.print(year);
    u8g.print(".");
    if (month < 10) {
      u8g.print(0);
    }
    u8g.print(month);
    u8g.print(".");

    if (day < 10) {
      u8g.print(0);
    }
    u8g.print(day);
    u8g.print(" ");
    u8g.print( temp );
    u8g.write(0xBA);
    u8g.print("C");
    u8g.print(" ");
    u8g.print( pres );
    u8g.print("mm");
    int i;
    for (i = 0; i < 128; i++ ) {
      u8g.drawLine(i, 64, i, 64 -  pressureArray[i]);
    }
    u8g.drawLine(0, 32, 128, 32);

  } while ( u8g.nextPage() );
  setLight();
}


void getData()
{
  temp = dps.readTemperature();
  pres = dps.readPressure() / 133.3;

}

void setLight()
{
  int a = analogRead(sensor);

  if (a > 700) {
    a = 700;
  }

  if (a < 20) {
    a = 20;
  }
 
  current = map(a, 20, 700, 2, 254);
  analogWrite(backlight, current);

}

void set_time()
{	  
    RTC.read(tm);
	tm.Hour = 21;
	//time.Minute = minute;
	//time.Second = second;
	rtc.write(tm);
}


void sound_beep()
{
  //int i = 0;
  //for (i; i < 3; i++) {
  tone(2, 1000, 100);
  delay(100);
  tone(2, 500, 50);
  delay(50);
  tone(2, 1000, 100);
  delay(100);
  //noTone(2);
  //}
}