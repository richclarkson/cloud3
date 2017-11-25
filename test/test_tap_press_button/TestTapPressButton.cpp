#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

TapPressButton btn;

void setUp() {
  btn = TapPressButton();
}

void test_tap_after_interval(TapPressButton &btn, unsigned long startTime,
                           int interval, bool testType) {
  unsigned long timerVal = startTime;
  unsigned long endTime = startTime + interval;
  while (timerVal < endTime) {
    btn.updateInput(true, timerVal);
    TEST_ASSERT_FALSE(btn.isTap());
    timerVal++;
  }
  btn.updateInput(false, timerVal);
  if (testType == true) {
    TEST_ASSERT_TRUE(btn.isTap());
  } else {
    TEST_ASSERT_FALSE(btn.isTap());
  }
}

void testMultipleDebounces() {
  test_tap_after_interval(btn, 0, 30, false);
  test_tap_after_interval(btn, 0, 20, false);
  test_tap_after_interval(btn, 5000, 40, false);
  test_tap_after_interval(btn, 87465439876, 30, false);
}

void testMultipleTaps() {
  test_tap_after_interval(btn, 0, 60, true);
  test_tap_after_interval(btn, 1000, 300, true);
  test_tap_after_interval(btn, 49876123478163, 100, true);
}

void testIsPressAfterFiveHundredTimerValueIncrements() {
  int timerVal = 0;
  int endVal = 5000;
  while (timerVal < endVal) {
    btn.updateInput(true, timerVal);
    if (timerVal > 500) {
      TEST_ASSERT_TRUE(btn.isPress());
    } else {
      TEST_ASSERT_FALSE(btn.isPress());
    }
    timerVal++;
  }
}

void test_press_counts(TapPressButton &btn, int count, unsigned long startVal) {
  unsigned long timerVal = startVal;
  unsigned long endVal = startVal + 600 + (count * 1000);
  int pressCountCounter = 1;
  int timerCounter = 0;
  while (timerVal < endVal) {
    btn.updateInput(true, timerVal);
    if (timerVal - startVal > 1000 * pressCountCounter) {
      TEST_ASSERT_EQUAL(pressCountCounter, btn.getPressCount());
    }
    timerVal++;
    timerCounter++;
    if (timerCounter >= 1000) {
      pressCountCounter++;
      timerCounter = 0;
    }
  }
}

void testPressCounterGoesToTen() {
  test_press_counts(btn, 10, 0);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(testMultipleDebounces);
  RUN_TEST(testMultipleTaps);
  RUN_TEST(testIsPressAfterFiveHundredTimerValueIncrements);
  RUN_TEST(testPressCounterGoesToTen);

  UNITY_END();
}

#endif
