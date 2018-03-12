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

// distance left
int left_cm = 0;

//distance right
int right_cm = 0;


// sonar left
int trigPinLeft = 13;
int echoPinLeft = 12;

// sonar central
int trigPin = 11;
int echoPin = 10;

//sonar right
int trigPinRight = 9;
int echoPinRight = 8;

//0 - стоп
//1 - вперед
//2 - назад 
//3 - поворот влево
//4 - поворот вправо
int mode = 0;

// максимальная скорость
int speed = 0;

void setup(){
	
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
	Serial.begin(9600);
	speed = 180;
	beep();
}

void loop()
{
	move();
}

void move()
{
	if(checkDistance() < 25 or checkDistanceLeft() < 15 or checkDistanceRight() < 15){
		
		if(checkDistance() < 10 or checkDistanceLeft() < 5 or checkDistanceRight() < 5)
		{	stop();
			backward();
			delay(1000);
		}
		
		stop();
		int j = 0;
		for(int d = 0; d < 15; d = j){
			
			j = checkDistance;
			
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
			for(int i = 0; i < 25; i = checkDistance()){
				turnLeft(300);
				 
			}
			beep();
		}
		else{
			beep();
			beep();
			for(int i = 0; i < 25; i = checkDistance()){
				turnRight(300);
				 
			}
			beep();
			
		}
	}
	forward();
}


int checkDistance()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  int duration = pulseIn(echoPin, HIGH); 
  cm = duration / 58;
  if(cm < 0){
	delay(2);
	return checkDistance();
  }
  Serial.print(cm);
  Serial.println(" front");
  return cm;
}

int checkDistanceLeft()
{
  digitalWrite(trigPinLeft, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPinLeft, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPinLeft, LOW); 
  int duration = pulseIn(echoPinLeft, HIGH); 
  left_cm = duration / 58;
  if(left_cm < 0){
	delay(2);
	return checkDistanceLeft();
  }
  Serial.print(left_cm);
  Serial.println(" left");
  return left_cm;
}

int checkDistanceRight()
{
  digitalWrite(trigPinRight, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPinRight, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPinRight, LOW); 
  int duration = pulseIn(echoPinRight, HIGH); 
  right_cm = duration / 58;
  if(right_cm < 0){
	delay(2);
	return checkDistanceRight();
  }
  Serial.print(right_cm);
  Serial.println(" right");
  return right_cm;
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
