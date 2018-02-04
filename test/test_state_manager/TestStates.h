#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

class TestState : public State {
public:
  int id;
  int getID();
};

class TestState1 : public TestState {
public:
  TestState1();
  ~TestState1();
  void tap(StateManager *sm);
};

class TestState2 : public TestState {
public:
  TestState2();
  ~TestState2();
  void tap(StateManager *sm);
};

#endif
