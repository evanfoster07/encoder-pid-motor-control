#include "PIDController.h"
#include <Arduino.h>

// PIDController.cpp
// PID controller implementation used for motor speed regulation.
// Computes control output based on target and measured speed.

PID_Control::PID_Control(float p, float i, float d) {
  setkp = p;
  setki = i;
  setkd = d;

  prevDerivative = 0;
  integral = 0;
  maxIntegral = 0;
  prevError = 0;
  prevTarget = 0;
}

PID_Control::PID_Control() {  //default constructor
  setkp = 50;
  setki = 50;
  setkd = 0.1 * kp;

  prevDerivative = 0;
  integral = 0;
  prevError = 0;
  prevTarget = 0;
}

float PID_Control::compute(float target, float measured, float dt, float maxInt, float lowThreshold, float midThreshold, float highThreshold) {
  if (target == 0) return 0; //Stop motor if target is 0
  if (abs(target - prevTarget) > 0.001) {  //If target changed, alter kp, ki, kd and maxIntegral based on speed 
    if (abs(target) < lowThreshold) {
      kp = setkp;
      ki = setki;
      kd = setkd;
      maxIntegral = maxInt;
    } else if (abs(target) < midThreshold){
      kp = 2 * setkp;
      ki = 2 * setki;
      kd = 2 * setkd;
      maxIntegral = 3 * maxInt;
    } else if (abs(target) < highThreshold) {
      kp = setkp * 4;
      ki = setki * 4;
      kd = setkd * 4;
      maxIntegral = 3 * maxInt;
    } else {
      kp = setkp * 8;
      ki = setki * 8;
      kd = setkd * 8;
      maxIntegral = 4 * maxInt;
    }
  }

  float error = target - measured;
  if (abs(integral) <= maxIntegral) { //restrict integral to maxIntegral to avoid excessive buildup
    integral += error * dt;
  } else if (integral < 0) {
    integral = maxIntegral * -1;
  } else {
    integral = maxIntegral;
  }
    
  float derivative;
  if (dt != 0) {
    derivative = (error - prevError) / dt;
  }

  prevError = error;
  prevDerivative = derivative;
  prevTarget = target;

  return error * kp + integral * ki + derivative * kd;
}

void PID_Control::setKP(float p) {
  setkp = p;
}

void PID_Control::setKI(float i) {
  setki = i;
}

void PID_Control::setKD(float d) {
  setkd = d;
}

float PID_Control::getPrevError() {
  return prevError;
}

float PID_Control::getPrevDerivative() {
  return prevDerivative;
}

float PID_Control::getIntegral() {
  return integral;
}