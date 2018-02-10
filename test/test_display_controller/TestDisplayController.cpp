#ifdef UNIT_TEST

#include "StateManager.h"
#include "States.h"
#include <unity.h>

class TestDisplayController : public DisplayController {
  int testVal;

public:
  TestDisplayController() { testVal = 0; }
  void setTestVal(int val) { testVal = val; }
  int getTestVal() { return testVal; }
  void turnOff() { testVal = 10; }
  void fallingDot() { testVal = 11; }
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

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_display_can_be_called_from_state_when_manager_updated);

  UNITY_END();
}

#endif
