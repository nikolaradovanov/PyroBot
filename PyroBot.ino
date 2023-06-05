#include "Arduino.h"
#include <SoftwareSerial.h>
#include <AFMotor.h>  
#define MAX_SPEED 255 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

const byte rxPin = 12;
const byte txPin = 13;
int trigPin = 11;    // TRIG pin
int echoPin = 10;    // ECHO pin

int pinRelay = 6;

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

int frontLights = A0;
int backLights = A1;

bool isMoving = false, goesForward = false, safeModeOn = false, isLightsOn = false;

SoftwareSerial mySerial(rxPin,txPin);

int speed = 0;

void setup() {

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  pinMode(pinRelay, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  analogWrite(enA, MAX_SPEED);
	analogWrite(enB, MAX_SPEED);
  pinMode(frontLights,OUTPUT);
  pinMode(backLights,OUTPUT);
  moveStop();
}

void loop() {

  if (safeModeOn) {
    if(measureDistance() < 20) {
      stepBack();
    } 
  }

  while (mySerial.available() > 0) {

    char data = (char) mySerial.read();

    switch(data) {

    case '1':
      moveForward();
    break;
    case '2':
      moveBackward();
    break;
    case '3':
      turnRight();
    break;
    case '4':
      turnLeft();
    break;
    case '0':
      moveStop();
    break;
    case 'S':
    toggleSafeMode();
    break;
    case 'F':
    burnBich();
    break;
    case 'L':
    toggleFrontLights();
    break;

    Serial.println(data);
    }

    //Serial.println("Data ");
    Serial.println(data);
    delay(50);
  }
}

void moveStop() {
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  isMoving = false;
  digitalWrite(backLights,LOW);
  } 
  
void moveForward() {

  isMoving = true;
  goesForward = true;
  analogWrite(enA, MAX_SPEED);
	analogWrite(enB, MAX_SPEED);
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

void moveBackward() {
  
  isMoving = true;
  goesForward = false;
  analogWrite(enA, 150);
	analogWrite(enB, 150);
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
  digitalWrite(backLights,HIGH);

}  

void turnRight() {
    analogWrite(enA, MAX_SPEED);
	analogWrite(enB, MAX_SPEED);
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  isMoving = true;         
} 
 
void turnLeft() {
    analogWrite(enA, MAX_SPEED);
	analogWrite(enB, MAX_SPEED);
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW); 
  isMoving = true; 
}  

float measureDistance () {

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  return 0.017 * duration_us;
}

void stepBack() {

  moveBackward();

  while (measureDistance() < 10) {
  delay(150);
  }

  moveStop();
}

void toggleSafeMode() {
  if (safeModeOn) {
    safeModeOn = false;
  } else {
    safeModeOn = true;
  }
}

void burnBich() {
  if(!isMoving) {
  digitalWrite(pinRelay,HIGH);
  delay(2000);
  digitalWrite(pinRelay,LOW);
  }
}

void toggleFrontLights() {

  if(isLightsOn) {
    digitalWrite(frontLights, LOW);
    isLightsOn = false;
  } else {
    digitalWrite(frontLights, HIGH);
    isLightsOn = true;
  }
}

