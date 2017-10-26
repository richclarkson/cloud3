#include "TapPressButton.h"

    void TapPressButton::updateInput(bool input) {
        prevButtonState = currentButtonState;
        currentButtonState = input;
    }
    
    bool TapPressButton::getCurrentState() {
        return currentButtonState;
    }
    
    bool TapPressButton::hasChanged() {
        return currentButtonState != prevButtonState;
    }
