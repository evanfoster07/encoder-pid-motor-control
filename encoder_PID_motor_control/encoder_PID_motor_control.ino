#include "Motor.h"
#include "PIDController.h"

const int motorPWM = 5;
const int encoderA = 2;
const int encoderB = 3;
const int dirAPin = 8;
const int dirBPin = 9;

//Motor and PID_Control objects
Motor Motor1(motorPWM, encoderA, encoderB, dirAPin, dirBPin);
PID_Control pid(50.0, 50.0, 5.0);

void encoderAWrapper() {  //Wrapper function for encoder A interrupts
  Motor1.encoderISRA();
}

//global variables for data printing intervals
unsigned long lastPrint;
unsigned long lastControl;
unsigned long now;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoderA), encoderAWrapper, RISING);

  Motor1.setTargetSpeedRPS(0.650);
  lastPrint = millis();
  lastControl = millis();
}


void loop() {
  controlMotor(Motor1);

  if(now - lastPrint > 2000) {  //print metrics for testing
    lastPrint = now;
    Serial.print("Target RPS: ");
    Serial.println(Motor1.getTargetSpeedRPS());
    Serial.print("Measured RPS: ");
    Serial.println(Motor1.getSpeedRPS());
    Serial.print("Error: ");
    Serial.println(pid.getPrevError());
    Serial.print("Recent derivative: ");
    Serial.println(pid.getPrevDerivative());
    Serial.print("Current integral: ");
    Serial.println(pid.getIntegral());
    Serial.println();
  }

  if(Serial.available()) {  //Change target speed (rps) based on input
    char buffer[20];
    Serial.readBytesUntil('\n', buffer, 20);  
    float newSpeed = atof(buffer);
    Motor1.setTargetSpeedRPS(newSpeed);
  }
}


void controlMotor(Motor motor) {
  now = millis();
  if(now - lastControl > 50) {  //Run control loop
    lastControl = now;
    Motor1.updateSpeedDir();
    float target = Motor1.getTargetSpeedRPS();
    float measured = Motor1.getSpeedRPS();
    float error = pid.compute(target, measured, 0.050, 0.50);

    float feedforward = 0;

    if(target > 0 && abs(target) > 0.50) {  //Only set feedforward if speed > 0.50rps (avg speed at pwm 90)
      feedforward = 90;
    } else if (abs(target) > 0.50){
      feedforward = -90;
    }

    float control = error + feedforward;
    
    Motor1.applyControl(control);
  }
}
