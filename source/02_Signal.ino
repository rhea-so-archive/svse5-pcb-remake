struct Signal
{
	int old = 0;				 // 이전 신호
	int debouncingPeriod;		 // 디바운싱 할 범위
	unsigned long ms = millis(); // 이전 신호의 실행 시간

	void init(int requestDebouncingPeriod)
	{
		debouncingPeriod = requestDebouncingPeriod;
	}

	int verify(int currentValue)
	{
		if (currentValue == 0)
		{
			if (old == 1)
			{
				if (millis() - ms >= debouncingPeriod)
				{
					old = currentValue;
					return currentValue;
				}
			}
		}
		else
		{
			ms = millis();
			if (old == 0)
			{
				old = currentValue;
				return currentValue;
			}
		}

		return -1;
	}
};
