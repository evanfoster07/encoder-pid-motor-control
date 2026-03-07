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
  int direction;
  
  volatile long ticks;

public:
  Motor(int pwm, int encA, int encB, int dirAP, int dirBP);

  void setSpeed(int pwm);
  void setDir(int dir);
  void updateSpeedDir();
  void encoderISRA();
  float getSpeedRPS();
  int getDir();
  long getTicks();
};

#endif
