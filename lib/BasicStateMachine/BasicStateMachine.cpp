#include "BasicStateMachine.h"

BasicStateMachine::BasicStateMachine() {}

BasicStateMachine::BasicStateMachine(int _stateCount) {
  currentState = 0;
  stateCount = _stateCount;
}

int BasicStateMachine::getState() { return currentState; }

void BasicStateMachine::next() {
  currentState++;
  if (currentState >= stateCount) {
    currentState = 0;
  }
}

void BasicStateMachine::prev() {
  currentState--;
  if (currentState < 0) {
    currentState = stateCount - 1;
  }
}

void BasicStateMachine::setState(int newState) {
  if (newState < 0) {
    newState = 0;
  } else if (newState >= stateCount) {
    newState = stateCount - 1;
  }
  currentState = newState;
}

