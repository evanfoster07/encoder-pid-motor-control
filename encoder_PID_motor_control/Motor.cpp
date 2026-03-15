#include "Motor.h"
#include <Arduino.h>

// Motor.cpp
// DC motor control using PWM and encoder feedback.
// Handles direction control, speed measurement from encoder interrupts,
// and applys control signals from the PID controller.

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
  avgSpeedRPS = 0;
  ticks = 0; 
  targetSpeedRPS = 0;
  setDirection = -1;

  pinMode(pwmPin, OUTPUT);
  pinMode(encoderAPin, INPUT);
  pinMode(encoderBPin, INPUT);
  pinMode(dirAPin, OUTPUT);
  pinMode(dirBPin, OUTPUT);
  digitalWrite(dirAPin, LOW);  //default clockwise
  digitalWrite(dirBPin, HIGH);
}

void Motor::setSpeedPWM(int pwm) {
  speedPWM = pwm;
  analogWrite(pwmPin, speedPWM);
}

void Motor::setTargetSpeedRPS(float rps) {
  targetSpeedRPS = rps;
}

void Motor::setDir(int dir) {
  if (dir != 1 && dir != -1) return;

  setDirection = dir;
  int dirA;
  int dirB;
  int currPWM = speedPWM; //Store speed and set 0 to safely change direction
  setSpeedPWM(0);
  delay(3);  //Wait 3ms to avoid large reverse current spike
  
  if (setDirection == -1) { //LOW/HIGH configured for specific pins % motor driver connections
    dirA = LOW;
    dirB = HIGH;
  } else if (setDirection == 1){
    dirA = HIGH;
    dirB = LOW;
  }
  digitalWrite(dirAPin, dirA);
  digitalWrite(dirBPin, dirB);

  setSpeedPWM(currPWM);
}

void Motor::updateSpeedDir() {
  unsigned long now = millis();

  if (now - lastSampleTime >= 50) {  //Sample after minimum 50ms using actual elapsed time
    noInterrupts();
    long currentTicks = ticks;
    interrupts();
    long deltaTicks = currentTicks - lastSampleTicks;
    float deltaTimeS = (now - lastSampleTime) / 1000.0;

    lastSampleTicks = currentTicks;
    lastSampleTime = now;

    float rawSpeed = (deltaTicks / float(TICKS_PER_REV)) / deltaTimeS / GEAR_RATIO; //calculate rps of current motor shaft speed
    float alpha = 0.1;
    speedRPS = alpha * speedRPS + (1 - alpha) * rawSpeed; //Only update speed by 90% of measured speed to avoid jumping
    
    if (abs(speedRPS) < 0.001) {
      measuredDirection = 0;  //dir = 0 if still
    } else if (speedRPS < 0) {   //negative speed clockwise -1 (B leads A), positive speed counterclockwise 1 (A leads B)
      measuredDirection = -1;
    } else {
      measuredDirection = 1;
    }
  }
}

void Motor::encoderISR() {
  bool A = digitalRead(encoderAPin);
  bool B = digitalRead(encoderBPin);
  
  if (A == B) {
    ticks--;  //B leads A
  } else {
    ticks++;  //A leads B
  }
}

long Motor::getTicks() {
  return ticks;
}

float Motor::getTargetSpeedRPS() {
  return targetSpeedRPS;
}

float Motor::getSpeedRPS() {
  return speedRPS;
}

int Motor::getSpeedPWM() {
  return speedPWM;
}

int Motor::getMeasuredDir() {
  return measuredDirection;
}

int Motor::getSetDirection() {
  return setDirection;
}

void Motor::applyControl(float control) {
  const int minPWM = 90;   //experimentally determined

  int newDir;
  if (control < 0) {
    newDir = -1;  //set direction based on control output
  } else {
    newDir = 1;   
  }

  int pwm = (int)abs(control);

  if (pwm > 255) {  //Clamp pwm to max or minimum effective if correction is needed
    pwm = 255;
  }

  if (pwm < minPWM) {
    pwm = 0;
  }

  if (newDir != setDirection) {  //Change direction if correction is required
    setDir(newDir);
  }

  setSpeedPWM(pwm);
}







