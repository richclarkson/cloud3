#ifdef UNIT_TEST

#include "StateManager.h"
#include "TestStates.h"
#include "States.h"
#include <unity.h>

#include <typeinfo>
#include <iostream>
using namespace std;


StateManager sm;

void setUp(void) { 
  sm = StateManager(new TestOne, new ColorOne);

}

void test_state_value_exists() {
  TEST_ASSERT_EQUAL(1, sm.getCurrent()->testVal);
}

void test_state_advances_on_tap() {
  sm.tap();
  TEST_ASSERT_EQUAL(2, sm.getCurrent()->testVal);
  sm.tap();
  TEST_ASSERT_EQUAL(1, sm.getCurrent()->testVal);
  sm.tap();
  TEST_ASSERT_EQUAL(2, sm.getCurrent()->testVal);
}

void test_color_exists() {
  TEST_ASSERT_EQUAL(10, sm.getColor()->testVal);
}

void test_color_advances_when_two_is_pressed() {
  sm.tap(); //get to TestTwo
  sm.press();
  TEST_ASSERT_EQUAL(20, sm.getColor()->testVal);
  sm.press();
  TEST_ASSERT_EQUAL(10, sm.getColor()->testVal);
}

int main() {
  UNITY_BEGIN();

  RUN_TEST(test_state_value_exists);
  RUN_TEST(test_state_advances_on_tap);
  RUN_TEST(test_color_exists);
  RUN_TEST(test_color_advances_when_two_is_pressed);

  UNITY_END();
}

#endif
