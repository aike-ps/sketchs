#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);  // Устанавливаем дисплей

// первый двигатель
int enA = 6; // on - off
int in1 = 3;
int in2 = 2;

// второй двигатель
int enB = 7; // on - off
int in3 = 5;
int in4 = 4;

// distance center
int cm = 0;

// distance center up
int up_cm = 0;

// distance left
int left_cm = 0;

//distance right
int right_cm = 0;

//distance back
int back_cm = 0;

// sonar left
int trigPinLeft = 30;
int echoPinLeft = 31;

// sonar centr
int trigPin = 28;
int echoPin = 29;

// sonar centr up
int trigPinUp = 26;
int echoPinUp = 27;

//sonar back
int trigPinBack = 24;
int echoPinBack = 25;

//sonar right
int trigPinRight = 22;
int echoPinRight = 23;

//0 - стоп
//1 - вперед
//2 - назад 
//3 - поворот влево
//4 - поворот вправо
int mode = 0;

// максимальная скорость
int speed = 0;

void setup(){

	lcd.init();                     
	lcd.backlight();// Включаем подсветку дисплея
	lcd.setCursor(0, 0);
	lcd.print("up  center  back");
	lcd.setCursor(0, 2);
	lcd.print("left   right");
	
	pinMode( A1, OUTPUT );
    analogWrite(A1, 0);
	beep();
	
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

	pinMode(echoPin, INPUT);
	pinMode(trigPin, OUTPUT);
	digitalWrite(echoPin, LOW);
	
	pinMode(echoPinLeft, INPUT);
	pinMode(trigPinLeft, OUTPUT);
	digitalWrite(echoPinLeft, LOW);
	
	pinMode(echoPinRight, INPUT);
	pinMode(trigPinRight, OUTPUT);
	digitalWrite(echoPinRight, LOW);
	
	pinMode(echoPinUp, INPUT);
	pinMode(trigPinUp, OUTPUT);
	digitalWrite(echoPinUp, LOW);
	
	pinMode(echoPinBack, INPUT);
	pinMode(trigPinBack, OUTPUT);
	digitalWrite(echoPinBack, LOW);
	
	Serial.begin(9600);
	speed = 180;
	beep();
}

void loop()
{
	checkDistanceCenter();
	checkDistanceLeft();
	checkDistanceRight(); 
	checkDistanceUp();
	checkDistanceBack();

	lcd.setCursor(0, 1);
	lcd.print(up_cm);
	lcd.print("   ");
	lcd.print(cm);
	lcd.print("      ");
	lcd.print(back_cm);
	lcd.print(" ");
	
	lcd.setCursor(0, 3);
	lcd.print(left_cm);
	lcd.print("      ");
    lcd.print(right_cm);
	lcd.print("  ");
	//move();
}

void move()
{
	if(checkDistanceCenter() < 25 or checkDistanceLeft() < 15 or checkDistanceRight() < 15){
		
		if(checkDistanceCenter() < 10 or checkDistanceLeft() < 5 or checkDistanceRight() < 5)
		{	stop();
			backward();
			delay(1000);
		}
		
		stop();
		int j = 0;
		for(int d = 0; d < 15; d = j){
			
			j = checkDistanceCenter;
			
			if(checkDistanceLeft() < checkDistanceRight()){
				j = checkDistanceRight();
			}
			else{
				j = checkDistanceLeft;
			}
			
			if(j < 20){
				beep();
				backward();
				delay(1000);
			}
		}
		
		if(checkDistanceLeft() > checkDistanceRight()){
			beep();
			for(int i = 0; i < 25; i = checkDistanceCenter()){
				turnLeft(300);
				 
			}
			beep();
		}
		else{
			beep();
			beep();
			for(int i = 0; i < 25; i = checkDistanceCenter()){
				turnRight(300);
				 
			}
			beep();
			
		}
	}
	forward();
}

int checkDistanceCenter()
{
  cm = checkDistance(trigPin, echoPin);
  return cm;
}

int checkDistanceUp()
{
  up_cm = checkDistance(trigPinUp, echoPinUp );
  return up_cm;
}

int checkDistanceBack()
{
  back_cm = checkDistance(trigPinBack, echoPinBack );
  return back_cm;
}

int checkDistanceLeft()
{
  left_cm = checkDistance(trigPinLeft, echoPinLeft );
  return left_cm;
}

int checkDistanceRight()
{
  right_cm = checkDistance(trigPinRight, echoPinRight );
  return right_cm;
}

int checkDistance(int trig, int echo )
{
  int i = 0;
  int roundDuration = 0;
  for(i; i < 50; i++){
     roundDuration += checkMainDistance(trig, echo );
  }
  roundDuration = roundDuration / 50;
  
  return roundDuration;
}

int checkMainDistance(int trig, int echo )
{
  digitalWrite(trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  int duration = pulseIn(echo, HIGH); 
  int main_cm = duration / 58;
  if(main_cm  < 0){
	delay(1);
	return checkMainDistance(trig, echo );
  }
  
  return main_cm;
}



void stop()
{
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
	analogWrite(enA, 0);
    analogWrite(enB, 0);
	mode = 0;
}

void forward()
{
	if(mode == 1){
		return;
	}
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	fixedAcceleratrion(speed);
	mode = 1;
}

void backward()
{
	if(mode == 2){
		return;
	}
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	fixedAcceleratrion(speed);
	mode = 2;
}

void turnRight(int max )
{
	 
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	fixedAcceleratrion(speed);
	delay(max);
	 
}

void turnLeft(int max )
{
	 
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	fixedAcceleratrion(speed);
	delay(max);
	 
}

void turnLeftFast(int max )
{
	analogWrite(enA, 0);
	analogWrite(enB, speed);
	delay(max);
	analogWrite(enA, speed);
}

void turnRightFast(int max )
{
	analogWrite(enB, 0);
	analogWrite(enA, speed);
	delay(max);
	analogWrite(enB, speed);
}

void acceleration(int max )
{
	if(max < 10 or max > speed){
		max = speed;
	}

	for (int i = 0; i < max; i += 5 )
	{
		analogWrite(enA, i);
		analogWrite(enB, i);
		delay(100);
	}
}

void fixedAcceleratrion(int max )
{
	if(max < 0 or max > speed){
		max = speed;
	}
	analogWrite(enA, max);
    analogWrite(enB, max);
}

void beep()
{
	analogWrite(A1, 200);
	delay(150);
	analogWrite(A1, 0);
	delay(150);
}
