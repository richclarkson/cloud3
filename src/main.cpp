#include <Arduino.h>
#include <TapPressButton.h>

TapPressButton capSensor(7);

unsigned long pressTime = 0;

void setup() {
    // put your setup code here, to run once:
}

void loop() {
    capSensor.check();
}