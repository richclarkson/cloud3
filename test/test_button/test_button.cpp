#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_debounce(unsigned long start) {
  TapPressButton btn = TapPressButton();
  unsigned long timerVal = start;
  unsigned long timerEnd = start;
  int TEST_PATTERN[9] = {10, 2, 10, 3, 30, 10, 45, 35, 50};
  for (int i = 0; i < 9; i++) {
    timerEnd += TEST_PATTERN[i];
  }
  int testPatternIndex = 0;
  unsigned long prevTimerTestVal = timerVal;
  bool btnState = true;
  while (timerVal < timerEnd) {
    btn.update(btnState, timerVal);
    timerVal++;
    if (timerVal - prevTimerTestVal == TEST_PATTERN[testPatternIndex]) {
      prevTimerTestVal = timerVal;
      testPatternIndex++;
      btnState = !btnState;
    }
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
  }
}

void Test_Debounces() {
  test_debounce(0);
  test_debounce(300);
  test_debounce(3287346);
  test_debounce(4294967295ul);
}

void test_tap(unsigned long start, unsigned long interval, bool testFlag) {
  TapPressButton btn = TapPressButton();
  unsigned long timerVal = start;
  unsigned long timerMax = interval + start;
  while (timerVal < timerMax) {
    btn.update(true, timerVal);
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
    timerVal++;
  }
  btn.update(false, timerVal);
  if (testFlag) {
    TEST_ASSERT_TRUE(btn.isTap())
  } else {
    TEST_ASSERT_FALSE(btn.isTap());
  }
  TEST_ASSERT_FALSE(btn.isPress());
}

void Test_Taps() {
  test_tap(0, 30, false);
  test_tap(0, 50, false);
  test_tap(0, 51, true);
  test_tap(0, 350, true);
  test_tap(0, 351, false);
  test_tap(300000000, 30, false);
  test_tap(300000000, 50, false);
  test_tap(300000000, 51, true);
  unsigned long startTime = 4294967295ul;
  test_tap(startTime, 350, true);
  test_tap(startTime, 351, false);
}

void test_press(unsigned long start) {
  TapPressButton btn = TapPressButton();
  unsigned long timerVal = start;
  unsigned long timerEnd = start + 3000;
  while (timerVal <= timerEnd) {
    btn.update(true, timerVal);
    if (timerVal - start < 500) {
      TEST_ASSERT_FALSE(btn.isPress());
    } else {
      TEST_ASSERT_TRUE(btn.isPress());
    }
    timerVal++;
  }
}

void Test_Presses() {
  test_press(0);
  test_press(1000);
  test_press(893459837);
}

void test_press_count(unsigned long start, unsigned long interval,
                      int pressCount) {
  TapPressButton btn = TapPressButton();
  unsigned long timerVal = start;
  unsigned long timerEnd = start + interval;
  while (timerVal < timerEnd) {
    btn.update(true, timerVal);
    timerVal++;
  }
  TEST_ASSERT_TRUE(btn.isPress());
  TEST_ASSERT_EQUAL_INT(pressCount, btn.getPressCount());
}

void Test_Press_Counts() {
  test_press_count(0, 501, 1);
  test_press_count(0, 1000, 1);
  test_press_count(0, 1500, 1);
  test_press_count(0, 1501, 2);
  test_press_count(0, 2000, 2);
  test_press_count(0, 2500, 2);
  test_press_count(0, 3000, 3);
  test_press_count(100, 4000, 4);
  test_press_count(1000000, 5000, 5);
  test_press_count(4294967295ul, 6000, 6);
  test_press_count(4294967295ul, 7000, 7);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(Test_Debounces);
  RUN_TEST(Test_Taps);
  RUN_TEST(Test_Presses);
  RUN_TEST(Test_Press_Counts);

  UNITY_END();
}

#endif