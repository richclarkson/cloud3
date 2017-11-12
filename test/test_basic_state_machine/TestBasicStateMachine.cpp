#ifdef UNIT_TEST

#include "BasicStateMachine.h"
#include <unity.h>

BasicStateMachine stateMachine;

void setUp(void) {
}

void tearDown(void) {

}

void test_state_machine_should_return_state(void) {
    TEST_ASSERT_EQUAL(0, stateMachine.getState());
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_state_machine_should_return_state);

  UNITY_END();
}

#endif
