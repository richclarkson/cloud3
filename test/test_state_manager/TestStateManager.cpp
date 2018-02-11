#ifdef UNIT_TEST

#include "StateManager.h"
#include "TestStates.h"
#include <unity.h>

#include <typeinfo>
#include <iostream>
using namespace std;


StateManager sm;
TestStateOne *ts = new TestStateOne();
ColorStateOne *cs = new ColorStateOne();

void setUp(void) { 
  sm = StateManager(new TestStateOne, new ColorStateOne); 
}

void test_state_value_exists() {
  // TEST_ASSERT_EQUAL(1, ts->testVal);
}

void test_state_advances_on_tap() {
  // sm.tap();
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_state_value_exists);
  RUN_TEST(test_state_advances_on_tap);

  UNITY_END();
}

#endif
