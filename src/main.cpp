#include <Arduino.h>

// 16 is D0 on esp8266. Also blinks the onboard LED.
const int FAN_PWM_PIN = 16;

void setup() {
    pinMode(FAN_PWM_PIN, OUTPUT);
    analogWrite(FAN_PWM_PIN, 30);
}

void loop() {}
