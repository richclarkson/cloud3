#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

#include <iostream>
using namespace std;

class TestState : public State {
  public:
  TestState();
  TestState(StateManager *sm);
  int testVal;
};

class TestStateOne : public TestState {
public:
  TestStateOne();
  TestStateOne(StateManager *sm);
  void tap();
  void press();
};

class TestStateTwo : public TestState {
public:
  TestStateTwo();
  TestStateTwo(StateManager *sm);
  void tap();
  void press();
};

class ColorStateOne : public TestState {
public:
  ColorStateOne();
  ColorStateOne(StateManager *sm);
  void press();
};

class ColorStateTwo : public TestState {
public:
  ColorStateTwo();
  ColorStateTwo(StateManager *sm);
  void press();
};

#endif
