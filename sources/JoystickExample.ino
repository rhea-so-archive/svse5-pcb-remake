// https://github.com/MHeironimus/ArduinoJoystickLibrary
#include "Joystick.h"

Joystick_ Joystick;

void setup()
{
	Joystick.begin();
}

void loop()
{
	Joystick.pressButton(1);
	delay(2000);
	Joystick.releaseButton(1);
	delay(2000);
	Joystick.pressButton(2);
	delay(2000);
	Joystick.releaseButton(2);
}
