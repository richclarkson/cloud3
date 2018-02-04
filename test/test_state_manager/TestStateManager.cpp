#ifdef UNIT_TEST

#include "StateManager.h"
#include "TestStates.h"
#include <unity.h>

class TestStateManager : public StateManager {
  TestState *current;

public:
  TestStateManager(TestState *starting) { current = starting; }
  void tap() { current->tap(this); }
  int getCurrentID() { return current->getID(); }
};

TestStateManager *sm;
TestState1 *ts = new TestState1();

void setUp(void) { sm = new TestStateManager(new TestState1()); }

void test_test_state_id_exists() { TEST_ASSERT_EQUAL(1, ts->getID()); }

void test_starting_state_val_is_1() {
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
}

void test_tap_moves_state() {
  sm->tap();
  TEST_ASSERT_EQUAL(2, sm->getCurrentID());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_test_state_id_exists);
  RUN_TEST(test_starting_state_val_is_1);
  RUN_TEST(test_tap_moves_state);

  UNITY_END();
}

#endif
