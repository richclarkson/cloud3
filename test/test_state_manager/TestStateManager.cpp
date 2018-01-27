#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>
#include <iostream>

StateManager sm;

using namespace std;

void fftCallBack() {
  cout << "fft call back called" << "\n";
}

void setUp(void) {
  sm = StateManager();
  sm.registerFFTCallback(fftCallBack);
}

void tearDown(void) {}

void test_machine_can_handle_taps() {
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.press();
}

void test_call_back_called() {
  sm.callFFTCallback();
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_machine_can_handle_taps);
  RUN_TEST(test_call_back_called);

  UNITY_END();
}

#endif
