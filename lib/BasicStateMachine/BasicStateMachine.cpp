#include "BasicStateMachine.h"

BasicStateMachine::BasicStateMachine() {}

BasicStateMachine::BasicStateMachine(int _stateCount) {
  currentState = 0;
  stateCount = _stateCount;
}

int BasicStateMachine::getState() { return currentState; }

bool BasicStateMachine::is(int checkState) { return getState() == checkState; }

void BasicStateMachine::next() {
  currentState++;
  if (currentState >= stateCount) {
    currentState = 0;
  }
}

void BasicStateMachine::next(int interval) {
  for (int i = 0; i < interval; i++) {
    next();
  }
}

void BasicStateMachine::prev() {
  currentState--;
  if (currentState < 0) {
    currentState = stateCount - 1;
  }
}

void BasicStateMachine::setState(int newState) {
  currentState = 0;
  if (newState < 0) {newState = 0;}
  for (int i = 0; i < newState; i++) {
    next();
  }
}
