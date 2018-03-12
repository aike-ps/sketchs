int motorPin1 = 11;	// Blue   - 28BYJ48 pin 1
int motorPin2 = 10;	// Pink   - 28BYJ48 pin 2
int motorPin3 = 9;	// Yellow - 28BYJ48 pin 3
int motorPin4 = 8;	// Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)

int motorSpeed = 2;     //variable to set stepper speed

#include <Adafruit_NeoPixel.h>
#define PIN 6 // номер порта к которому подключен модуль
#define count_led 8 // количество светодиодов 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(count_led, PIN, NEO_GRB + NEO_KHZ800); //first number change does distance between colors


int mode = 0;
int counter = 0;

void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.begin(9600);
  pixels.begin();
  pixels.show();

  
}

void colorSet()
{
	mode++;
	if(mode == 10){
		mode = 0;
	}
	int color1 = 0;
	int color2 = 0;
	int color3 = 0;
	switch(mode){
	
	case 0:
      color1 = 200;
	  color2 = 0;
	  color3 = 0;
	  break;
    
	case 1:
      color1 = 0;
	  color2 = 200;
	  color3 = 0;
	  break;
    
	case 2:
      color1 = 0;
	  color2 = 0;
	  color3 = 200;
	  break;
    
	case 3:
      color1 = 200;
	  color2 = 200;
	  color3 = 0;
	  break;
    
	case 4:
      color1 = 0;
	  color2 = 200;
	  color3 = 200;
	  break;
    
	  
	case 5:
      color1 = 200;
	  color2 = 0;
	  color3 = 200;
	  break;
    
	  
	case 6:
      color1 = 200;
	  color2 = 200;
	  color3 = 200;
	  break;
    
	  
	case 7:
      color1 = 150;
	  color2 = 250;
	  color3 = 50;
	  break;
    
	  
	case 8:
      color1 = 100;
	  color2 = 100;
	  color3 = 200;
	  break;
    
	  
	case 9:
      color1 = 50;
	  color2 = 50;
	  color3 = 50;
	  break;
    	  
    default:
      color1 = 0;
	  color2 = 0;
	  color3 = 0;
	  break;
	
	}

	for(int i = 1; i < 9; i++){
		pixels.setPixelColor(i, pixels.Color(color1,color2,color3)); 
	}
	
	pixels.show();
	 
}


void loop(){
   //for(int i=0;i)
   counter++;
   if(counter == 1000){
       colorSet();
	   counter = 0;
   }
   counterclockwise();
  
}

void clockwise(){
	
  // 1
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);
  // 2
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay (motorSpeed);
  // 3
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);
  // 4
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);
  // 5
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);
  // 6
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, HIGH);
  delay (motorSpeed);
  // 7
  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(motorSpeed);
  // 8
  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(motorSpeed);
}
void counterclockwise (){
  // 1
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);
  // 2
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay (motorSpeed);
  // 3
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);
  // 4
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);
  // 5
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);
  // 6
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, HIGH);
  delay (motorSpeed);
  // 7
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(motorSpeed);
  // 8
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(motorSpeed);
}