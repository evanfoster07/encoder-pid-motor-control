#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int pwm, int encA, int encB, int dirAP, int dirBP) {
  pwmPin = pwm;
  encoderAPin = encA;
  encoderBPin = encB;
  dirAPin = dirAP;
  dirBPin = dirBP;
  lastSampleTicks = 0;
  lastSampleTime = millis();
  speedPWM = 0;
  speedRPS = 0;
  ticks = 0; 

  pinMode(pwmPin, OUTPUT);
  pinMode(encoderAPin, INPUT);
  pinMode(encoderBPin, INPUT);
  pinMode(dirAPin, OUTPUT);
  pinMode(dirBPin, OUTPUT);
  digitalWrite(dirAPin, LOW);  //default clockwise
  digitalWrite(dirBPin, HIGH);
}

void Motor::setSpeed(int pwm) {
  speedPWM = pwm;
  analogWrite(pwmPin, speedPWM);
}

float Motor::getSpeedRPS() {
  return speedRPS;
}

void Motor::setDir(int dir) {
  int dirA;
  int dirB;
  int currPWM = speedPWM; //Store speed and set 0 to safely change direction
  setSpeed(0);
  delay(5);  //Wait 5ms to avoid large reverse current spike
  
  if(dir == -1) { //LOW/HIGH configured for specific pins % motor driver connections
    dirA = LOW;
    dirB = HIGH;
  } else if (dir == 1){
    dirA = HIGH;
    dirB = LOW;
  }
  digitalWrite(dirAPin, dirA);
  digitalWrite(dirBPin, dirB);

  setSpeed(currPWM);
}

int Motor::getDir() {
  return direction;
}

void Motor::updateSpeedDir() {
  unsigned long now = millis();

  if(now - lastSampleTime >= 75) {   //Sample every 75ms
    long currentTicks = ticks;
    long deltaTicks = currentTicks - lastSampleTicks;
    float deltaTimeS = (now - lastSampleTime) / 1000.0;

    lastSampleTicks = currentTicks;
    lastSampleTime = now;

    float speed = (deltaTicks / float(ticksPerRev)) / deltaTimeS / gearRatio;
    
    if(abs(speed) < 0.001) {
      direction = 0;  //dir = 0 if still
    } else if (speed < 0) {   //negative speed clockwise -1 (B leads A), positive speed counterclockwise 1 (A leads B)
      direction = -1;
    } else {
      direction = 1;
    }
    
    speedRPS = abs(speed);
  }
}

void Motor::encoderISRA() {
  if(digitalRead(encoderBPin) == HIGH) {
    ticks--;  //B leads A
  } else {
    ticks++;  //A leads B
  }
}

long Motor::getTicks() {
  return ticks;
}









