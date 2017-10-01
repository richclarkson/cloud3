/*
SaberButton.h - A library to control the button for the Saber music lighting product
Created by Steve Faletti, 2017-09-30
Proprietary to Richard Clarkson Studio
*/

#ifndef SaberButton_h
#define SaberButton_h

#include "Arduino.h"

class SaberButton
{
	public:
		SaberButton(int pin);
		long getPressTime();
	private:
		long _pressMillis;
		long _prevMillis;
};

#endif
