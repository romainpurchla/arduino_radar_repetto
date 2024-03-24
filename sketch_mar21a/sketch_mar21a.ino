#include <Servo.h>

Servo myServo;

int incomingByte = 0;

// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;

int const potPin = A0;
int potVal;
int angle = 0;
int delayTime;
int delayLimit = 150;
int distanceLimit = 100;
int distanceAverage = 0;
long duration;

int angleMin = 15;
int angleMax = 165;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  myServo.attach(9);
  Serial.begin(9600);
}

void calculeDelay() {
  potVal = analogRead(potPin);
  // Serial.print("potVal: ");
  // Serial.print(potVal);
  // Serial.print(".\n");
  delayTime = map(potVal, 0, 1023, 30, delayLimit);
}

void loop() {
 int i ;
 calculeDelay();
  for (angle; angle<angleMax; angle++) {
    myServo.write(angle);
    pauseWhenPotIsInLimit();
    delay(delayTime);
    averageDistance();
    printDebug();
    executeBuzzer();
  }
  for(angle; angle>angleMin; angle--) {
    myServo.write(angle);
    pauseWhenPotIsInLimit();
    delay(delayTime);
    averageDistance();
    printDebug();    
    executeBuzzer();
  }
 }

 void executeBuzzer() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    tone(A1, 20000, 20);
    delay(10);
  }
 }

void averageDistance() {
  int nbAverage = 10;
  int distances[nbAverage];
  int i = 0;
  int s = 0;
  do {
    int distance = calculateDistance();
    if (distance < distanceLimit) {
      distances[i] = distance;
      s += distance;
      i++;      
    }
    delay(5);
  } while(i < nbAverage);

  distanceAverage = s/nbAverage;
} 
 
void printDebug() {
  Serial.print(angle);
  Serial.print(",");
  Serial.print(distanceAverage);
  Serial.print(",");
  Serial.print(delayTime);
  Serial.print(".\n");    
}

void pauseWhenPotIsInLimit() {
  calculeDelay();
  while (delayTime == delayLimit) {
    calculeDelay();
    delay(5);
  }
}

 // Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {    
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance= duration*0.034/2;
  return distance;
}

