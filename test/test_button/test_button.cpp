#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

#include <iostream>

void test_debounce() {
  const int debounceVal = 50;
  TapPressButton btn = TapPressButton();
  btn.setDebounce(debounceVal);
  float prevCounter = 0;
  float counter = 0;
  for (float factor=.25; factor<=1.0; factor+=.25) {
    while (counter - prevCounter < debounceVal * factor) {
      std::cout << counter;
      std::cout << '\n';
      btn.update(true, counter);
      TEST_ASSERT_FALSE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
      counter ++;
    }
    btn.update(false, counter);
    if (factor < 1) {
      TEST_ASSERT_FALSE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
    }
    prevCounter += counter;
    counter ++;
  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);

  UNITY_END();
}

#endif