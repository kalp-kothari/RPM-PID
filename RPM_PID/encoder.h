#pragma once
#include <Arduino.h>
class encoder {
private:
    int pinA;
    int pinB;
    volatile long position = 0;
    volatile int lastA = 0;
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    static void handleInterrupt(void* arg) {
        encoder* enc = (encoder*)arg;
        enc->update();
    }
    void update() {
        int a = digitalRead(pinA);
        int b = digitalRead(pinB);
        portENTER_CRITICAL_ISR(&mux);
        if (a != lastA) {
            if (a == b) {
                position++;
            } else {
                position--;
            }
            lastA = a;
        }
        portEXIT_CRITICAL_ISR(&mux);
    }
public:
    encoder(int a, int b) {
        pinA = a;
        pinB = b;
    }
    void Begin() {
        pinMode(pinA, INPUT_PULLUP);
        pinMode(pinB, INPUT_PULLUP);
        lastA = digitalRead(pinA);
        attachInterruptArg(digitalPinToInterrupt(pinA),handleInterrupt,this,CHANGE);
    }
    long getPosition() {
        portENTER_CRITICAL(&mux);
        long count = position;
        portEXIT_CRITICAL(&mux);
        return count;
    }
    void resetPosition() {
        portENTER_CRITICAL(&mux);
        position = 0;
        lastA = digitalRead(pinA);
        portEXIT_CRITICAL(&mux);
    }
};
