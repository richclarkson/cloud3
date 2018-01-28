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

// Sound Library hooks & data
float soundLevel;
const int FFT_RESOLUTION = 256;
float fftArray[FFT_RESOLUTION / 2];

void analyzeFFT() {
  // run FFT analysis function
  // store the resulting data to the `FFTArray` variable
}

void analyzeLevel() {
  // run Level analysis function
  // store resulting value to the `soundLevel` variable
}

void setup() {
  Serial.begin(9600);
  loopTime = 0;
  capSensor = TapPressButton(50, 300, 1000, 1000);
  mainState = StateManager();
  mainState.registerFFT(analyzeFFT, &fftArray[0]);
  mainState.registerLevel(analyzeLevel, &soundLevel);
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
  mainState.update();
}

#endif
