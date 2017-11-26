#ifdef UNIT_TEST

#include "SaberStateHandler.h"
#include <unity.h>

SaberStateHandler stateHandler;

void setUp() { stateHandler = SaberStateHandler(); }

void test_cycle_through_normal_modes() {
  int testVal;
  int modeCount = 7;
  for (int i = 0; i < 10; i++) {
    testVal = i;
    if (testVal >= modeCount) {
      testVal -= modeCount;
    }
    TEST_ASSERT_EQUAL(testVal, stateHandler.getNormalMode());
    stateHandler.tap();
  }
}

void test_cycle_through_main_modes() {
  TEST_ASSERT_EQUAL(0, stateHandler.getMainState());
  stateHandler.press();
  TEST_ASSERT_EQUAL(1, stateHandler.getMainState());
  stateHandler.press();
  TEST_ASSERT_EQUAL(0, stateHandler.getMainState());
}

void test_switch_to_setting_mode_and_cycle_through() {
  stateHandler.press();
  int testVal;
  int modeCount = 5;
  for (int i=0; i<7; i++) {
    testVal = i;
    if (testVal >= modeCount) {
      testVal -= modeCount;
    }
    TEST_ASSERT_EQUAL(testVal, stateHandler.getSettingMode());
    stateHandler.tap();
  }
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_cycle_through_normal_modes);
  RUN_TEST(test_cycle_through_main_modes);
  RUN_TEST(test_switch_to_setting_mode_and_cycle_through);

  UNITY_END();
}

#endif
