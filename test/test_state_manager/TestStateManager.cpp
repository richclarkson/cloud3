#ifdef UNIT_TEST

#include "StateManager.h"
#include "TestStates.h"
#include <unity.h>

#include <iostream>
using namespace std;

StateManager *sm;
TestState *ts = new TestStateOne();

void setUp(void) {
  sm = new StateManager(new TestStateOne, new ColorStateOne);
}

void test_test_state_has_id() {
  TEST_ASSERT_EQUAL(1, ts->getID());
}

int main() {
  UNITY_BEGIN();

  UNITY_END();
}

#endif
