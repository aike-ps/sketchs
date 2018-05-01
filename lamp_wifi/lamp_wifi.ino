#include <Adafruit_NeoPixel.h>
#define PIN 14 // номер порта к которому подключен модуль
#define count_led 8 // количество светодиодов 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800); //first number change does distance between colors

int mode = 6;
int counter = 8;
int key_1 = 4;
int key_2 = 5;
int animation_mode = 0;
int animation = 0;

void setup() {

  Serial.begin(9600);
  pixels.begin();
  pixels.show();
  //colorSet();
  pinMode(key_1, INPUT);
  pinMode(key_2, INPUT);
  colorSet();

}

void loop() {

  int a = digitalRead(key_1);
  int b = digitalRead(key_2);

  if (a == 0) {
    mode++;
    if (mode > 7) {
      mode = 0;
    }
    colorSet();
    delay(250);
  }

  if (b == 0) {
    if (animation_mode == 0) {
      counter++;
      if (counter > 8) {
        counter = 0;
      }
      colorSet();
      delay(250);
    }
    else {
      animation++;
      if (animation > 10) {
        animation = 0;
      }
      animation_action();
      delay(250);
    }
  }

  if (animation_mode == 1) {
    animation_action();
  }

}

void animation_action() {
  counter = 8;
  switch (animation) {
    case 0:
      animation_0();
      break;

    case 1:
      animation_1();
      break;

    case 2:
      animation_2();
      break;

    case 3:
      animation_3();
      break;

    case 4:
      animation_4();
      break;

    case 5:
      animation_5();
      break;

    case 6:
      animation_6();
      break;

    case 7:
      animation_7();
      break;

    case 8:
      animation_8();
      break;

    case 9:
      animation_9();
      break;
  }
}

void animation_0() {
  setLight(254, 0, 0, 150);
  setLightRev(0, 254, 0, 150);
  setLight(0, 254, 0, 150);
  setLightRev(254, 0, 0, 150);
  setLight(0, 0, 254, 150);
  setLightRev(0, 0, 254, 150);

}

void animation_1() {

}

void animation_2() {

}

void animation_3() {

}

void animation_4() {

}

void animation_5() {

}

void animation_6() {

}

void animation_7() {

}

void animation_8() {

}

void animation_9() {

}

void colorSet()
{

  int color1 = 0;
  int color2 = 0;
  int color3 = 0;
  
  switch (mode) {

    case 0:
      color1 = 254;
      color2 = 0;
      color3 = 0;
      break;

    case 1:
      color1 = 0;
      color2 = 254;
      color3 = 0;
      break;

    case 2:
      color1 = 0;
      color2 = 0;
      color3 = 254;
      break;

    case 3:
      color1 = 254;
      color2 = 254;
      color3 = 0;
      break;

    case 4:
      color1 = 0;
      color2 = 254;
      color3 = 254;
      break;


    case 5:
      color1 = 254;
      color2 = 0;
      color3 = 254;
      break;

    case 6:
      color1 = 254;
      color2 = 254;
      color3 = 254;
      break;

    case 7:
      if (animation_mode == 1) {
        animation_mode = 0;
      } else {
        animation_mode = 1;
      }
      break;

    case 8:
      color1 = 0;
      color2 = 0;
      color3 = 0;
      break;

    default:
      color1 = 0;
      color2 = 0;
      color3 = 0;
      break;

  }

  setLight(color1, color2, color3, 50);

}

void setLight(int color1, int color2, int color3, int d_time) {
  for (int i = 0; i < counter; i++) {
    pixels.setPixelColor(i, pixels.Color(color1, color2, color3));
    delay(d_time);
    pixels.show();
  }
  if (counter == 0) {
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      delay(d_time);
      pixels.show();
    }
  }
}

void setLightRev(int color1, int color2, int color3, int d_time) {
  for (int i = 7; i >= 0; i--) {
    pixels.setPixelColor(i, pixels.Color(color1, color2, color3));
    delay(d_time);
    pixels.show();
  }
}