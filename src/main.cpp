#ifndef UNIT_TEST

#include <Arduino.h>
#include <TapPressButton.h>
#include <BasicStateMachine.h>

TapPressButton capSensor;

BasicStateMachine mainState;
BasicStateMachine settingState;
BasicStateMachine lampState;

void setup() {
    mainState = BasicStateMachine(7);
    settingState = BasicStateMachine(5);
    lampState = BasicStateMachine(4);
    capSensor = TapPressButton();
}

void loop() {

}

#endif
