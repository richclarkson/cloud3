#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_debounce() {
  TapPressButton btn = TapPressButton();
  btn.setDebounce(50);
  for (int i = 0; i < 200; i++) {
    if (i % 30 == 0) {
      btn.update(false, i);
    } else {
      btn.update(true, i);
    }
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
  }
}

void test_tap() {
  TapPressButton btn = TapPressButton();
  btn.setDebounce(20);
  for (int i = 0; i < 1000; i++) {
    if (i % 30 == 0) {
      btn.update(false, i);
      TEST_ASSERT_TRUE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
    } else {
      btn.update(true, i);
      TEST_ASSERT_FALSE(btn.isTap());
      TEST_ASSERT_FALSE(btn.isPress());
    }
  }
}

void test_press() {
  TapPressButton btn = TapPressButton();
  for (int i=0; i<1000; i++) {
    btn.update(true, i);
    if (i > 500) {
      TEST_ASSERT_TRUE(btn.isPress());
    }
    else TEST_ASSERT_FALSE(btn.isPress());
  }
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);
  RUN_TEST(test_tap);
  RUN_TEST(test_press);

  UNITY_END();
}

#endif