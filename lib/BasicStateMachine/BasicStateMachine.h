#ifndef BasicStateMachine_h
#define BasicStateMachine_h

class BasicStateMachine {
public:
  BasicStateMachine();
  int getState();
  void next();
  void prev();
  
private:
  int currentState;
};

#endif