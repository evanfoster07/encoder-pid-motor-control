#include "Motor.h"
#include "PIDController.h"

const int motorPWM = 5;
const int encoderA = 2;
const int encoderB = 3;
const int dirAPin = 8;
const int dirBPin = 9;

Motor Motor1(motorPWM, encoderA, encoderB, dirAPin, dirBPin);
PID_Control pid(50.0, 0.0, 0.0);

//Wrapper function for encoder A interrupts
void encoderAWrapper() {
  Motor1.encoderISRA();
}

unsigned long lastPrint;
unsigned long lastControl;
unsigned long now;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoderA), encoderAWrapper, RISING);

  Motor1.setTargetSpeedRPS(-0.40);
  lastPrint = millis();
  lastControl = millis();
}


void loop() {
  now = millis();
  if(now - lastControl > 50) {
    lastControl = now;
    Motor1.updateSpeedDir();
    float target = Motor1.getTargetSpeedRPS();
    float measured = Motor1.getSpeedRPS();
    float error = pid.compute(target, measured, 0);
    Motor1.applyControl(error);
  }

  if(now - lastPrint > 1000) {  //print speedRPS, PWM and the set direction for testing
    lastPrint = now;
    Serial.println(Motor1.getSpeedRPS());
    Serial.println(Motor1.getSpeedPWM());
    Serial.println(Motor1.getSetDirection());
  }
}

