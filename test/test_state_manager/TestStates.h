#ifndef TestStates_h
#define TestStates_h

#include "StateManager.h"

class TestState : public State {
public:
  int id;
  int getID();
};

class TestStateOne : public TestState {
public:
  TestStateOne();
  ~TestStateOne();
  void tap(StateManager *sm);
};

class TestStateTwo : public TestState {
public:
  TestStateTwo();
  ~TestStateTwo();
  void tap(StateManager *sm);
  void press(StateManager *sm);
};

class TestStateThree : public TestState {
  public:
  TestStateThree();
  void press(StateManager *sm);
};

#endif
