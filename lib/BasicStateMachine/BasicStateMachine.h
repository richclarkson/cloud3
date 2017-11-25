#ifndef BasicStateMachine_h
#define BasicStateMachine_h

class BasicStateMachine {
public:
  BasicStateMachine();
  BasicStateMachine(int _stateCount);
  int getState();
  bool is(int checkState);
  void setState(int newState);
  void next();
  void prev();
  
private:
  int currentState;
  int stateCount;
};

#endif