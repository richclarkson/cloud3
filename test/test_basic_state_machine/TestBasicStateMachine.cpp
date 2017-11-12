#ifdef UNIT_TEST

#include "BasicStateMachine.h"
#include <unity.h>

BasicStateMachine stateMachine;

void setUp(void) { stateMachine = BasicStateMachine(2); }

void tearDown(void) {}

void test_state_machine_should_return_initial_state(void) {
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
}

void test_state_machine_increment(void) {
  stateMachine.next();
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
}

void test_state_machine_decrement() {
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
  stateMachine.prev();
  TEST_ASSERT_EQUAL(-1, stateMachine.getState());
}

void test_state_machine_rollover_on_increment() {
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
  stateMachine.next();
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
  stateMachine.next();
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
}

void test_state_machine_rollback_on_decrement() {
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
  stateMachine.prev();
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_state_machine_should_return_initial_state);
  RUN_TEST(test_state_machine_increment);
  RUN_TEST(test_state_machine_decrement);
  RUN_TEST(test_state_machine_rollover_on_increment);
  RUN_TEST(test_state_machine_rollback_on_decrement);

  UNITY_END();
}

#endif
