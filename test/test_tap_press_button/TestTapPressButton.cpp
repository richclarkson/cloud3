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

void test_no_flag_in_debounce_time() {
  PRESS_BUTTON(3);
  RELEASE_BUTTON(10);
  PRESS_BUTTON(5);
  RELEASE_BUTTON(2);
  PRESS_BUTTON(8);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  btn.update(false, 31);
  // pretend a long time has passed
  RELEASE_BUTTON(300);
  PRESS_BUTTON(30);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_tap_between_50_and_300_timercounts() {
  PRESS_BUTTON(55);
  TEST_ASSERT_FALSE(btn.isTap());
  RELEASE_BUTTON(1);
  TEST_ASSERT_TRUE(btn.isTap());
  // pretend a long time has passed
  RELEASE_BUTTON(5000);
  PRESS_BUTTON(300);
  TEST_ASSERT_FALSE(btn.isTap());
  RELEASE_BUTTON(1);
  TEST_ASSERT_TRUE(btn.isTap());
}

void test_no_tap_before_51_timercounts() {
  PRESS_BUTTON(49);
  TEST_ASSERT_FALSE(btn.isTap());
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_tap_times_out_after_300_timercounts() {
  // btn.update(true, 0);
  // btn.update(true, 50);
  PRESS_BUTTON(350);
  TEST_ASSERT_FALSE(btn.isTap());
  // btn.update(false, 350);
  RELEASE_BUTTON(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_press_starts_after_500_timercounts() {
  btn.update(true, 0);
  btn.update(true, 501);
  TEST_ASSERT_TRUE(btn.isPress());
}

void press_does_not_start_until_after_500_timercounts() {
  btn.update(true, 0);
  btn.update(true, 500);
  TEST_ASSERT_FALSE(btn.isPress());
  // try multiple times
  btn.update(false, 10000);
  btn.update(true, 10001);
  btn.update(true, 10501);
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_press_flag_is_cleared_after_access() {
  btn.update(true, 0);
  btn.update(true, 501);
  TEST_ASSERT_TRUE(btn.isPress());
  TEST_ASSERT_FALSE(btn.isPress());
  btn.update(true, 2000);
  TEST_ASSERT_TRUE(btn.isPress());
  TEST_ASSERT_FALSE(btn.isPress());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_tap_between_50_and_300_timercounts);
  RUN_TEST(test_no_tap_before_51_timercounts);
  RUN_TEST(test_tap_times_out_after_300_timercounts);
  RUN_TEST(test_no_flag_in_debounce_time);
  RUN_TEST(test_press_starts_after_500_timercounts);
  RUN_TEST(press_does_not_start_until_after_500_timercounts);
  RUN_TEST(test_press_flag_is_cleared_after_access);

  UNITY_END();
}

#endif
