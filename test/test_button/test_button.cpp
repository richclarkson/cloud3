#ifdef UNIT_TEST

#include <TapPressButton.h>
#include <unity.h>

void test_debounce() {
  int testTimes[8] = {0,20,50,90,1000,1049,50000,50030};
  int testTimesLen = 8;
  bool btnSwitch = true;
  TapPressButton btn = TapPressButton();
  for (int i=testTimes[0]; i<testTimesLen; i++) {
    btn.update(btnSwitch, testTimes[i]);
    btn.update(!btnSwitch, testTimes[i]);
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
  }
}

void test_tap() {
  TapPressButton btn = TapPressButton();
  for (int i=0; i<50; i++) {
    btn.update(true, i);
    TEST_ASSERT_FALSE(btn.isTap());
    TEST_ASSERT_FALSE(btn.isPress());
  }
  btn.update(false, 51);
  TEST_ASSERT_TRUE(btn.isTap());
  TEST_ASSERT_FALSE(btn.isPress());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(test_debounce);
  RUN_TEST(test_tap);

  UNITY_END();
}

#endif