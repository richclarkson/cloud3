#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>
#include <iostream>

StateManager sm;

using namespace std;

int testVal = 0;

void fftCallBack() {
  testVal = 1;
}

void levelCallback() {
  testVal = 2;
}

void setUp(void) {
  sm = StateManager();
  sm.registerFFTCallback(fftCallBack);
  sm.registerLevelCallback(levelCallback);
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

void test_fft_callback() {
  sm.callFFTCallback();
  TEST_ASSERT_EQUAL(1, testVal);
}

void test_level_callback() {
  sm.callLevelCallback();
  TEST_ASSERT_EQUAL(2, testVal);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_machine_can_handle_taps);
  RUN_TEST(test_fft_callback);
  RUN_TEST(test_level_callback);

  UNITY_END();
}

#endif
