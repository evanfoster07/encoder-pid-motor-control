#include "Motor.h"
#include "PIDController.h"
#include <U8g2lib.h>

// ==============================
// Hardware Configuration
// ==============================

//GME12864 module
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);

//Hardware pins
const int motorPWM = 5;
const int mEncoderA = 2;
const int mEncoderB = 12;
const int rEncoderA = 3;
const int rEncoderB = 13;
const int dirAPin = 8;
const int dirBPin = 9;

//Hardware configuration constants
const unsigned long DISPLAY_INTERVAL_MS = 100;
const unsigned long CONTROL_INTERVAL_MS = 50;
const unsigned long RENC_DEBOUNCE_US = 2000;
const float TARGET_STEP_RPS = 0.01;
const float FEEDFORWARD_PWM = 90.0;
const float CONTROL_DT_S = 0.050;

//Global timing variables
unsigned long lastDisplayUpdate;
unsigned long lastControl;
volatile unsigned long lastREncUpdate;

//Buffer variables for calculating 1s average speed
const int AVG_WINDOW = 20;
float recentSpeedRPS[AVG_WINDOW];
float avgSpeedRPS = 0;
int bufferIndex = 0;
float speedSumRPS = 0;

//Rotary encoder input variables
volatile int rEncTicks = 0;
int lastREncTicks = 0;


//Motor and PID_Control objects
Motor motor(motorPWM, mEncoderA, mEncoderB, dirAPin, dirBPin);
PID_Control pid(20.0, 20.0, 1.0);



// =====================================
//  Motor and Rotary Encoder Interrupts
// =====================================

void encoderMotorWrapper() {  //wrapper function for encoder A interrupts
  motor.encoderISR();
}

void rotEncoderISR() {
  const unsigned long currentMicros = micros();
  if (currentMicros - lastREncUpdate > RENC_DEBOUNCE_US) { //Ignore quick successive interrupts for software debounce
    lastREncUpdate = currentMicros;
    bool A = digitalRead(rEncoderA);
    bool B = digitalRead(rEncoderB);

    if (A == B) {
      rEncTicks++;
    } else {
      rEncTicks--;
    }
  }
}


void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(mEncoderA), encoderMotorWrapper, RISING);
  attachInterrupt(digitalPinToInterrupt(rEncoderA), rotEncoderISR, FALLING);

  pinMode(rEncoderA, INPUT_PULLUP);
  pinMode(rEncoderB, INPUT_PULLUP);

  u8g2.begin();

  lastDisplayUpdate = millis();
  lastControl = millis();
}

void loop() {
  controlMotor();
  updateDisplay();

  noInterrupts();
  int ticks = rEncTicks;
  interrupts();

  if (ticks != lastREncTicks) {
    motor.setTargetSpeedRPS(rEncTicks * TARGET_STEP_RPS);
    lastREncTicks = ticks;
  }
}


// =================
//  Control Logic
// =================

void updateDisplay() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastDisplayUpdate > DISPLAY_INTERVAL_MS) { //wait at least 100 ms to update
    lastDisplayUpdate = currentMillis;
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_ncenB08_tr);

      u8g2.setCursor(0, 15);
      u8g2.print("Target Speed: ");
      u8g2.print(motor.getTargetSpeedRPS());

      u8g2.setCursor(0, 30);
      u8g2.print("1s Avg Speed: ");
      u8g2.print(avgSpeedRPS);

      u8g2.setCursor(0, 45);
      u8g2.print("Actual Speed: ");
      u8g2.print(motor.getSpeedRPS());

      u8g2.setCursor(0, 60);
      u8g2.print("   (all speed in RPS)");
    } while (u8g2.nextPage());
  }
}

void updateAverage(float newSpeed) {
  speedSumRPS -= recentSpeedRPS[bufferIndex];
  recentSpeedRPS[bufferIndex] = newSpeed;
  speedSumRPS += newSpeed;
  bufferIndex++;

  if (bufferIndex == AVG_WINDOW) {
    bufferIndex = 0;
  }

  avgSpeedRPS = speedSumRPS / AVG_WINDOW;
}

void controlMotor() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastControl > CONTROL_INTERVAL_MS) {  //Run control loop
    lastControl = currentMillis;
    motor.updateSpeedDir();
    const float target = motor.getTargetSpeedRPS();
    const float measured = motor.getSpeedRPS();

    // Compute PID correction based on target vs measured speed
    float pidOutput = pid.compute(target, measured, CONTROL_DT_S, 1.00, 0.30, 0.50, 0.80); //args are experimentally determined for given motor's performance

    float feedforward = 0;
    

    if (target > 0.01) {
      feedforward = FEEDFORWARD_PWM;
    } else if (target < -0.01) {
      feedforward = -FEEDFORWARD_PWM;
    }

    float control = pidOutput + feedforward;
    
    updateAverage(measured);
    motor.applyControl(control);
  }
}
