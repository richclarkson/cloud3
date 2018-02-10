#ifdef UNIT_TEST

#include <unity.h>
#include "StateManager.h"
#include "States.h"

StateManager sm;
DisplayController dc;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(&dc);
}

int main() {
  UNITY_BEGIN();

  UNITY_END();
}

#endif
