#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PID_Control {
private:
  float kp;
  float ki;
  float kd;

  float prevError;

public:
  PID_Control(float p, float i, float d);
  
  float compute(float target, float measured, float dt);

};

#endif