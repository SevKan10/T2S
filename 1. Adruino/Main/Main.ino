#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/*=========OLED=========*/

#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
/*=========Cảm biến màu sắc=========*/

#define BLYNK_TEMPLATE_ID "TMPL6FBFJXTeP"
#define BLYNK_TEMPLATE_NAME "T2S"
#define BLYNK_AUTH_TOKEN "ew1TOzN0pqj0zEr1HK-6S2juw6HfvDHU"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define button_Correct 21  // nút hiệu chuẩn
#define button_Start 19    // nút khởi động

#define R_acid 32    // dung dịch acid
#define R_base 33    // dung dich base
#define R_phenol 25  // dung dịch phenol
#define R_water 26   // nước
#define R_sti 27     // máy khoáy từ
#define buzzer 2     // còi

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Khang nè";
char pass[] = "12345678@";

static float angle = 0.0;  // biến hiệu ứng load
const int radius = 15;
const int centerX = 64;
const int centerY = 32;
const int numDots = 7;

int flagCorrect = 0;  // biến cờ hiệu chuẩn
int flagStart = 0;    // biến cờ bắt đầu
int buttonBlynk;      // nút từ app

void setup() {
  Serial.begin(9600);
  Wire.begin(23, 22);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

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
  while (millis() - startMillis < 10000) {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(" TITRA TECH STANDARD");
    for (int i = 0; i < numDots; i++) {
      float dotAngle = radians(angle + i * 360.0 / numDots);
      int x = centerX + radius * cos(dotAngle);
      int y = centerY + radius * sin(dotAngle);
      display.drawPixel(x, y, SH110X_WHITE);
    }
    angle += 10;
    if (angle >= 360.0) {
      angle = 0.0;
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
  /*----------Loading effect--------*/

  pinMode(R_acid, OUTPUT);
  pinMode(R_base, OUTPUT);
  pinMode(R_phenol, OUTPUT);
  pinMode(R_water, OUTPUT);
  pinMode(R_sti, OUTPUT);
  pinMode(buzzer, OUTPUT);
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

  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: ");
  Serial.print(colorTemp);
  Serial.print(" K - ");
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.print(" - ");
  Serial.print("Red: ");
  Serial.print(r);
  Serial.print(" ");
  Serial.print("Green: ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print("Blue: ");
  Serial.print(b);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(" TITRA TECH STANDARD");
  display.display();

  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Correct:   ");
  display.display();

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Start  :   ");
  display.display();

  if (digitalRead(button_Correct) == 0) {
    String data = "! ! !Correcting!";
    Blynk.virtualWrite(V1, data);

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Correcting");
    display.display();

    digitalWrite(R_acid, 1);
    digitalWrite(R_base, 1);
    digitalWrite(R_phenol, 1);
    digitalWrite(R_water, 1);


  } else {
    digitalWrite(R_acid, 0);
    digitalWrite(R_base, 0);
    digitalWrite(R_phenol, 0);
    digitalWrite(R_water, 0);
  }
}
