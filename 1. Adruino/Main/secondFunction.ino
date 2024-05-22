void ringBell(int delayTimes, int repeatTimes)
{
  for(int i=1; i<=repeatTimes; i++)
  {
    digitalWrite(buzzer, 1); delay(delayTimes);
    digitalWrite(buzzer, 0); delay(delayTimes);
  }
}