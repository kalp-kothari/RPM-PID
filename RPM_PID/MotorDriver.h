#pragma once
#include <Arduino.h>
#define MAX_PWM 200

class MotorDriver{
    private:
      int dir;
      int pwm;
    public:
      MotorDriver(int _dir , int _pwm):dir(_dir),pwm(_pwm){}
    void stopmotor(){
        analogWrite(pwm, 0);
    }
    void begin(){
        pinMode(dir, OUTPUT);
        pinMode(pwm, OUTPUT);
        stopmotor();
    }
    void setSpeed(int speed){
      if(speed>0.0f){
        digitalWrite(dir, HIGH);
        analogWrite(pwm, map(speed,0,MAX_PWM,0,200));
      }
      else{
        digitalWrite(dir, LOW);
        analogWrite(pwm, map(speed,0,MAX_PWM,0,200));
      }
    }
};