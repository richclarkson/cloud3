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
  sm = StateManager(ts, cs); 
}

void test_state_value_exists() {
  TEST_ASSERT_EQUAL(1, ts->testVal);
}

void test_state_advances_on_tap() {
  sm.tap();
  TEST_ASSERT_EQUAL(2, sm.getCurrent()->testVal);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_state_value_exists);
  RUN_TEST(test_state_advances_on_tap);

  UNITY_END();
}

#endif
