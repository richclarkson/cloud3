#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>

TapPressButton capSensor;

const int btnPin = 7;
bool LEDVal = false;

void setup() {
    pinMode(btnPin, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    capSensor.updateInput(digitalRead(btnPin), millis());
    delay(5);
    if (capSensor.isTap()) {
        LEDVal = !LEDVal;
        Serial.println("Tap");
    } else if (capSensor.isPress()) {
        Serial.println("Press");
    }
    digitalWrite(LED_BUILTIN, LEDVal);
}

#endif