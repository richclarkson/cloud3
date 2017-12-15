#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

TapPressButton btn;
unsigned long timer_val;

#define PRESS_BUTTON_FOR(length)                                               \
  for (int i = timer_val; timer_val < i + length; timer_val++)                 \
  btn.update(true, timer_val)

#define RELEASE_BUTTON_FOR(length)                                             \
  for (int i = timer_val; timer_val < i + length; timer_val++)                 \
  btn.update(false, timer_val)

unsigned long _add_time(unsigned long timedelta) {
  timer_val += timedelta;
  return timer_val;
}

#define PRESS_BUTTON_AFTER(length) btn.update(true, _add_time(length))

#define RELEASE_BUTTON_AFTER(length) btn.update(false, _add_time(length))

void setUp() {
  btn = TapPressButton();
  timer_val = 0;
}

void test_no_tap_no_press_after_10_counts() {
  PRESS_BUTTON_FOR(10);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_no_tap_no_press_after_49_counts() {
  PRESS_BUTTON_FOR(49);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_no_tap_no_press_after_multiple_presses() {
  PRESS_BUTTON_FOR(10);
  RELEASE_BUTTON_FOR(5);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  PRESS_BUTTON_FOR(40);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  RELEASE_BUTTON_FOR(447865);
  PRESS_BUTTON_FOR(45);
  RELEASE_BUTTON_FOR(12);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_debounce() {
  test_no_tap_no_press_after_10_counts();
  test_no_tap_no_press_after_49_counts();
  test_no_tap_no_press_after_multiple_presses();
}

void test_no_tap_before_50_counts() {
  PRESS_BUTTON_FOR(49);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_tap_after_50_counts() {
  PRESS_BUTTON_FOR(50);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_TRUE(btn.isTap());
}

void test_tap_at_200_counts() {
  PRESS_BUTTON_FOR(200);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_TRUE(btn.isTap());
}

void test_tap_at_250_press_counts_and_30_count_release_delay() {
  PRESS_BUTTON_FOR(250);
  RELEASE_BUTTON_AFTER(30);
  TEST_ASSERT_TRUE(btn.isTap());
}

void test_tap_at_350_counts() {
  PRESS_BUTTON_FOR(349);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_TRUE(btn.isTap());
}

void test_no_tap_after_350_counts() {
  PRESS_BUTTON_FOR(350);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_no_press_before_500_counts() {
  PRESS_BUTTON_FOR(500);
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_press_after_500_counts() {
  PRESS_BUTTON_FOR(501);
  TEST_ASSERT_TRUE(btn.isPress());
}

void test_hold_100_check_tap_hold_500_is_press() {
  PRESS_BUTTON_FOR(100);
  btn.isTap();
  PRESS_BUTTON_FOR(500);
  TEST_ASSERT_TRUE(btn.isPress());
}

void test_hold_550_check_press_hold_550_is_press() {
  PRESS_BUTTON_FOR(550);
  btn.isPress();
  PRESS_BUTTON_FOR(550);
  TEST_ASSERT_TRUE(btn.isPress());
}

void test_press_flag_cleared_when_read() {
  PRESS_BUTTON_FOR(501);
  btn.isPress();
  PRESS_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isPress());
}

void test_hold_550_check_press_hold_350_release_not_tap() {
  PRESS_BUTTON_FOR(550);
  btn.isPress();
  PRESS_BUTTON_FOR(350);
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
}

void test_press_detected_after_multiple_holds_and_reads() {
  PRESS_BUTTON_FOR(501);
  TEST_ASSERT_TRUE(btn.isPress());
  PRESS_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isPress());
  PRESS_BUTTON_FOR(200);
  TEST_ASSERT_FALSE(btn.isPress());
  PRESS_BUTTON_FOR(350);
  TEST_ASSERT_TRUE(btn.isPress());
  PRESS_BUTTON_FOR(1000);
  TEST_ASSERT_TRUE(btn.isPress());
}

void test_tap_not_triggering_after_press() {
  PRESS_BUTTON_FOR(501);
  TEST_ASSERT_TRUE(btn.isPress());
  PRESS_BUTTON_FOR(51);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
  RELEASE_BUTTON_FOR(1);
  TEST_ASSERT_FALSE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);
  RUN_TEST(test_tap_at_200_counts);
  RUN_TEST(test_tap_at_250_press_counts_and_30_count_release_delay);
  RUN_TEST(test_no_tap_before_50_counts);
  RUN_TEST(test_tap_after_50_counts);
  RUN_TEST(test_tap_at_350_counts);
  RUN_TEST(test_no_tap_after_350_counts);
  RUN_TEST(test_no_press_before_500_counts);
  RUN_TEST(test_press_after_500_counts);
  RUN_TEST(test_hold_100_check_tap_hold_500_is_press);
  RUN_TEST(test_hold_550_check_press_hold_550_is_press);
  RUN_TEST(test_press_flag_cleared_when_read);
  RUN_TEST(test_hold_550_check_press_hold_350_release_not_tap);
  RUN_TEST(test_press_detected_after_multiple_holds_and_reads);
  RUN_TEST(test_tap_not_triggering_after_press);

  UNITY_END();
}

#endif
