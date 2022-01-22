// 버튼과 로터리 엔코더 신호를 계속해서 읽어들이며, 이를 가상 조이스틱 신호로 변환합니다.
void loop()
{
	buttonStart.execute();
	buttonA.execute();
	buttonB.execute();
	buttonC.execute();
	buttonD.execute();
	fxA.execute();
	fxB.execute();

	knobLeft.execute();
	knobRight.execute();
}
