#include "BasicStateMachine.h"

BasicStateMachine::BasicStateMachine() {
    currentState = 0;
}

int BasicStateMachine::getState() {
    return currentState;
}

void BasicStateMachine::next() {
    currentState ++;
}
