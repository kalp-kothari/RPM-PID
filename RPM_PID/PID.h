#pragma once
#include <Arduino.h>
class PID {
private:
    static constexpr float MAX_PWM = 1000.0f;
    float kp, ki, kd;
    float integral;
    float previousError;
    bool first;

public:
    PID(): kp(0.0f), ki(0.0f), kd(0.0f),integral(0.0f), previousError(0.0f),first(true) {}
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
        if (targetRPM <= 0.0f) {
            reset();
            return 0.0f;
        }
        float error = targetRPM - measuredRPM;
        float P = kp * error;
        float derivative =first ? 0.0f : (error - previousError) / dt;
        float D = kd * derivative;
        if (ki > 0.0f) {
            integral = constrain(integral + error * dt,-MAX_PWM,MAX_PWM);
        }
        else integral = 0.0f;
        float output = P + ki * integral + D;
        output = constrain(output, 0.0f, MAX_PWM);
        previousError = error;
        first = false;
        return output;
    }
};
