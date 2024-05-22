void ringBell(int delayTimes, int repeatTimes)
{
  for(int i=1; i<=repeatTimes; i++)
  {
    digitalWrite(buzzer, 1); delay(delayTimes);
    digitalWrite(buzzer, 0); delay(delayTimes);
  }
}
void titrationProcess(String chemistry, int volume, int repetitions, int pin) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Chemistry: " + chemistry);
  display.display();
  display.setCursor(0, 30);
  display.println("Process: Titrating");
  display.display();
  for (int i = 1; i <= repetitions; i++) {
    digitalWrite(pin, 1);
    delay(250);
    digitalWrite(pin, 0);
    delay(2000);
  }
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Volume: " + String(volume) + "ml");
  display.display();
  Blynk.virtualWrite(V1, "!" + chemistry + "!" + String(volume) + "ml!Titrating!");
  delay(1000);
  display.drawRect(15, 48, 100, 15, SH110X_WHITE);
  display.fillRect(15, 48, (volume / 100) * 75, 15, SH110X_WHITE);
  display.display();
}
