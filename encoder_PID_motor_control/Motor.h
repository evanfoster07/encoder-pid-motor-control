#ifndef MOTOR_H
#define MOTOR_H

class Motor {
private:
  int pwmPin;
  int encoderAPin;
  int encoderBPin;
  int dirAPin;
  int dirBPin;

  long lastSampleTicks;
  unsigned long lastSampleTime;
  const int ticksPerRev = 8;
  const int gearRatio = 120;
  int speedPWM;
  float speedRPS;
  float targetSpeedRPS;
  int measuredDirection;
  int setDirection;
  
  volatile long ticks;

public:
  Motor(int pwm, int encA, int encB, int dirAP, int dirBP);

  void setSpeedPWM(int pwm);
  void setTargetSpeedRPS(float rps);
  void setDir(int dir);
  void updateSpeedDir();
  void encoderISRA();
  void applyControl(float err);
  
  float getTargetSpeedRPS();
  float getSpeedRPS();
  int getSpeedPWM();
  int getMeasuredDir();
  int getSetDirection();
  long getTicks();
};

#endif
