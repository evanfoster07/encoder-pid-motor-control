#include "PIDController.h"
#include <Arduino.h>

PID_Control::PID_Control(float p, float i, float d) {
  setkp = p;
  setki = i;
  setkd = d;

  prevDerivative = 0;
  integral = 0;
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

float PID_Control::compute(float target, float measured, float dt, float threshold) {
  if(abs(target - prevTarget) > 0.001) {  //If target changed, alter kp, ki, kd based on speed
    integral = 0;
    if (abs(target) < threshold) {
      kp = lowkp;
      ki = lowki;
      kd = lowkd;
    } else {
      kp = setkp;
      ki = setki;
      kd = setkd;
    }
  }

  float error = target - measured;
  integral += error * dt;
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