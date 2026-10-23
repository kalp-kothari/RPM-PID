#include <Arduino.h>
#include <DS4Arduino.h>
#include <string.h>
#include <math.h>
#include "PID.h"
#include "EncoderRPM.h"
#include "MotorDriver.h"

#define PWM_PIN 22
#define DIR_PIN 23
#define ENC_A 26
#define ENC_B 25
#define CPR 1885
#define MAX_RPM 200

DS4Controller ds4;
MotorDriver motor(PWM_PIN, DIR_PIN);
EncoderRPM encoder(ENC_A, ENC_B, CPR);
PID pid;

float kp = 20.0f;
float ki = 11.0f;
float kd = 0.0f;
float target = 0.0f;
unsigned long lastUs = 0;
unsigned long lastPrintMs = 0;

void readSerial() {
    String s = Serial.readStringUntil('\n');
    s.trim();
    s.toLowerCase();
    char name[8] = {0};
    float value = 0.0f;
    if (sscanf(s.c_str(), "%7s %f", name, &value) != 2 ||
        !isfinite(value) || value < 0.0f) {
        return;
    }
    if (!strcmp(name, "kp")) {
        kp = value;
    } else if (!strcmp(name, "ki")) {
        ki = value;
    } else if (!strcmp(name, "kd")) {
        kd = value;
    } else {
        Serial.println("Unknown command. Use kp, ki, or kd.");
        return;
    }
    pid.setTunings(kp, ki, kd);
    Serial.printf("Kp=%.3f Ki=%.3f Kd=%.3f\n",kp, ki, kd);
}
void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    motor.begin();
    encoder.begin();
    pid.setTunings(kp, ki, kd);
    ds4.begin();
    lastUs = micros();
}
void loop() {
    ds4.update();
    readSerial();
   if (ds4.connected()) {
    int ly = ds4.leftStickY();
        target=map(ly,120,0,0,MAX_RPM);// taking consideration that deadzone is 8 and center is 128 
        target = constrain(target, 0.0f, (float)MAX_RPM);
   }
    else {
        target = 0.0f;
        pid.reset();
        motor.stopmotor();
    }
    unsigned long now = micros();
    unsigned long elapsed = now - lastUs;
    if (elapsed < 20000UL) {
        delay(1);
        return;
    }
    float dt = elapsed / 1000000.0f;
    lastUs = now;
    encoder.update();
    float measuredRPM = encoder.getRpm();
    float pwm = 0.0f;
    if (target > 0.5f) {
        pwm = pid.compute(target, measuredRPM, dt);
    } else {
        pid.reset();
        pwm = 0.0f;
    }
    motor.setPWM((int)(pwm));
    if (millis() - lastPrintMs >= 100) {
        lastPrintMs = millis();
        Serial.printf("Target:%.1f,RPM:%.1f,Kp:%.2f,Ki:%.2f,Kd:%.2f\n",target, measuredRPM,kp, ki, kd);
    }
}
