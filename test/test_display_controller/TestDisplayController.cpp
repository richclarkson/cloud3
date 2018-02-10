#ifdef UNIT_TEST

#include <unity.h>
#include "StateManager.h"
#include "States.h"

StateManager sm;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(new DisplayController);
}

void test_display_can_be_called_from_state_when_manager_updated() {
  sm.update();
  TEST_ASSERT_EQUAL(10, sm.getDisplayTest());
  sm.tap();
  sm.update();
  TEST_ASSERT_EQUAL(20, sm.getDisplayTest());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_display_can_be_called_from_state_when_manager_updated);

  UNITY_END();
}

#endif
