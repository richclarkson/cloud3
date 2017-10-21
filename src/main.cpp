#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>

TapPressButton capSensor = TapPressButton();

const int btnPin = 7;
bool LEDVal = false;

void setup() {
    pinMode(btnPin, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    capSensor.update(digitalRead(btnPin), millis());
    if (capSensor.isTap()) {
        LEDVal = !LEDVal;
        // do tap things
    }
    else if (capSensor.isPress()) {
        // int pressCount = capSensor.getPressCount();
        // do press things, likely with pressCount
    }

    digitalWrite(LED_BUILTIN, LEDVal);
}

#endif