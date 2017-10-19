#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_debounce() {
  const int debounceVal = 50;
  TapPressButton btn = TapPressButton();
  btn.setDebounce(debounceVal);
  float counter = 0;
  float timerVal = 0;
  for (float factor=.25; factor<=1.0; factor+=.25) {
    while (timerVal - counter < debounceVal * factor) {
      btn.update(true, timerVal);
      TEST_ASSERT_FALSE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
      timerVal ++;
    }
    btn.update(false, timerVal);
    if (factor < 1) {
      TEST_ASSERT_FALSE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
    }
    counter += timerVal;
    timerVal ++;
  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);

  UNITY_END();
}

#endif