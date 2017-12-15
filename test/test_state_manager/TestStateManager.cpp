#ifdef UNIT_TEST

#include "StateManager.h"
#include <unity.h>

StateManager sm;
Event testEvt1, testEvt2;

void setUp(void) {
  sm = StateManager();
  testEvt1 = Event();
  testEvt2 = Event();
}

void tearDown(void) {}

int main() {
  UNITY_BEGIN();

  UNITY_END();
}

#endif
