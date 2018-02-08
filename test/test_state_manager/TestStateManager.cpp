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

void test_one_moves_to_two_when_tapped() {
  TEST_ASSERT_EQUAL(1, ts->getID());
  sm->tap();
  TEST_ASSERT_EQUAL(2, sm->getCurrentID());
}

void test_two_moves_to_one_when_tapped() {
  sm->tap();
  sm->tap();
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
}

void test_two_moves_to_three_when_pressed() {
  sm->tap();
  sm->press();
  TEST_ASSERT_EQUAL(3, sm->getCurrentID());
}

void test_three_moves_to_one_when_pressed() {
  sm->tap();
  sm->press();
  sm->press();
  TEST_ASSERT_EQUAL(1, sm->getCurrentID());
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
  RUN_TEST(test_one_moves_to_two_when_tapped);
  RUN_TEST(test_two_moves_to_one_when_tapped);
  RUN_TEST(test_two_moves_to_three_when_pressed);
  RUN_TEST(test_three_moves_to_one_when_pressed);
  RUN_TEST(test_color_is_changed_when_state_three_is_tapped);

  UNITY_END();
}

#endif
