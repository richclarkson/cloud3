#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>
#include <BasicStateMachine.h>
#include <SaberStateHandler.h>

TapPressButton capSensor;

BasicStateMachine mainState;
BasicStateMachine settingState;
BasicStateMachine lampState;

const int capPin = 23;
const int touchTime = 50;
unsigned long loopTime;
bool isTouch;

void setup() {
    mainState = BasicStateMachine(7);
    settingState = BasicStateMachine(5);
    lampState = BasicStateMachine(4);

    loopTime = 0;

    capSensor = TapPressButton();
    isTouch = false;
}

void loop() {
    isTouch = touchRead(capPin) > touchTime;
    loopTime = millis();
    capSensor.update(isTouch, loopTime);
    if (capSensor.isTap()) {
        mainState.next()
    }
    if (capSensor.isTouch()) {
        if (mainState)
    }
}

#endif
