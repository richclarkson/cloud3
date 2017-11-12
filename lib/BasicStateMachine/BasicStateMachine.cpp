#include "BasicStateMachine.h"

BasicStateMachine::BasicStateMachine() {
}

BasicStateMachine::BasicStateMachine(int _stateCount) {
    currentState = 0;
    stateCount = _stateCount;
}

int BasicStateMachine::getState() {
    return currentState;
}

void BasicStateMachine::next() {
    currentState ++;
}

void BasicStateMachine::prev() {
    currentState --;
}