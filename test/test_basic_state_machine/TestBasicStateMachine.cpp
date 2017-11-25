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

void test_state_machine_state_setter() {
  stateMachine.setState(1);
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
  stateMachine.setState(0);
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
  stateMachine.setState(-10);
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
  stateMachine.setState(50);
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
  stateMachine.setState(2);
  TEST_ASSERT_EQUAL(1, stateMachine.getState());
}

void test_state_is() {
  TEST_ASSERT_TRUE(stateMachine.is(0));
  stateMachine.next();
  TEST_ASSERT_TRUE(stateMachine.is(1));
  stateMachine.next();
  TEST_ASSERT_TRUE(stateMachine.is(0));
}

void test_state_machine_is_advanced_multiple_times() {
  stateMachine = BasicStateMachine(5);
  TEST_ASSERT_TRUE(stateMachine.is(0));
  stateMachine.next(2);
  TEST_ASSERT_TRUE(stateMachine.is(2));
  stateMachine.next();
  TEST_ASSERT_TRUE(stateMachine.is(3));
  stateMachine.next(2);
  TEST_ASSERT_TRUE(stateMachine.is(0));
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_state_machine_should_return_initial_state);
  RUN_TEST(test_state_machine_increment);
  RUN_TEST(test_state_machine_rollover_on_increment);
  RUN_TEST(test_state_machine_rollback_on_decrement);
  RUN_TEST(test_state_machine_state_setter);
  RUN_TEST(test_state_is);
  RUN_TEST(test_state_machine_is_advanced_multiple_times);

  UNITY_END();
}

#endif
