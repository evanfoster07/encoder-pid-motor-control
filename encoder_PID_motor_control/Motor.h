#ifndef MOTOR_H
#define MOTOR_H

class Motor {
private:
  int pwmPin;
  int encoderAPin;
  int encoderBPin;

  long lastSampleTicks;
  unsigned long lastSampleTime;
  const int ticksPerRev = 8;
  const int gearRatio = 120;
  float speedRPS;
  int dir;
  
  volatile long ticks;

public:
  Motor(int pwm, int encA, int encB);

  void setSpeed(int speed);
  void updateSpeedDir();
  void encoderISRA();
  float getSpeedRPS();
  int getDir();
  long getTicks();
};

#endif
