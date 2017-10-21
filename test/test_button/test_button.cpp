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

  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(Test_Debounces);

  UNITY_END();
}

#endif