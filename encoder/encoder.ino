#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // Устанавливаем дисплей
#include <TM74HC595Display.h> // библиотека мелкого екрана

///////////////////////////

// ПИНЫ ЭНКОДЕРА
#define CLK 2
#define DT 3
#define SW 4
//  
#define NORM_STEP 10  // шаг изменения переменной norm_counter при вращении
#define HOLD_STEP 1	   // шаг изменения переменной hold_counter при нажатии, удерживании и вращении

int norm_counter, hold_counter;
boolean DT_now, DT_last, SW_state, hold_flag, butt_flag, turn_flag;
unsigned long debounce_timer;

/////////////////////////


////////////////
// пины мелкого екрана

int SCLK = 7;
int RCLK = 6;
int DIO = 5;

TM74HC595Display disp(SCLK, RCLK, DIO);
unsigned char LED_0F[29];

   
/////////////////


void setup()
{
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("encoder test");
  Serial.begin (9600);
  
  // энкодер
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  DT_last = digitalRead(CLK);  // читаем начальное положение CLK (энкодер)
  //
  
  // екранчик
  pinMode(RCLK, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(DIO, OUTPUT); 

  // заполняем массив
  
  LED_0F[0] = 0xC0; //0
  LED_0F[1] = 0xF9; //1
  LED_0F[2] = 0xA4; //2
  LED_0F[3] = 0xB0; //3
  LED_0F[4] = 0x99; //4
  LED_0F[5] = 0x92; //5
  LED_0F[6] = 0x82; //6
  LED_0F[7] = 0xF8; //7
  LED_0F[8] = 0x80; //8
  LED_0F[9] = 0x90; //9
  LED_0F[10] = 0x88; //A
  LED_0F[11] = 0x83; //b
  LED_0F[12] = 0xC6; //C
  LED_0F[13] = 0xA1; //d
  LED_0F[14] = 0x86; //E
  LED_0F[15] = 0x8E; //F
  LED_0F[16] = 0xC2; //G
  LED_0F[17] = 0x89; //H
  LED_0F[18] = 0xF9; //I
  LED_0F[19] = 0xF1; //J
  LED_0F[20] = 0xC3; //L
  LED_0F[21] = 0xA9; //n
  LED_0F[22] = 0xC0; //O
  LED_0F[23] = 0x8C; //P
  LED_0F[24] = 0x98; //q
  LED_0F[25] = 0x92; //S
  LED_0F[26] = 0xC1; //U
  LED_0F[27] = 0x91; //Y
  LED_0F[28] = 0xFE; //hight -
   
  
}

void loop()
{
  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 1);
  
  encoderTick();                      // ФУНКЦИЯ ОТРАБОТКИ ЭНКОДЕРА! СМОТРИ, ВОТ ОНА!
  rm_counter >= 0){
     disp.digit4(norm_counter, 50);
  }else{
     disp.digit4(norm_counter * -1, 50);
  }
  
   
   
}
 
void encoderClick() {
  Serial.println("Click");
}
void encoderPress() {
  Serial.println("Press");
}
void encoderHold() {
  Serial.println("Hold");
}
void encoderTurn() {
  Serial.print("Turn: ");
  Serial.println(norm_counter);
  lcd.setCursor(0, 1);
  lcd.print(norm_counter);
  lcd.print("       ");

}
void encoderHoldTurn() {
  Serial.print("HoldTurn: ");
  Serial.println(hold_counter);
  lcd.setCursor(0, 1);
  lcd.print(hold_counter);
  lcd.print("       ");
}

//---------------------------------------------------------------------------
// -------------------------ОТРАБОТКА ЭНКОДЕРА-------------------------------
void encoderTick() {
  DT_now = digitalRead(CLK);          // читаем текущее положение CLK
  SW_state = !digitalRead(SW);        // читаем положение кнопки SW
  // отработка нажатия кнопки энкодера
  if (SW_state && !butt_flag && millis() - debounce_timer > 200) {
    hold_flag = 0;
    butt_flag = 1;
    turn_flag = 0;
    debounce_timer = millis();
    encoderClick();
  }
  if (!SW_state && butt_flag && millis() - debounce_timer > 200 && millis() - debounce_timer < 500) { 
	butt_flag = 0; 
	if (!turn_flag && !hold_flag) { 
		// если кнопка отпущена и ручка не поворачивалась 
		turn_flag = 0; encoderPress(); 
	} 
	debounce_timer = millis(); 
  } 
  if (SW_state && butt_flag && millis() - debounce_timer > 800 && !hold_flag) {
    hold_flag = 1;
    if (!turn_flag) {  // если кнопка отпущена и ручка не поворачивалась
      turn_flag = 0;
      encoderHold();
    }
  }
  if (!SW_state && butt_flag && hold_flag) {
    butt_flag = 0;
    debounce_timer = millis();
  }
  if (DT_now != DT_last) {            // если предыдущее и текущее положение CLK разные, значит был поворот
    if (digitalRead(DT) != DT_now) {  // если состояние DT отличается от CLK, значит крутим по часовой стрелке
      if (SW_state) {           // если кнопка энкодера нажата
        hold_counter += HOLD_STEP;
        encoderHoldTurn();
      } else {                  // если кнопка энкодера не нажата
        norm_counter += NORM_STEP;
        encoderTurn();
      }
    } else {                          // если совпадают, значит против часовой
      if (SW_state) {           // если кнопка энкодера нажата
        hold_counter -= HOLD_STEP;
        encoderHoldTurn();
      } else {                  // если кнопка энкодера не нажата
        norm_counter -= NORM_STEP;
        encoderTurn();
      }
    }
    turn_flag = 1;                    // флаг что был поворот ручки энкодера
  }
  DT_last = DT_now;                   // обновить значение для энкодера
}