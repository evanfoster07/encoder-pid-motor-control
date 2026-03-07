#include "Motor.h"

const int motorPWM = 5;
const int encoderA = 2;
const int encoderB = 3;

Motor Motor1(motorPWM, encoderA, encoderB);

//Wrapper functions for encoder interrupts
void encoderAWrapper() {
  Motor1.encoderISRA();
}

void encoderBWrapper() {
  Motor1.encoderISRB();
}


void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoderA), encoderAWrapper, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderB), encoderBWrapper, RISING);
}


void loop() {
  while(!Serial.available()) {Motor1.updateSpeed();}  //Update speed every 0.5s until input

  //Take Serial input to change speed/display data
  String in = Serial.readStringUntil('\n');
  in.trim();  
  if(in == "A") {
    Serial.print("A ticks: ");
    Serial.println(Motor1.getTicksA());
  }
  else if(in == "B") {
    Serial.print("B ticks: ");
    Serial.println(Motor1.getTicksB());
  }
  else if(in == "Speed") {
    Serial.print("Speed: ");
    Serial.println(Motor1.getSpeedRPS());
  }
  else if(isInteger(in)) {  //Change speed if Serial input is an integer
    int speed = in.toInt();
    Motor1.setSpeed(speed);
  }
}

bool isInteger(String s) {
  if(s.length() == 0) return false;
  for(int i = 0; i < s.length(); i++) {
    if(!isdigit(s[i])) {
      return false;
    }
  }
  return true;
}

