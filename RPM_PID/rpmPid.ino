#include <Arduino.h>
#include "PID.h"
#include "EncoderRPM.h"
#include "MotorDriver.h"
#include <DS4Arduino.h>
DS4Controller ds4;

#define PWM_PIN 4
#define DIR_PIN 18
#define ENC_A   22
#define ENC_B   23
#define CPR     1885
#define MAX_RPM 200

MotorDriver motor(PWM_PIN, DIR_PIN);
EncoderRPM encoder(ENC_A, ENC_B, CPR);
PID pid;
float kp = 0.5, ki = 0, kd = 0;
unsigned long lastUs=0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  motor.begin();                 
  encoder.begin();
  ds4.begin();
  pid.setTunings(kp, ki, kd);
  Serial.println("Type: kp 1.2   ki 3   kd 0.01");
}

void readSerial() {
  String s = Serial.readStringUntil('\n');
  s.toLowerCase();
  char name[8];
  float v;
  if (sscanf(s.c_str(), "%7s %f", name, &v) == 2 && v >= 0) {
    if (!strcmp(name, "kp")) kp = v;
    else if (!strcmp(name, "ki")) ki = v;
    else if (!strcmp(name, "kd")) kd = v;
    pid.setTunings(kp, ki, kd);
    Serial.printf("Kp=%.3f Ki=%.3f Kd=%.3f\n", kp, ki, kd);
  }
}

void loop() {
  ds4.update();
  readSerial();
  unsigned long now = micros();
  if (now - lastUs < 20000) {        
    delay(1);                        
    return;
  }
  lastUs = now;
  float dt = 0.02f;
  float target = 0;                  
  if (ds4.connected()) {
    int r2 = ds4.leftStickY();         
    if (r2 == 0) target = MAX_RPM;
  }
  encoder.update();
  float rpm = encoder.getRpm();
  float speed = 0;
  if (target > 0) speed = pid.compute(target, rpm, dt);
  else pid.reset();
  motor.setSpeed((int)speed);         
  Serial.printf("Target:%.1f,RPM:%.1f\n", target, rpm);
}