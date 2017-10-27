#ifdef UNIT_TEST

#include "TapPressButton.h"
#include <unity.h>

void testTapPressButtonExists() { TapPressButton btn; }

void testTapPressButtonInputIsTrue() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  TEST_ASSERT_TRUE(btn.getCurrentState());
}

void testTapPressButtonInputHasChanged() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(true, 50);
  TEST_ASSERT_FALSE(btn.stateHasChanged());
  btn.updateInput(false, 100);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(true, 101);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
  btn.updateInput(false, 300);
  TEST_ASSERT_TRUE(btn.stateHasChanged());
}

void testHasPressTimeElapsed() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 100);
  TEST_ASSERT_TRUE(btn.getPressTime() > 0);
}

void testPressInTapWindow() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 50);
  TEST_ASSERT_FALSE(btn.isPressInTapWindow());
  btn.updateInput(true, 51);
  TEST_ASSERT_TRUE(btn.isPressInTapWindow());
  btn.updateInput(true, 600);
  TEST_ASSERT_FALSE(btn.isPressInTapWindow());
}

void testPressInPressWindow() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 500);
  TEST_ASSERT_FALSE(btn.isPressInPressWindow());
  btn.updateInput(true, 501);
  TEST_ASSERT_TRUE(btn.isPressInPressWindow());
  btn.updateInput(true, 1500);
  TEST_ASSERT_TRUE(btn.isPressInPressWindow());
}

void testButtonTapped() {
  TapPressButton btn;
  btn.updateInput(true, 0);
  btn.updateInput(true, 60);
  TEST_ASSERT_FALSE(btn.isTap());
  btn.updateInput(false, 70);
  TEST_ASSERT_TRUE(btn.isTap());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  RUN_TEST(testTapPressButtonExists);
  RUN_TEST(testTapPressButtonInputIsTrue);
  RUN_TEST(testTapPressButtonInputHasChanged);
  RUN_TEST(testHasPressTimeElapsed);
  RUN_TEST(testPressInTapWindow);
  RUN_TEST(testPressInPressWindow);
  RUN_TEST(testButtonTapped);

  UNITY_END();
}

#endif
