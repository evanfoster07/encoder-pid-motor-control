#include "Motor.h"
#include "PIDController.h"

const int motorPWM = 5;
const int encoderA = 2;
const int encoderB = 3;
const int dirAPin = 8;
const int dirBPin = 9;

//Motor and PID_Control objects
Motor Motor1(motorPWM, encoderA, encoderB, dirAPin, dirBPin);
PID_Control pid(50.0, 50.0, 0.0);

void encoderAWrapper() {  //Wrapper function for encoder A interrupts
  Motor1.encoderISRA();
}

//global variables for data printing intervals and motor updates
unsigned long lastPrint;
unsigned long lastControl;
unsigned long now;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoderA), encoderAWrapper, RISING);

  Motor1.setTargetSpeedRPS(0.75);
  lastPrint = millis();
  lastControl = millis();
}


void loop() {
  now = millis();
  if(now - lastControl > 50) {  //Run control loop
    lastControl = now;
    Motor1.updateSpeedDir();
    float target = Motor1.getTargetSpeedRPS();
    float measured = Motor1.getSpeedRPS();
    float error = pid.compute(target, measured, 0.050);

    float feedforward = 0;

    if(target > 0 && abs(target) > 0.50) {  //Only set feedforward if speed > 0.50rps (avg speed at pwm 90)
      feedforward = 90;
    } else if (abs(target) > 0.50){
      feedforward = -90;
    }

    float control = error + feedforward;
    
    Motor1.applyControl(control);
  }

  if(now - lastPrint > 1000) {  //print speedRPS, PWM and the set direction for testing
    lastPrint = now;
    Serial.print("Speed (rps): ");
    Serial.println(Motor1.getSpeedRPS());
    Serial.print("Speed (pwm): ");
    Serial.println(Motor1.getSpeedPWM());
    Serial.print("Direction (-1/1): ");
    Serial.println(Motor1.getSetDirection());
  }
}

