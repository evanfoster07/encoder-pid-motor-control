#include "Motor.h"

const int motorPWM = 5;
const int encoderA = 2;
const int encoderB = 3;
const int dirAPin = 8;
const int dirBPin = 9;

Motor Motor1(motorPWM, encoderA, encoderB, dirAPin, dirBPin);

//Wrapper function for encoder A interrupts
void encoderAWrapper() {
  Motor1.encoderISRA();
}


void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoderA), encoderAWrapper, RISING);
}

void loop() {
  while(!Serial.available()) {Motor1.updateSpeedDir();}  //Update speed every 0.5s until input

  //Take Serial input to change speed/display data
  String input = Serial.readStringUntil('\n');
  input.trim();
  testMotor(input);
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

void testMotor(String in) {
  if(in == "T") {
    Serial.print("Ticks: ");
    Serial.println(Motor1.getTicks());
  } else if(in == "Speed") {
    Serial.print("Speed: ");
    Serial.print(Motor1.getSpeedRPS());
    Serial.println("rps");
  } else if (in == "D") {
    int dir = Motor1.getDir();
    Serial.print("Direction: ");
      if (dir == 0){
        Serial.println("still");
      } else if(dir == -1) {
        Serial.println("clockwise");
      } else {
        Serial.println("counter-clockwise");
      }
  } else if(in == "C") {  //Change direction (clockwise or counter-clockwise)
    Motor1.setDir(-1);
  } else if (in == "CC") {
    Motor1.setDir(1);
  } else if(isInteger(in)) {  //Change speed if Serial input is an integer
    int speed = in.toInt();
    Motor1.setSpeed(speed);
  }
}

