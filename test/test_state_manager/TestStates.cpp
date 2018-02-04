#include "TestStates.h"
#include <iostream>
using namespace std;

int TestState::getID() {
  return id;
}

TestStateOne::TestStateOne() { 
  cout << "\nTest State 1 created\n"; 
  this->id = 1;
}

TestStateOne::~TestStateOne() { 
  cout << "\nTest State 1 destroyed\n"; 
}

void TestStateOne::tap(StateManager *sm) {
  cout << "1 tapped: " << sm << "\n";
  // TestStateTwo* ts = new TestStateTwo;
  delete this;
  sm->setCurrent(new TestStateTwo());
}

TestStateTwo::TestStateTwo() { 
  cout << "\nTest State 2 created\n"; 
  this->id = 2;
}

TestStateTwo::~TestStateTwo() { 
  cout << "\nTest State 2 destroyed\n"; 
}

void TestStateTwo::tap(StateManager *sm) {
  cout << "2 tapped: " << sm << "\n";
  delete this;
  sm->setCurrent(new TestStateOne());
}

void TestStateTwo::press(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestStateThree());
}

TestStateThree::TestStateThree() {
  this->id = 3;
}

void TestStateThree::press(StateManager *sm) {
  delete this;
  sm->setCurrent(new TestStateOne());
}