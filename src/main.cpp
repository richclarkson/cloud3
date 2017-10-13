#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>

TapPressButton capSensor = TapPressButton();

const int btnPin = 7;


void setup() {
    pinMode(btnPin, INPUT);
}

void loop() {
    capSensor.update(digitalRead(btnPin), millis());
}

#endif