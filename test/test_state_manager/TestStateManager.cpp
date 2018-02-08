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

void test_test_one_tap_moves_to_test_two_when_tapped() {
  TEST_ASSERT_EQUAL(1, ts->getID());
  sm->tap();
  TEST_ASSERT_EQUAL(2, sm->getCurrentID());
}

void test_color_is_changed_when_state_three_is_tapped() {
  //get to StateThree
  sm->tap();
  sm->press();
  // advance color
  sm->tap();
  State *cur = sm->getColor();
  TEST_ASSERT_EQUAL(12, cur->getID());
  sm->tap();
  cur = sm->getColor();
  TEST_ASSERT_EQUAL(11, cur->getID());
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_test_state_has_id);
  RUN_TEST(test_test_one_tap_moves_to_test_two_when_tapped);
  RUN_TEST(test_color_is_changed_when_state_three_is_tapped);

  UNITY_END();
}

#endif
