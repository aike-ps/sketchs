#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#include "U8glib.h"
#include <DS3232RTC.h>
#include <TimeLib.h>
#include "DHT.h"
#define DHTPIN 6
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
U8GLIB_ST7920_128X64_1X u8g(13, 12, 11);
Adafruit_BME280 bme;
DS3232RTC  rtc(true);
long now_hour = 0;
long Temperature = 0, Pressure = 0;
int pwm = 3;

int temp = 0;
int tempMin = 0;
int tempMax = 0;

int pres = 0;
int presMin = 0;
int presMax = 0;

int hydim = 0;
int hydimMin = 0;
int hydimMax = 0;

int outTemp = 0;
int outTempMin = 0;
int outTempMax = 0;

int outHydim = 0;
int outHydimMin = 0;
int outHydimMax = 0;

int counter = 0;
int light = 50;
int pressure_counter = 0;
int pressureArray[47];
int hydimArray[62];
int hydimOutArray[62];
int tempArray[62];
int tempOutArray[62];

long previousMillis = 0;
long interval = 1;
tmElements_t tm;
int sensor = A0;
int key = A1;
int mode = 1;
int menuTimeOut = 30000;

// 1 - normal
// 2 - set minute
// 3 - set hour
// 4 - set day
// 5 - set month
// 5 - set year

int backlight = 3;
int current = 128;

void setup()
{
  pinMode(backlight, OUTPUT);
  setSyncProvider(RTC.get);
  Serial.begin(9600);
  boot_screen();
  setup_bmp();
  dht.begin();
  previousMillis = tm.Second;
  //set_time();
  pinMode(key, INPUT);
  pinMode(sensor, INPUT);
  delay(1000);
  firstRun();
  getData();
  // first run set current value as min and max
  updateDataArrays();
  show_data();
}

void loop()
{
  RTC.read(tm);
  int keyVal = analogRead(key);
  if (keyVal > 50) {
    delay(10);
    keyVal = analogRead(key);
    if (keyVal > 50) {
      if (keyVal < 200) {
        if (mode < 7) {
          mode++;
          if (mode == 7) {
            mode = 1;
            show_data();
          }
        }
      }
    }
  }

  int currentMillis = tm.Second;

  if (mode > 1) {
    set_time();
    delay(10);
    show_data();
    return;
  }

  // 30 sec
  if (counter == 30) {
    getData();
    counter = 0;
    show_data();
    setLight();
  }

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

  if (pressure_counter == 600) {
    updateDataArrays();
    pressure_counter = 0;
    show_data();
  }
}

void updateDataArrays()
{
  updateMinMax();
  updatePressure();
  updateTemp();
  updateTempOut();
  updateHydim();
  updateHydimOut();
}

void updatePressure()
{
  if (presMin == presMax) {
    presMax = presMin + 7;
  }

  int tmp_val = map(pres, presMin, presMax, 1, 14);
  updatePressureArray(pressureArray, tmp_val);
}

void updateTemp()
{
  if (tempMin == tempMax) {
    tempMax = tempMin + 7;
  }

  int tmp_val = map(temp, tempMin, tempMax, 1, 14);
  updateDataArray(tempArray, tmp_val);
}

void updateTempOut()
{
  if (outTempMin == outTempMax) {
    outTempMax = outTempMin + 7;
  }

  int tmp_val = map(outTemp, outTempMin, outTempMax, 1, 14);
  updateDataArray(tempOutArray, tmp_val);
}

void updateHydim()
{
  if (hydimMin == hydimMax) {
    hydimMax = hydimMin + 7;
  }

  int tmp_val = map(hydim, hydimMin, hydimMax, 1, 14);
  updateDataArray(hydimArray, tmp_val);
}

void updateHydimOut()
{
  if (outHydimMin == outHydimMax) {
    outHydimMax = outHydimMin + 7;
  }

  int tmp_val = map(outHydim, outHydimMin, outHydimMax, 1, 14);
  updateDataArray(hydimOutArray, tmp_val);
}

void updatePressureArray(int arr[], int data)
{
  int i;

  for (i = 0; i < 47; i++ ) {

    if (i == 46) {
      arr[i] = data;
    } else {
      arr[i] = arr[i + 1];
    }

    if (arr[i] > 14) {
      arr[i] = 14;
    }

    if (arr[i] < 1) {
      arr[i] = 0;
    }
  }
}

void updateDataArray(int arr[], int data)
{
  int i;

  for (i = 0; i < 62; i++ ) {

    if (i == 61) {
      arr[i] = data;
    } else {
      arr[i] = arr[i + 1];
    }

    if (arr[i] > 14) {
      arr[i] = 14;
    }

    if (arr[i] < 1) {
      arr[i] = 0;
    }
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

  if (! bme.begin(0x76, &Wire)) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1) delay(10);
  }
  RTC.read(tm);
}

void show_data()
{
  int hour = tm.Hour;
  int minute = tm.Minute;
  int second = tm.Second;
  //  int year = tm.Year + 1970;
  //  int month = tm.Month;
  //  int day = tm.Day;

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
    //    u8g.print(":");
    //    if (second < 10) {
    //      u8g.print(0);
    //    }
    //    u8g.print(second);
    u8g.undoScale();
    // pressure
    int i ;
    for (i = 80; i < 127; i++ ) {
      u8g.drawLine(i, 20, i, 20 - pressureArray[i - 80]);
    }
    //
    u8g.setFont(u8g_font_6x12);
    u8g.setPrintPos(0, 31);
    u8g.print( pres );
    u8g.print("mm");
    u8g.setPrintPos(31, 31);
    //u8g.print(" ");
    u8g.print( temp );
    u8g.print( "|" );
    u8g.print( outTemp );
    u8g.print( "|" );
    u8g.print(int(dht.computeHeatIndex(outTemp, outHydim, false)));
    u8g.write(0xBA);
    u8g.print("C");
    u8g.setPrintPos(92, 31);
    //u8g.print(" ");
    u8g.print( hydim );
    u8g.print( "|" );
    u8g.print( outHydim );
    u8g.print("%");

    if (mode == 1) {
      int i;
      for (i = 0; i < 62; i++ ) {
        u8g.drawLine(i, 49, i, 49 - tempArray[i]);
        u8g.drawLine(i + 66, 49, i + 66, 49 - tempOutArray[i]);
        u8g.drawLine(i, 63, i, 63 - hydimArray[i]);
        u8g.drawLine(i + 66, 63, i + 66, 63 - hydimOutArray[i]);
      }
    }
    else {
      u8g.setPrintPos(0, 45);
      u8g.print("mode ");
      u8g.print(mode);
    }
  } while ( u8g.nextPage() );
  setLight();
}

void firstRun()
{
  temp = int(bme.readTemperature());
  tempMin = temp;
  tempMax = temp;

  pres = int(bme.readPressure() / 133.3);
  presMin = pres;
  presMax = pres;

  hydim = int(bme.readHumidity());
  hydimMin = hydim;
  hydimMax = hydim;

  outHydim = int(dht.readHumidity());
  outHydimMin = outHydim;
  outHydimMax = outHydim;

  outTemp  = int(dht.readTemperature());
  outTempMin = outTemp;
  outTempMax = outTemp;
}

void updateMinMax()
{
  if (temp < tempMin) {
    tempMin = temp;
  }

  if (temp > tempMax) {
    tempMax = temp;
  }

  if (pres < presMin) {
    presMin = pres;
  }

  if (pres > presMax) {
    presMax = pres;
  }

  if (hydim < hydimMin) {
    hydimMin = hydim;
  }

  if (hydim > hydimMax) {
    hydimMax = hydim;
  }

  if (outHydim < outHydimMin) {
    outHydimMin = outHydim;
  }

  if (outHydim > outHydimMax) {
    outHydimMax = outHydim;
  }

  if (outTemp < outTempMin) {
    outTempMin = outTemp;
  }

  if (outTemp > outTempMax) {
    outTempMax = outTemp;
  }
}


void getData()
{
  temp = int(bme.readTemperature());
  pres = int(bme.readPressure() / 133.3);
  hydim = int(bme.readHumidity());

  int tempVal = int(dht.readHumidity());

  if (tempVal < 100 and tempVal > -100) {
    outHydim = tempVal;
  }

  tempVal  = int(dht.readTemperature());

  if (tempVal < 100 and tempVal > -100) {
    outTemp = tempVal;
  }

  int hour = tm.Hour;
  int minute = tm.Minute;
  int second = tm.Second;
  int year = tm.Year + 1970;
  int month = tm.Month;
  int day = tm.Day;
  Serial.print(year);
  Serial.print("-");
  Serial.print(month);
  Serial.print("-");
  Serial.print(day);
  Serial.print(" ");

  if (hour < 10) {
    Serial.print(0);
  }

  Serial.print(hour);
  Serial.print(":");

  if (minute < 10) {
    Serial.print(0);
  }
  Serial.print(minute);
  Serial.print(":");

  if (second < 10) {
    Serial.print(0);
  }

  Serial.print(second);
  Serial.print(";");
  Serial.print(temp);
  Serial.print(";");
  Serial.print(outTemp);
  Serial.print(";");
  Serial.print(hydim);
  Serial.print(";");
  Serial.print(outHydim);
  Serial.print(";");
  Serial.print(pres);
  Serial.print("\n");
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
  int keyVal = analogRead(key);
  if (mode == 2) {
    if (keyVal > 200 && keyVal < 300) {
      if (tm.Minute < 60)
      {
        tm.Minute++;
      }
      else {
        tm.Minute = 0;
      }
    }
    if (keyVal > 300) {
      if (tm.Minute >= 0) {
        tm.Minute--;
      }
      else {
        tm.Minute = 59;
      }
    }
  }
  if (mode == 3) {
    if (keyVal > 200 && keyVal < 300) {
      if (tm.Hour < 24) {
        tm.Hour++;
      } else {
        tm.Hour = 0;
      }
    }
    if (keyVal > 300) {
      if (tm.Hour >= 0) {
        tm.Hour--;
      }
      else {
        tm.Hour = 24;
      }
    }
  }

  if (mode == 4) {
    if (keyVal > 200 && keyVal < 300) {
      tm.Day++;
    }
    if (keyVal > 300) {
      tm.Day--;
    }
  }
  if (mode == 5) {
    if (keyVal > 200 && keyVal < 300) {
      tm.Month++;
    }
    if (keyVal > 300) {
      tm.Month--;
    }
  }
  if (mode == 6) {
    if (keyVal > 200 && keyVal < 300) {
      tm.Year++;
    }
    if (keyVal > 300) {
      tm.Year--;
    }
  }

  rtc.write(tm);
}
