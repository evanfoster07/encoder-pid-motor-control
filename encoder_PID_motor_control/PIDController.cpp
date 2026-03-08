#include "PIDController.h"
#include <Arduino.h>

PID_Control::PID_Control(float p, float i, float d) {
  kp = p;
  ki = i;
  kd = d;

  prevError = 0;
}

float PID_Control::compute(float target, float measured, float dt) {
  float error = target - measured;

  return error * kp;
}