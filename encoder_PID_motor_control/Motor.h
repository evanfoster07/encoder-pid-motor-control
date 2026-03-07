#ifndef MOTOR_H
#define MOTOR_H

class Motor {
private:
  int pwmPin;
  int encoderAPin;
  int encoderBPin;

  long lastSampleTicks;
  unsigned long lastSampleTime;
  const int ticksPerRev = 20;
  float speedRPS;
  
  volatile long ticksA;
  volatile long ticksB;

public:
  Motor(int pwm, int encA, int encB);

  void setSpeed(int speed);
  void updateSpeed();
  void encoderISRA();
  void encoderISRB();
  float getSpeedRPS();
  int getDir();
  long getTicksA();
  long getTicksB();
};

#endif
