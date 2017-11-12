#ifndef BasicStateMachine_h
#define BasicStateMachine_h

class BasicStateMachine {
public:
  BasicStateMachine();
  int getState();

private:
  int currentState;
};

#endif