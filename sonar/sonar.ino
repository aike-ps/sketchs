
// sonar central
int trigPin = 5;
int echoPin = 6;

void setup(){
	 
	pinMode(echoPin, INPUT);
	pinMode(trigPin, OUTPUT);
	digitalWrite(echoPin, LOW);
	Serial.begin(9600);
	 
}

void loop()
{
	checkDistance();
	delay(1000);
}



int checkDistance()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  int duration = pulseIn(echoPin, HIGH); 
  int cm = duration / 58;
  if(cm < 0){
	delay(2);
	return checkDistance();
  }
  Serial.print(cm);
  Serial.println(" front");
  return cm;
}
