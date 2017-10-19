#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void Given_ButtonCycled_When_BelowDebounceThreshold_Then_TapAndPressIsFalse() {
  float timerVal = 0;
  const long timerMax = pow(10, 6);
  int counter = 0;
  float factor = .1;
  const int debounceVal = 50;
  TapPressButton btn = TapPressButton();
  btn.setDebounce(debounceVal);
  while (timerVal < timerMax) {
    if (timerVal - counter >= debounceVal * factor) {
      btn.update(false, timerVal);
      counter = timerVal;
      factor += .1;
    } else {
      btn.update(true, timerVal);
    }
    if (factor > .9) {
      factor = .1;
    }
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
    timerVal++;
  }
}

  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);
  RUN_TEST(
      Given_ButtonCycled_When_BelowDebounceThreshold_Then_TapAndPressIsFalse);
  // RUN_TEST(test_tap);

  UNITY_END();
}

#endif