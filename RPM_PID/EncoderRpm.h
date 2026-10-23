#pragma once
#include <Arduino.h>
#include "encoder.h"
class EncoderRPM {
private:
    encoder enc;
    long CPR;
    long prev_pos;
    unsigned long prev_time;
    float rpm;
public:
    EncoderRPM(int pinA, int pinB, long cpr): enc(pinA, pinB),CPR(cpr),prev_pos(0),prev_time(0),rpm(0.0f) {}
    void begin() {
        enc.Begin();
        prev_pos = enc.getPosition();
        prev_time = micros();
        rpm = 0.0f;
    }
    void update() {
        unsigned long curr_time = micros();
        long curr_pos = enc.getPosition();
        unsigned long elapsed = curr_time - prev_time;
        float dt = elapsed / 1000000.0f;
        long deltaPosition = curr_pos - prev_pos;
        rpm = (deltaPosition * 60.0f) /(CPR * dt);
        prev_pos = curr_pos;
        prev_time = curr_time;
    }
    float getRpm() const {
        return rpm;
    }
    void reset() {
        enc.resetPosition();
        prev_pos = 0;
        prev_time = micros();
        rpm = 0.0f;
    }
};
