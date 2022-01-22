struct Button
{
	boolean isReversedSignal; // Start Button 처럼 0 1 신호가 서로 반대인 경우?
	int pinId;
	int buttonId;
	Signal sensor;

	void init(int pin, int requestButtonId, int debouncingPeriod)
	{
		pinMode(pin, INPUT);
		pinId = pin;
		buttonId = requestButtonId;
		sensor.init(debouncingPeriod);
	}

	void execute()
	{
		int verifiedValue = sensor.verify(digitalRead(pinId));
		if (verifiedValue == 0)
		{
			gameController.pressButton(buttonId);
		}
		else if (verifiedValue == 1)
		{
			gameController.releaseButton(buttonId);
		}
	}
};
