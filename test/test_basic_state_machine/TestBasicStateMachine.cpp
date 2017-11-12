#ifdef UNIT_TEST

#include "BasicStateMachine.h"
#include <unity.h>

BasicStateMachine stateMachine;

void setUp(void) {
  stateMachine = BasicStateMachine();
}

void tearDown(void) {}

void test_state_machine_should_return_initial_state(void) {
  TEST_ASSERT_EQUAL(0, stateMachine.getState());
}

}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_state_machine_should_return_initial_state);

  UNITY_END();
}

#endif
