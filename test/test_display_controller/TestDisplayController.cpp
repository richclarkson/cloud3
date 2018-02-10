#ifdef UNIT_TEST

#include "StateManager.h"
#include "States.h"
#include <unity.h>

class TestDisplayController : public DisplayController {
  int testVal;

public:
  TestDisplayController() { testVal = 0; }
  int getTestVal() { return testVal; }
  void turnOff() { testVal = 10; }
  void displayFallingDot() { testVal = 11; }
  void displayMiddleOut() { testVal = 12; }
  void displayRipple() { testVal = 13; }
  void displayBangAndFade() { testVal = 14; }
  void displayRainbow() { testVal = 15; }
  void displayColorPreview() { testVal = 16; }
};

StateManager sm;
TestDisplayController tdc;
unsigned long timerVal;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(&tdc);
}

void test_display_can_be_called_from_state_when_manager_updated() {
  timerVal = 0;
  sm.update(timerVal);
  TEST_ASSERT_EQUAL(10, tdc.getTestVal());
  sm.tap();
  sm.update(timerVal);
  TEST_ASSERT_EQUAL(11, tdc.getTestVal());
}

void test_states_call_displays_as_expected() {
  sm.update();
  TEST_ASSERT_EQUAL(10, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(11, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(12, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(13, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(14, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(15, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(16, tdc.getTestVal());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(10, tdc.getTestVal());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_display_can_be_called_from_state_when_manager_updated);
  RUN_TEST(test_states_call_displays_as_expected);

  UNITY_END();
}

#endif
