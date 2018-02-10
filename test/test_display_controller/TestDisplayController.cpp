#ifdef UNIT_TEST

#include <unity.h>
#include "StateManager.h"
#include "States.h"

StateManager sm;

void setUp() {
  sm = StateManager(new NormalOff, new Neon);
  sm.registerDisplayController(new DisplayController);
}

int main() {
  UNITY_BEGIN();

  UNITY_END();
}

#endif
