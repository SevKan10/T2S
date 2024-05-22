#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BLYNK_TEMPLATE_ID "TMPL6FBFJXTeP"
#define BLYNK_TEMPLATE_NAME "T2S"
#define BLYNK_AUTH_TOKEN "ew1TOzN0pqj0zEr1HK-6S2juw6HfvDHU"

#define button_Correct 21
#define button_Start 19
#define R_sti 27
#define R_water 26
#define R_phenol 25
#define R_base 33
#define R_acid 32
#define buzzer 2

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Xưởng 1";
char pass[] = "xuong2dangxay";

int flagCorrect = 0;
int flagStart = 0;
int buttonBlynk;
int Lux;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  Wire.begin(23, 22);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  if (!display.begin(i2c_Address, true)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  Serial.println("Booting time...");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(23, 8);
  display.println("WELCOME");
  display.display();
  delay(2000);
  display.clearDisplay();

  unsigned long startMillis = millis();
  while (millis() - startMillis < 7000) {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(" TITRA TECH STANDARD");
    for (int i = 0; i < 7; i++) {
      float dotAngle = radians(i * 360.0 / 7);
      int x = 64 + 15 * cos(dotAngle);
      int y = 32 + 15 * sin(dotAngle);
      display.drawPixel(x, y, SH110X_WHITE);
    }
    display.display();
    delay(100);
    display.clearDisplay();
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(" TITRA TECH STANDARD");
  display.display();
  display.clearDisplay();

  pinMode(R_acid, OUTPUT);
  pinMode(R_base, OUTPUT);
  pinMode(R_phenol, OUTPUT);
  pinMode(R_water, OUTPUT);
  pinMode(R_sti, OUTPUT);
  pinMode(button_Correct, INPUT_PULLUP);
  pinMode(button_Start, INPUT_PULLUP);

  digitalWrite(R_acid, 0);
  digitalWrite(R_base, 0);
  digitalWrite(R_phenol, 0);
  digitalWrite(R_water, 0);
  digitalWrite(R_sti, 0);
}

void loop() {
  Blynk.run();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(" TITRA TECH STANDARD");
  display.display();

  if (buttonBlynk == 1) {
    flagStart = 1;
  } else if (buttonBlynk == 2) {
    String data = "!none!none!Correcting!";
    Blynk.virtualWrite(V1, data);

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Correcting");
    display.display();

    digitalWrite(R_acid, 1);
    digitalWrite(R_base, 1);
    digitalWrite(R_phenol, 1);
    delay(20000);
    display.clearDisplay();
    flagCorrect = 1;
  }

  if (digitalRead(button_Correct) == 0) {
    String data = "!none!none!Correcting!";
    Blynk.virtualWrite(V1, data);

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Correcting");
    display.display();

    digitalWrite(R_acid, 1);
    digitalWrite(R_base, 1);
    digitalWrite(R_phenol, 1);
    delay(20000);
    display.clearDisplay();
    flagCorrect++;
    if (flagCorrect >= 2) {
      flagCorrect = 0;
    }
  }
  switch (flagCorrect) {
    case 0:
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.println("Correct:none");
      display.display();

      display.setTextSize(1);
      display.setCursor(0, 20);
      display.println("Start  :none ");
      display.display();

      display.setTextSize(1);
      display.setCursor(0, 30);
      display.println("Process:           ");
      display.display();
      break;
    case 1:
      digitalWrite(R_acid, 1);
      digitalWrite(R_base, 1);
      digitalWrite(R_phenol, 1);

      display.setTextSize(1);
      display.setCursor(0, 10);
      display.println("Correct:OK  ");
      display.display();

      display.setTextSize(1);
      display.setCursor(0, 20);
      display.println("Start  :Ready ");
      display.display();

      display.setTextSize(1);
      display.setCursor(0, 30);
      display.println("Process:           ");
      display.display();
      break;
  }

  if (digitalRead(button_Start) == 0 && flagCorrect == 0) {
    Serial.println("Please press correction button");
    display.clearDisplay();
    flagStart = 2;
  } else if (digitalRead(button_Start) == 0 && flagCorrect != 0) {
    Serial.println("Stating process titra");
    display.clearDisplay();
    flagStart = 1;
  }

  switch (flagStart) {
    case 1:
      titrationProcess("HCl", 100, 10, R_acid);
      titrationProcess("Phenol", 5, 3, R_phenol);
      titrationProcess("NaOH", 110, 1, R_base);
      break;
    case 2:
      display.setTextSize(1);
      display.setCursor(0, 40);
      display.println("Correction is none");
      display.display();
  }
}



