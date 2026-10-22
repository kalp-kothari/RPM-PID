#pragma once 
#include <Arduino.h>
#include<Encoder.h>

class EncoderRPM{
  private:
    long time;
    Encoder enc;
    long CPR;
    float rpm;
    long encoder_ticks;
  public:
    EncoderRPM(int pinA, int pinB, long cpr):enc(pinA, pinB),CPR(cpr),time(0),encoder_ticks(0),rpm(0.0){}
    void begin(){
      enc.write(0);
      time=micros();
      encoder_ticks=enc.read();
    }
    void update(){
      long curr_time=micros();
      long curr_ticks=enc.read();
      long elapsed=curr_time-time;
      float dt=elapsed / 1000000.0;
      long delEnc=curr_ticks-encoder_ticks;
      rpm=((float)delEnc / (float)CPR)* (60.0 / dt);
      time=curr_time;
      encoder_ticks=curr_ticks;
    }
    float getRpm(){
      return rpm;
    }
};