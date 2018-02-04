#ifdef UNIT_TEST

#include "StateManager.h"
#include "TestStates.h"
#include <unity.h>

#include <iostream>
using namespace std;

class TestStateManager : public StateManager {
  TestState *current;

public:
  TestStateManager(TestState *starting) { setCurrent(starting); }
  void tap() { current->tap(this); }
  void setCurrent(TestState *s) { current = s; }
  int getCurrentID() { return current->getID(); }
};

TestStateManager *sm;
TestState *ts = new TestState1();

void setUp(void) {
  cout << "\n===setup====";
  sm = new TestStateManager(new TestState1);
  sm->tap();
  cout << "====end setup====\n";
}

void test_test_state_id_exists() { TEST_ASSERT_EQUAL(1, ts->getID()); }

void test_starting_state_val_is_1() {
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
}

  cout << "\n===================\n";
  cout << "initial: " << sm << "\n\n";
void test_tap_moves_state_from_one_to_two() {
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
  sm->tap();
  TEST_ASSERT_EQUAL(2, sm->getCurrentID());
}

void test_tap_moves_state_from_two_to_one() {
  sm->tap();
  TEST_ASSERT_EQUAL(2, sm->getCurrentID());
  sm->tap();
  cout << "test done\n";
  cout << "===================\n\n";
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_test_state_id_exists);
  RUN_TEST(test_starting_state_val_is_1);
  RUN_TEST(test_tap_moves_state_from_one_to_two);
  RUN_TEST(test_tap_moves_state_from_two_to_one);

  UNITY_END();
}

#endif
