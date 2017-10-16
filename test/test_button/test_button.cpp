#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_debounce() {
  TapPressButton btn = TapPressButton();
  btn.update(false, 0);
  btn.update(true, 30);
  TEST_ASSERT_FALSE(btn.isTap());
  // TEST_ASSERT_FALSE(btn.isPress());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);

  UNITY_END();
}

#endif