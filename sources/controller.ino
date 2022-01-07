// Reference: https://github.com/MHeironimus/ArduinoJoystickLibrary
#include "Joystick.h"

// 로터리 엔코더
#define VOLUME_LEFT_A A5
#define VOLUME_LEFT_B A4
#define VOLUME_RIGHT_A A3
#define VOLUME_RIGHT_B A2

// 버튼
#define BUTTON_START 2
#define BUTTON_A 3
#define BUTTON_B 4
#define BUTTON_C 5
#define BUTTON_D 6
#define FX_A 7
#define FX_B 8

// 가상 조이스틱 생성
Joystick_ Joystick;

// 로터리 엔코더 신호 변환 용도 변수들
int volumeLeftOldA = 0;
int volumeLeftOldB = 0;
unsigned long volumeLeftOldTime;
int volumeLeft = 0;
int volumeLeftEnablePeriod = 20;
unsigned long volumeLeftTime;

int volumeRightOldA = 0;
int volumeRightOldB = 0;
unsigned long volumeRightOldTime;
int volumeRight = 0;
int volumeRightEnablePeriod = 20;
unsigned long volumeRightTime;

// 버튼 신호 변환 용도 변수들
int buttonSignalOlds[7] = {0, 0, 0, 0, 0, 0, 0};
int startButtonEnablePeriod = 30;
unsigned long startButtonTime;

// 기판을 활성화하여 신호를 받을 수 있도록 합니다.
void setup()
{
	pinMode(VOLUME_LEFT_A, INPUT);
	pinMode(VOLUME_LEFT_B, INPUT);
	pinMode(VOLUME_RIGHT_A, INPUT);
	pinMode(VOLUME_RIGHT_B, INPUT);
	pinMode(BUTTON_START, INPUT);
	pinMode(BUTTON_A, INPUT);
	pinMode(BUTTON_B, INPUT);
	pinMode(BUTTON_C, INPUT);
	pinMode(BUTTON_D, INPUT);
	pinMode(FX_A, INPUT);
	pinMode(FX_B, INPUT);
	Joystick.begin();
}

// 버튼과 로터리 엔코더 신호를 계속해서 읽어들이며, 이를 가상 조이스틱 신호로 변환합니다.
void loop()
{
	enableStartButton(BUTTON_START, 0, 0);
	enableButton(BUTTON_A, 1, 1);
	enableButton(BUTTON_B, 2, 2);
	enableButton(BUTTON_C, 3, 3);
	enableButton(BUTTON_D, 4, 4);
	enableButton(FX_A, 5, 5);
	enableButton(FX_B, 6, 6);
	enableRotaryEncoder(VOLUME_LEFT_A, VOLUME_LEFT_B, 7, 8, &volumeLeftOldA, &volumeLeftOldB, &volumeLeftOldTime, &volumeLeft, &volumeLeftTime, volumeLeftEnablePeriod);
	enableRotaryEncoder(VOLUME_RIGHT_A, VOLUME_RIGHT_B, 9, 10, &volumeRightOldA, &volumeRightOldB, &volumeRightOldTime, &volumeRight, &volumeRightTime, volumeRightEnablePeriod);
}

// Start Button의 입력을 받습니다.
// BT, FX와는 다르게 개별 함수를 만든 이유: StartButton에 사용한 마이크로스위치가 고장난 모양인지, 대기 상태가 1이나 0이 아니라 펄스마냥 01010101 반복하고 있어서 이를 디바운싱 시켜주려고 분리함.
void enableStartButton(int pin, int joystickButtonId, int signalId)
{
	int signal = digitalRead(pin);
	if (signal == 0)
	{
		if (millis() - startButtonTime >= startButtonEnablePeriod)
		{
			if (buttonSignalOlds[signalId] == 1)
			{
				Joystick.pressButton(joystickButtonId);
				buttonSignalOlds[signalId] = 0;
			}
		}
	}
	else if (signal == 1)
	{
		if (buttonSignalOlds[signalId] == 0)
		{
			Joystick.releaseButton(joystickButtonId);
			buttonSignalOlds[signalId] = 1;
		}
		startButtonTime = millis();
	}
}

// BT, FX의 입력을 받습니다.
// 노이즈가 없어서 디바운싱 처리는 안했습니다.
void enableButton(int pin, int joystickButtonId, int signalId)
{
	int signal = digitalRead(pin);
	if (signal == 0)
	{
		if (buttonSignalOlds[signalId] == 1)
		{
			Joystick.pressButton(joystickButtonId);
		}
	}
	else if (signal == 1)
	{
		if (buttonSignalOlds[signalId] == 0)
		{
			Joystick.releaseButton(joystickButtonId);
		}
	}
	buttonSignalOlds[signalId] = signal;
}

// 로터리 엔코더 신호를 디바운싱 합니다.
void enableRotaryEncoder(int pinA, int pinB, int joystickButtonIdA, int joystickButtonIdB, int *volumeOldA, int *volumeOldB, unsigned long *volumeOldTime, int *volumeValue, unsigned long *volumeTime, int volumeEnablePeriod)
{
	int result = getRotaryEncoderResult(pinA, pinB, volumeOldA, volumeOldB, volumeOldTime);

	if (result == 0)
	{
		if (millis() - *volumeTime >= volumeEnablePeriod)
		{
			if (*volumeValue != 0)
			{
				Joystick.releaseButton(joystickButtonIdA);
				Joystick.releaseButton(joystickButtonIdB);
				*volumeValue = 0;
			}
		}
	}
	else
	{
		if (result != *volumeValue)
		{
			Joystick.releaseButton(joystickButtonIdA);
			Joystick.releaseButton(joystickButtonIdB);
			if (result == 1)
			{
				Joystick.pressButton(joystickButtonIdA);
			}
			else if (result == -1)
			{
				Joystick.pressButton(joystickButtonIdB);
			}
			*volumeValue = result;
		}
		*volumeTime = millis();
	}
}

// 로터리 엔코더 신호를 받습니다.
int getRotaryEncoderResult(int pinA, int pinB, int *volumeOldA, int *volumeOldB, unsigned long *volumeTime)
{
	int result = 0;
	int signalA = digitalRead(pinA);
	int signalB = digitalRead(pinB);

	if (*volumeOldA == 0 && *volumeOldB == 0)
	{
		if (signalA == 1 && *volumeOldB == 0)
		{
			result = 1;
			*volumeTime = millis();
		}
		else if (signalA == 0 && *volumeOldB == 1)
		{
			result = -1;
			*volumeTime = millis();
		}
		else
		{
			if (millis() - *volumeTime > 100)
			{
				result = 0;
			}
		}
	}
	else if (*volumeOldA == 1 && *volumeOldB == 0)
	{
		if (signalA == 1 && signalB == 1)
		{
			result = 1;
			*volumeTime = millis();
		}
		else if (signalA == 0 && signalB == 0)
		{
			result = -1;
			*volumeTime = millis();
		}
		else
		{
			if (millis() - *volumeTime > 100)
			{
				result = 0;
			}
		}
	}
	else if (*volumeOldA == 1 && *volumeOldB == 1)
	{
		if (signalA == 0 && signalB == 1)
		{
			result = 1;
			*volumeTime = millis();
		}
		else if (signalA == 1 && signalB == 0)
		{
			result = -1;
			*volumeTime = millis();
		}
		else
		{
			if (millis() - *volumeTime > 100)
			{
				result = 0;
			}
		}
	}
	else if (*volumeOldA == 0 && *volumeOldB == 1)
	{
		if (signalA == 1 && signalB == 1)
		{
			result = -1;
			*volumeTime = millis();
		}
		else if (signalA == 0 && *volumeOldB == 0)
		{
			result = 1;
			*volumeTime = millis();
		}
		else
		{
			if (millis() - *volumeTime > 100)
			{
				result = 0;
			}
		}
	}
	*volumeOldA = signalA;
	*volumeOldB = signalB;

	return result;
}
