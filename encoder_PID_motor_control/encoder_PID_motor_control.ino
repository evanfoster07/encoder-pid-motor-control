#include <math.h>

volatile long ticksA = 0;
volatile long ticksB = 0;

const int motor = 5;
const int encoderChanA = 2;
const int encoderChanB = 3;

void encoderISRA() {
  ticksA++;
}

void encoderISRB() {
  ticksB++;
}

float getSpeedRPS() {
  long ticks1 = ticksA;
  unsigned long now = millis();
  while(millis() - now < 1000) {}   
  return (ticksA - ticks1) / 20.0;  //20 ticks/revolution
}

float getSpeedRPM() {
  long ticks1 = ticksA;
  unsigned long now = millis();
  while(millis() - now < 1000) {}   
  return ((ticksA - ticks1) / 20.0) * 60;  //20 ticks/revolution, 1rps = 60rpm
}


void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);
  pinMode(encoderChanA, INPUT);
  pinMode(encoderChanB, INPUT);
  digitalWrite(motor, LOW);
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(encoderChanA), encoderISRA, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderChanB), encoderISRB, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available()) {}
  
  String input = Serial.readStringUntil('\n');
  int pwm = input.toInt();

  analogWrite(motor, pwm);
  delay(500);
  float speedRPS = getSpeedRPS();
  Serial.println(speedRPS, 2);
  analogWrite(motor, 0);
}


