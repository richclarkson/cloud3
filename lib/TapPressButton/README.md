# TapPressButton
Author: Steve Faletti
Date: 2017.10.03
Version: 1.0

## Description
This library provides a basic button object that can detect both taps and presses from a digital input pin.

## Usage
1. `TapPressButton myBtn = TapPressButton(pin)` to initialize TapPressButton(int Pin), declaring the intended input pin.
2. `myBtn.check()` to update the button. This should be called once per loop.
3. `char pressType = myBtn.getPressType()` returns the press type flag from the button. 0 = no press, 1 = tap, 2 = press. Taps are recorded on button release and presses are recorded while the button is being pressed. Calling this will also reset the internal press tpye flag to 0.
4. `myBtn.getPressLength()` returns the length of the press. Also resets the internal press length storage to 0. This should be used in conjunction with the `getPressType()` method.