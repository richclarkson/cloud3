/* 
Saber State Machine
A very simple finite state machine library
Created by Steve Faletti, 2017-10-01
Proprietary to Richard Clarkson Studio
*/

#ifndef SaberStateMachine_h
#define SaberStateMachine_h

class SaberStateMachine
{
public:
  // Add any new lighting animations to the States enum. Make sure they are placed before STATES_ENUM_LENGTH.
  enum States {
    OFF,
    FALLING_DOT,
    MIDDLE_OUT,
    RIPPLE,
    FADE,
    MUSIC_RAINBOW,
    LAMP_MODE,
    STATES_ENUM_LENGTH
  };
  SaberStateMachine();
  int getCurrent();
  void setCurrent(int newState);
  void next();

private:
  int _currentState;
};

#endif