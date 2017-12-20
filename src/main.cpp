#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>
#include <StateManager.h>

TapPressButton capSensor;
StateManager mainState;

const int capPin = 23;
const int touchTime = 50;
unsigned long loopTime;
bool isTouch;

void setup() {
    loopTime = 0;
    capSensor = TapPressButton();
    // mainState = StateManager();
    // Serial.begin(9600);
    isTouch = false;
}

void loop() {
    isTouch = touchRead(capPin) > touchTime;
    loopTime = millis();
    capSensor.update(isTouch, loopTime);
    // if (capSensor.isTap()) {
        // mainState.tap();
    // }
    // if (capSensor.isPress()) {
    //     mainState.press();
    // }
}

#endif
