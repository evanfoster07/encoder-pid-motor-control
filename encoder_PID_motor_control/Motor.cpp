#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int pwm, int encA, int encB) {
  pwmPin = pwm;
  encoderAPin = encA;
  encoderBPin = encB;
  lastSampleTicks = 0;
  lastSampleTime = millis();
  speedRPS = 0;
  ticks = 0; 
  dir = 0;

  pinMode(pwmPin, OUTPUT);
  pinMode(encoderAPin, INPUT);
  pinMode(encoderBPin, INPUT);
}

void Motor::setSpeed(int pwm) {
  analogWrite(pwmPin, pwm);
}

float Motor::getSpeedRPS() {
  return speedRPS;
}

int Motor::getDir() {
  return dir;
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
      dir = 0;  //dir = 0 if still
    } else if (speed < 0) {   //negative speed clockwise -1 (B leads A), positive speed counterclockwise 1 (A leads B)
      dir = -1;
    } else {
      dir = 1;
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









