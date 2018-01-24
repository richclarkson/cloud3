#ifndef UNIT_TEST

#include <Arduino.h>
#include <StateManager.h>
#include <TapPressButton.h>

TapPressButton capSensor;
StateManager mainState;

const int capPin = 19;
const int touchTime = 1000;
unsigned long loopTime;
bool isTouch;

void setup() {
  Serial.begin(9600);
  loopTime = 0;
  capSensor = TapPressButton(50, 300, 1000, 1000);
  mainState = StateManager();
  isTouch = false;
}

void loop() {
  isTouch = touchRead(capPin) > touchTime;
  loopTime = millis();
  // Serial.println(touchRead(capPin));
  capSensor.update(isTouch, loopTime);
  if (capSensor.isTap()) {
    mainState.tap();
  }
  if (capSensor.isPress()) {
    mainState.press();
  }
}

#endif
