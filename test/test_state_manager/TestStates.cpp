#include "TestStates.h"
#include <iostream>
using namespace std;

int TestState::getTestVal() {
  return testVal;
}

TestState1::TestState1() { 
  cout << "\nTest State 1 created\n"; 
  this->testVal = 1;
}

TestState1::~TestState1() { 
  cout << "\nTest State 1 destroyed\n"; 
}

void TestState1::tap(StateManager *sm) {
  sm->setCurrent(new TestState2());
  delete this;
}

TestState2::TestState2() { 
  cout << "\nTest State 2 created\n"; 
  this->testVal = 2;
}

TestState2::~TestState2() { 
  cout << "\nTest State 2 destroyed\n"; 
}

void TestState2::tap(StateManager *sm) {
  sm->setCurrent(new TestState1());
  delete this;
}