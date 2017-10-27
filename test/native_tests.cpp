#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

void testTapPressButtonExists() { TapPressButton btn; }

void testTapPressButtonInputIsTrue() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  TEST_ASSERT_TRUE(btn.getCurrentState());
}

void testTapPressButtonInputHasChanged() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(true, 50);
  TEST_ASSERT_FALSE(btn.stateHasChanged());
  btn.updateInput(false, 100);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(true, 101);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(false, 300);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
}

void testHasPressTimeElapsed() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 100);
  TEST_ASSERT_TRUE(btn.getPressTime() > 0);
}

void testPressInTapWindow() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 50);
  TEST_ASSERT_FALSE(btn.isPressInTapWindow());
  btn.updateInput(true, 51);
  TEST_ASSERT_TRUE(btn.isPressInTapWindow());
  btn.updateInput(true, 600);
  TEST_ASSERT_FALSE(btn.isPressInTapWindow());
}

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

void testTapAfterInterval(TapPressButton &btn, unsigned long startTime, int interval, bool testType) {
    unsigned long timerVal = startTime;
    unsigned long endTime = startTime + interval;
    while (timerVal < endTime) {
        btn.updateInput(true, timerVal);
        TEST_ASSERT_FALSE(btn.isTap());
        timerVal ++;
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
    testTapAfterInterval(btn, 0, 30, false);
    testTapAfterInterval(btn, 0, 20, false);
    testTapAfterInterval(btn, 5000, 40, false);
    testTapAfterInterval(btn, 87465439876, 30, false);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(testTapPressButtonExists);
  RUN_TEST(testTapPressButtonInputIsTrue);
  RUN_TEST(testTapPressButtonInputHasChanged);
  RUN_TEST(testHasPressTimeElapsed);
  RUN_TEST(testPressInTapWindow);
  RUN_TEST(testPressInPressWindow);
  RUN_TEST(testMultipleDebounces);

  UNITY_END();
}

#endif
