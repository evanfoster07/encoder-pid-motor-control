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
  const float lowkp = 5;
  const float lowki = 5;
  const float lowkd = 1;

  float prevDerivative;
  float integral;
  float prevError;
  float prevTarget;

public:
  PID_Control(float p, float i, float d);
  PID_Control();
  
  float compute(float target, float measured, float dt, float threshold);

  void setKP(float p);
  void setKI(float i);
  void setKD(float d);

  float getPrevError();
  float getPrevDerivative();
  float getIntegral();
};

#endif