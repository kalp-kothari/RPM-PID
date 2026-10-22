#pragma once
#include <Arduino.h>
#include <math.h>

class PID {
private:
    static constexpr float max_pwm = 200.0f;
    float kp, ki, kd;
    float integral, previousError;
    bool first = true;  
public:
    PID() : kp(0.0f), ki(0.0f), kd(0.0f),integral(0.0f), previousError(0.0f) {}
    void setTunings(float kp_, float ki_, float kd_) {
        kp = kp_; 
        ki = ki_;
        kd = kd_;
    }
    void reset() {
        integral = 0.0f;
        previousError = 0.0f;
        first = true;
    }
    float compute(float targetRPM, float measuredRPM, float dt) {
        float error = targetRPM - measuredRPM;
        float P = kp * error;
        if (ki > 0.0f) {
            float lim = max_pwm / ki;
            integral = constrain(integral + error * dt, -max_pwm, max_pwm);
        } 
        else {
            integral = 0.0f;
        }
        float I = ki * integral;
        float derivative = first ? 0.0f : (error - previousError) / dt;
        first = false;
        float D = kd * derivative;
        float output = constrain((P + I + D), -max_pwm, max_pwm);
        previousError = error;
        return output;
    }
};