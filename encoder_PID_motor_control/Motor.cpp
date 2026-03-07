#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int pwm, int encA, int encB) {
  pwmPin = pwm;
  encoderAPin = encA;
  encoderBPin = encB;
  lastSampleTicks = 0;
  lastSampleTime = millis();
  speedRPS = 0;
  ticksA = 0;
  ticksB = 0;

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

void Motor::updateSpeed() {
  unsigned long now = millis();

  if(now - lastSampleTime >= 500) {   //Sample every 500ms
    long currentTicks = ticksA;
    long deltaTicks = ticksA - lastSampleTicks;
    float deltaTimeS = (now - lastSampleTime) / 1000.0;

    lastSampleTicks = currentTicks;
    lastSampleTime = now;

    float speed = (deltaTicks / float(ticksPerRev)) / deltaTimeS;
    speedRPS = speed;
  }
}

void Motor::encoderISRA() {
  ticksA++;
}

void Motor::encoderISRB() {
  ticksB++;
}

long Motor::getTicksA() {
  return ticksA;
}

long Motor::getTicksB() {
  return ticksB;
}

int Motor::getDir() {
  return 1;   //Will implement code to determine motor direction
}








