// Reference: https://github.com/MHeironimus/ArduinoJoystickLibrary
#include "Joystick.h"

// 가상 조이스틱 생성
Joystick_ Joystick;

struct GameController
{
	void init()
	{
		Joystick.begin();
	}

	void pressButton(int id)
	{
		Joystick.pressButton(id);
	}

	void releaseButton(int id)
	{
		Joystick.releaseButton(id);
	}
};

GameController gameController; // Can use this struct like Singleton Class. lol
