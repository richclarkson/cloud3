/*
TapPressButton.h - A library to control the button for the Saber music lighting product
Created by Steve Faletti, 2017-09-30
Proprietary to Richard Clarkson Studio
*/

#ifndef TapPressButton_h
#define TapPressButton_h

class TapPressButton
{
public:
	TapPressButton(int pin);
	void check();
	char getPressType();
	unsigned long getPressLength();
	unsigned long tapThreshold;
	unsigned long tapLength;
	unsigned long pressThreshold;
	unsigned long pressLength;
	
private:
	char _setPressType(char pressType);
	void _clearPrevMillis();
	unsigned long _getPressMillis();
	char _pin;
	unsigned long _prevMillis;
	unsigned long _pressMillis;
	char _pressType;
};

#endif
