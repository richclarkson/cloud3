#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_tap() {
  TapPressButton btn = TapPressButton();
  btn.update(false, 10);
  btn.update(true, 30);
  TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  btn.update(false, 100);
  btn.update(true, 130);
  TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  btn.update(true, 200);
  TEST_ASSERT_EQUAL(btn.getPressType(), 0);
  btn.update(false, 201);
  TEST_ASSERT_EQUAL(btn.getPressType(), 1);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_tap);

  UNITY_END();
}

#endif