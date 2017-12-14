#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

TapPressButton btn;
unsigned long timer_val;

#define PRESS_BUTTON(length)                                                   \
  for (int i = timer_val; timer_val < i + length; timer_val++)                 \
  btn.update(true, timer_val)

#define RELEASE_BUTTON(length)                                                 \
  for (int i = timer_val; timer_val < i + length; timer_val++)                 \
  btn.update(false, timer_val)

void setUp() {
  btn = TapPressButton();
  timer_val = 0;
}

void test_no_tap_no_press_after_10_counts() {
  PRESS_BUTTON(10);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_no_tap_no_press_after_49_counts() {
  PRESS_BUTTON(49);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_no_tap_no_press_after_multiple_presses() {
  PRESS_BUTTON(10);
  RELEASE_BUTTON(5);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  PRESS_BUTTON(40);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  RELEASE_BUTTON(447865);
  PRESS_BUTTON(45);
  RELEASE_BUTTON(12);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_debounce() {
  test_no_tap_no_press_after_10_counts();
  test_no_tap_no_press_after_49_counts();
  test_no_tap_no_press_after_multiple_presses();
}

void test_no_tap_before_51_counts() {
  PRESS_BUTTON(50);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_tap_after_50_counts() {
  PRESS_BUTTON(51);
  RELEASE_BUTTON(1);
  TEST_ASSERT_TRUE(btn.isTap());
}

  TEST_ASSERT_FALSE(btn.isPress());
}

  TEST_ASSERT_TRUE(btn.isPress());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);
  RUN_TEST(test_no_tap_before_51_counts);
  RUN_TEST(test_tap_after_50_counts);

  UNITY_END();
}

#endif
