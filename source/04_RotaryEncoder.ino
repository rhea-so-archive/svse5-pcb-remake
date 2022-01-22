struct RotaryEncoder
{
	int pinLeft;
	int pinRight;
	int oldValue = 0;
	int oldLeft = 0;
	int oldRight = 0;
	int buttonIdLeft;
	int buttonIdRight;
	int debouncingPeriod;
	unsigned long ms = millis();

	void init(int requestPinLeft, int requestButtonIdLeft, int requestPinRight, int requestButtonIdRight, int requestDebouncingPeriod)
	{
		pinLeft = requestPinLeft;
		pinRight = requestPinRight;
		buttonIdLeft = requestButtonIdLeft;
		buttonIdRight = requestButtonIdRight;
		debouncingPeriod = requestDebouncingPeriod;

		pinMode(pinLeft, INPUT);
		pinMode(pinRight, INPUT);
	}

	// 로터리 엔코더 신호를 디바운싱 합니다.
	void execute()
	{
		int result = getValue();

		if (result == 0)
		{
			if (millis() - ms >= debouncingPeriod)
			{
				if (oldValue != 0)
				{
					gameController.releaseButton(buttonIdLeft);
					gameController.releaseButton(buttonIdRight);
					oldValue = 0;
				}
			}
		}
		else
		{
			if (result != oldValue)
			{
				gameController.releaseButton(buttonIdLeft);
				gameController.releaseButton(buttonIdRight);
				if (result == 1)
				{
					gameController.pressButton(buttonIdLeft);
				}
				else if (result == -1)
				{
					gameController.pressButton(buttonIdRight);
				}
				oldValue = result;
			}
			ms = millis();
		}
	}

	// 로터리 엔코더 신호를 받습니다.
	int getValue()
	{
		int result = 0;
		int valueA = digitalRead(pinLeft);
		int valueB = digitalRead(pinRight);

		if (oldLeft == 0 && oldRight == 0)
		{
			if (valueA == 1 && oldRight == 0)
			{
				result = 1;
				ms = millis();
			}
			else if (valueA == 0 && oldRight == 1)
			{
				result = -1;
				ms = millis();
			}
			else
			{
				if (millis() - ms > 100)
				{
					result = 0;
				}
			}
		}
		else if (oldLeft == 1 && oldRight == 0)
		{
			if (valueA == 1 && valueB == 1)
			{
				result = 1;
				ms = millis();
			}
			else if (valueA == 0 && valueB == 0)
			{
				result = -1;
				ms = millis();
			}
			else
			{
				if (millis() - ms > 100)
				{
					result = 0;
				}
			}
		}
		else if (oldLeft == 1 && oldRight == 1)
		{
			if (valueA == 0 && valueB == 1)
			{
				result = 1;
				ms = millis();
			}
			else if (valueA == 1 && valueB == 0)
			{
				result = -1;
				ms = millis();
			}
			else
			{
				if (millis() - ms > 100)
				{
					result = 0;
				}
			}
		}
		else if (oldLeft == 0 && oldRight == 1)
		{
			if (valueA == 1 && valueB == 1)
			{
				result = -1;
				ms = millis();
			}
			else if (valueA == 0 && oldRight == 0)
			{
				result = 1;
				ms = millis();
			}
			else
			{
				if (millis() - ms > 100)
				{
					result = 0;
				}
			}
		}
		oldLeft = valueA;
		oldRight = valueB;

		return result;
	}
};
