#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_bounce(void) {
  TapPressButton btn = TapPressButton();
  for (int i=0; i<60; i++) {
      btn.update(true, i);
      TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  }
  btn.update(false, 70);
  TEST_ASSERT_EQUAL(btn.getPressType(), 1);
  btn.update(true, 500);
  TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  btn.update(true, 551);
  TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  btn.update(false, 555);
  TEST_ASSERT_EQUAL(btn.getPressType(), 1);
}

void test_tap() {
    TapPressButton btn = TapPressButton();
    btn.update(true, 70);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_bounce);

  UNITY_END();
}

#endif