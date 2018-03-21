#include <Arduino.h>
#include <DS1307.h>
#include <Wire.h>
#include <BMP085.h>
#include <SHT2x.h>
#include "U8glib.h"

//U8GLIB_ST7920_128X64_4X u8g(SCK = E, MOSI = R/W, CS = RS);
//U8GLIB_ST7920_128X64_4X u8g(E,R/W,RS);
//U8GLIB_ST7920_128X64_4X u8g(серый,черный,белый);
U8GLIB_ST7920_128X64_1X u8g(13, 11, 10);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
DS1307 rtc(4, 5);
BMP085 dps = BMP085();
Time  t;

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

long previousMillis = 0;        // храним время последнего переключения светодиода
long interval = 1000;           // интервал между включение/выключением светодиода (1 секунда)

void setup()
{
  sound_beep();
  analogWrite(pwm, light);
  Serial.begin(9600);
  boot_screen();
  setup_bmp();
  getData();
  setLight();
}

void loop()
{

  /* if (analogRead(3) < 2)
    {
     if (light == 0) {
       light  = 200;
     }
     else {
       light -= 10;
     }
     analogWrite(pwm, light);
     delay(500);
    }
    else {
  */

  if (counter == 300) {
    getData();
    counter = 0;
    show_data();
    setLight();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    show_data();
    counter++;
    previousMillis = currentMillis;
  }
}

void boot_screen()
{
  u8g.setColorIndex(3);
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_8x13B);
    //u8g.setScale2x2();
    u8g.drawStr( 0, 10, "Loading...");
  } while ( u8g.nextPage() );
}


void setup_bmp()
{
  Wire.begin();
  delay(1000);
  dps.init(MODE_ULTRA_HIGHRES, 210, true);
  dumpRegisters();
  dps.dumpCalData();
  delay(5000);
  Serial.begin(9600);
  getData();
}

void show_data()
{
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_8x13B);
    u8g.setScale2x2();
    u8g.drawStr( 0, 10, rtc.getTimeStr());
    u8g.undoScale();
    u8g.setFont(u8g_font_6x12);
    u8g.drawStr( 0, 30, rtc.getDOWStr(FORMAT_SHORT));
    u8g.drawStr( 60, 30, rtc.getDateStr());
    u8g.drawStr( 0, 45, "T(*C)   H(%)   P(mm)");
    u8g.setPrintPos(0, 60);
    u8g.print( temp );
    u8g.print( "*     " );
    u8g.print( hydim );
    u8g.print( "%    ");
    u8g.print( pres );
    u8g.print("mm");
  } while ( u8g.nextPage() );
}




void getData()
{
  dps.getTemperature(&Temperature);
  temp = Temperature / 10;
  dps.getPressure(&Pressure);
  pres = Pressure / 133.3;
  hydim = SHT2x.GetHumidity();
  temp2 = SHT2x.GetTemperature();
  t = rtc.getTime();

}

void setLight()
{
  if (t.hour > 21) {
    light = 20;
  }
  else {
    if (t.hour < 7) {
      light = 20;
    }
    else {
      light = 200;
    }
  }
  if ( now_hour != t.hour) {
    analogWrite(pwm, light);
    now_hour = t.hour;
  }
}

void set_time()
{
  rtc.halt(false);
  rtc.setDOW(SUNDAY);        // Set Day-of-Week to SUNDAY
  rtc.setTime(10, 10, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(9, 7, 2016);   // Set the date to October 3th, 2010
}

void dumpRegisters()
{
  byte ValidRegisterAddr[] = {0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xF6, 0xF7, 0xF8, 0xF9};
  byte _b, i, totregisters = sizeof(ValidRegisterAddr);
  for (i = 0; i < totregisters; i++) {
    dps.readmem(ValidRegisterAddr[i], 1, &_b);
  }
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