#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

void testTapPressButtonExists() { TapPressButton btn; }

void testPressInPressWindow() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 500);
  TEST_ASSERT_FALSE(btn.isPressInPressWindow());
  btn.updateInput(true, 501);
  TEST_ASSERT_TRUE(btn.isPressInPressWindow());
  btn.updateInput(true, 1500);
  TEST_ASSERT_TRUE(btn.isPressInPressWindow());
}

void test_TapAfterInterval(TapPressButton &btn, unsigned long startTime,
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
  TapPressButton btn;
  test_TapAfterInterval(btn, 0, 30, false);
  test_TapAfterInterval(btn, 0, 20, false);
  test_TapAfterInterval(btn, 5000, 40, false);
  test_TapAfterInterval(btn, 87465439876, 30, false);
}

void testMultipleTaps() {
  TapPressButton btn;
  test_TapAfterInterval(btn, 0, 60, true);
  test_TapAfterInterval(btn, 1000, 300, true);
  test_TapAfterInterval(btn, 49876123478163, 100, true);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(testTapPressButtonExists);
  RUN_TEST(testPressInPressWindow);
  RUN_TEST(testMultipleDebounces);
  RUN_TEST(testMultipleTaps);

  UNITY_END();
}

#endif
