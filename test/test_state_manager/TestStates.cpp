#include "TestStates.h"
#include <iostream>
using namespace std;

int TestState::getID() {
  return id;
}

TestState1::TestState1() { 
  cout << "\nTest State 1 created\n"; 
  this->id = 1;
}

TestState1::~TestState1() { 
  cout << "\nTest State 1 destroyed\n"; 
}

void TestState1::tap(StateManager *sm) {
  cout << "1 tapped: " << sm << "\n";
  // TestState2* ts = new TestState2;
  delete this;
  sm->setCurrent(new TestState2());
}

TestState2::TestState2() { 
  cout << "\nTest State 2 created\n"; 
  this->id = 2;
}

TestState2::~TestState2() { 
  cout << "\nTest State 2 destroyed\n"; 
}

void TestState2::tap(StateManager *sm) {
  cout << "2 tapped: " << sm << "\n";
  delete this;
  sm->setCurrent(new TestState1());
}
