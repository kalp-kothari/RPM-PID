
#pragma once
#include <Arduino.h>

class MotorDriver {
private:
    int pwm;
    int dir;
    static constexpr int MAX_COMMAND = 1000;
public:
    MotorDriver(int pwmPin, int dirPin): pwm(pwmPin), dir(dirPin) {}
    void begin() {
        pinMode(pwm, OUTPUT);
        pinMode(dir, OUTPUT);
        digitalWrite(dir, HIGH);
        analogWrite(pwm, 0);
    }
    void stopmotor() {
        analogWrite(pwm, 0);
    }
    void setPWM(int command) {
        command = constrain(command, 0, MAX_COMMAND);
        digitalWrite(dir, HIGH);
        int duty = command * 255L / MAX_COMMAND;        
        analogWrite(pwm, duty);
    }
};
