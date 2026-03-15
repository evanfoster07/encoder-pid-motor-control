#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PID_Control {
private:
  float kp;
  float ki;
  float kd;

  float setkp;
  float setki;
  float setkd;

  float prevDerivative;
  float integral;
  float maxIntegral;
  float prevError;
  float prevTarget;

public:
  PID_Control(float p, float i, float d);
  PID_Control();
  
  float compute(float target, float measured, float dt, float maxInt, float lowThreshold, float midThreshold, float highThreshold);

  void setKP(float p);
  void setKI(float i);
  void setKD(float d);

  float getPrevError();
  float getPrevDerivative();
  float getIntegral();
};

#endif