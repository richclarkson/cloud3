#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>

StateManager sm;

enum testState {none, fft, level};

testState testVal = none;

void fftCallBack() {
  testVal = fft;
}

void levelCallback() {
  testVal = level;
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
  TEST_ASSERT_EQUAL(fft, testVal);
}

void test_level_callback() {
  sm.callLevelCallback();
  TEST_ASSERT_EQUAL(level, testVal);
}

void test_fallingdot_updates_level() {
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(level, testVal);
}

void test_ripple_updates_fft() {
  sm.tap();
  sm.tap();
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(fft, testVal);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_machine_can_handle_taps);
  RUN_TEST(test_fft_callback);
  RUN_TEST(test_level_callback);
  RUN_TEST(test_fallingdot_updates_level);
  RUN_TEST(test_ripple_updates_fft);

  UNITY_END();
}

#endif
