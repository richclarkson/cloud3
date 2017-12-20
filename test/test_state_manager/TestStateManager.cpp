#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>

StateManager sm;

void setUp(void) {
  sm = StateManager();
}

void tearDown(void) {}

void test_machine_can_handle_taps() {
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.tap();
    sm.press();
    sm.press();
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_machine_can_handle_taps);

  UNITY_END();
}

#endif
