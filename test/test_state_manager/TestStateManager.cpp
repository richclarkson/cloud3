#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>

StateManager sm;

const float TEST_FFT = .5;
const float TEST_LEVEL = .9;

float levelVal = 0;
float fftArray[128];

void fftCallBack() {
  for (int i=0; i<128; i++) {
    fftArray[i] = TEST_FFT;
  }
}

void levelCallback() {
  levelVal = TEST_LEVEL;
}

void setUp(void) {
  sm = StateManager();
  sm.registerFFT(fftCallBack, &fftArray[0]);
  sm.registerLevel(levelCallback, &levelVal);
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
  TEST_ASSERT_EQUAL(TEST_FFT, fftArray[10]);
}

void test_level_callback() {
  sm.callLevelCallback();
  TEST_ASSERT_EQUAL(TEST_LEVEL, levelVal);
}

void test_fallingdot_updates_level() {
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(TEST_LEVEL, levelVal);
}

void test_ripple_updates_fft() {
  sm.tap();
  sm.tap();
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(TEST_FFT, fftArray[56]);
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
