#include "PIDController.h"
#include <Arduino.h>

PID_Control::PID_Control(float p, float i, float d) {
  kp = p;
  ki = i;
  kd = d;

  integral = 0;
  prevError = 0;
}

float PID_Control::compute(float target, float measured, float dt) {
  float error = target - measured;
  integral += error * dt;

  return error * kp + integral * ki;
}

void PID_Control::setkp(float p) {
  kp = p;
}

void PID_Control::setki(float i) {
  ki = i;
}

void PID_Control::setkd(float d) {
  kd = d;
}