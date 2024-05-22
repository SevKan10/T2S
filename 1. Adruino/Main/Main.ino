#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/*=========OLED=========*/

#define BLYNK_TEMPLATE_ID "TMPL6FBFJXTeP"
#define BLYNK_TEMPLATE_NAME "T2S"
#define BLYNK_AUTH_TOKEN "ew1TOzN0pqj0zEr1HK-6S2juw6HfvDHU"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
/*---------BLYNK----------*/

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

static float angle = 0.0;
const int radius = 15;
const int centerX = 64;
const int centerY = 32;
const int numDots = 7;
int flagCorrect = 0;
int flagStart = 0;
int Lux;
/*=========Variable=========*/

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  Blynk.begin(auth, ssid, pass);
  Wire.begin(23, 22);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) // Kiểm tra Wifi
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  if (!display.begin(i2c_Address, true)) // Kiểm tra màn hình 
  {  
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
  while (millis() - startMillis < 7000) 
  { 
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(" TITRA TECH STANDARD");
    for (int i = 0; i < numDots; i++) 
    {
      float dotAngle = radians(angle + i * 360.0 / numDots);
      int x = centerX + radius * cos(dotAngle);
      int y = centerY + radius * sin(dotAngle);
      display.drawPixel(x, y, SH110X_WHITE);
    }
    angle += 10; 
    if (angle >= 360.0) 
    {
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
  pinMode(button_Correct, INPUT_PULLUP);
  pinMode(button_Start, INPUT_PULLUP);

  digitalWrite(R_acid,0);
  digitalWrite(R_base,0);
  digitalWrite(R_phenol,0);
  digitalWrite(R_water,0);
  digitalWrite(R_sti,0);
  ringBell(100,2);

}

void loop()
{
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(" TITRA TECH STANDARD");
  display.display();

  if (digitalRead(button_Correct) == 0) // Hiệu chuẩn
  {
    String data = "!none!none!Correcting!";
    Blynk.virtualWrite(V1, data); 

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Correcting");
    display.display();

    digitalWrite(R_acid,1);
    digitalWrite(R_base,1);
    digitalWrite(R_phenol,1);
    delay(20000);
    display.clearDisplay();
    ringBell(100,2);

    flagCorrect++;
    if(flagCorrect >=2){flagCorrect = 0;}
  }
  switch(flagCorrect) // Giao diện nút hiệu chuẩn
  {
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
      digitalWrite(R_acid,1);
      digitalWrite(R_base,1);
      digitalWrite(R_phenol,1);

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

 if (digitalRead(button_Start) == 0 && flagCorrect == 0)
  {
    Serial.println("Please press correction button");
    display.clearDisplay();

    flagStart = 2;
  }
  else if (digitalRead(button_Start) == 0 && flagCorrect != 0 )
  {
    Serial.println("Stating process titra");
    display.clearDisplay();

    flagStart = 1;
  }

switch(flagStart) {
  case 1: {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(" TITRA TECH STANDARD");
    display.display();

    String data = "!HCl! !Titrating!"; //Acid
    Blynk.virtualWrite(V1, data);

    display.setTextSize(1); 
    display.setCursor(0, 10);
    display.println("Chemistry: HCl ");
    display.display();
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Titrating");
    display.display();
    digitalWrite(R_base, LOW); 
    digitalWrite(R_phenol, LOW); 

    for(int i = 1; i < 10; i++) {
      digitalWrite(R_acid, HIGH);
      delay(250);
      digitalWrite(R_acid, LOW);
      delay(2000);
    }
    digitalWrite(R_acid, LOW); 
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Volume: 100ml");
    display.display();
    String data1 = "!HCl!100ml!Titrating!"; //Acid
    Blynk.virtualWrite(V1, data1);
    delay(1000);

    display.drawRect(15, 48, 100, 15, SH110X_WHITE); // hiệu ứng vẽ đường load
    display.fillRect(15, 48, 25, 15, SH110X_WHITE);
    display.display();

    String data2 = "!Phenol! !Titrating!"; // Phenol
    Blynk.virtualWrite(V1, data2);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("Chemistry: Phenol");
    display.display();
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Titrating");
    display.display();
    digitalWrite(R_acid, LOW); 
    digitalWrite(R_base, LOW); 
    digitalWrite(R_sti, HIGH); 

    for(int i = 1; i < 3; i++) {
      digitalWrite(R_phenol, HIGH);
      delay(250);
      digitalWrite(R_phenol, LOW);
      delay(2000);
    }                                     
    digitalWrite(R_phenol, LOW);
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Volume: 5ml");
    display.display();
    String data3 = "!Phenol!5ml!Titrating!"; // Phenol
    Blynk.virtualWrite(V1, data3);
    delay(1000);

    display.drawRect(15, 48, 100, 15, SH110X_WHITE); // hiệu ứng vẽ đường load
    display.fillRect(15, 48, 50, 15, SH110X_WHITE);
    display.display();

    String data4 = "!NaOH! !Titrating!"; // Base
    Blynk.virtualWrite(V1, data4);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("Chemistry: NaOH");
    display.display();
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Titrating");
    display.display();
    digitalWrite(R_acid, LOW); 
    digitalWrite(R_phenol, LOW); 

    if (Lux <= 100) {
      digitalWrite(R_base, HIGH);
      delay(250);
      digitalWrite(R_base, LOW);
      delay(2000);
    } else {
      digitalWrite(R_acid, LOW);
      digitalWrite(R_base, LOW);
      digitalWrite(R_phenol, LOW);
      digitalWrite(R_sti, LOW);
    }
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Volume: 110ml");
    display.display();
    String data5 = "!NaOH!110ml!Titrating!"; // Base
    Blynk.virtualWrite(V1, data5);
    delay(1000);

    display.drawRect(15, 48, 100, 15, SH110X_WHITE); // hiệu ứng vẽ đường load
    display.fillRect(15, 48, 75, 15, SH110X_WHITE);
    display.display();

    display.clearDisplay();
    display.setTextSize(1); 
    display.setCursor(0, 10);
    display.println("Chemistry: ");
    display.display();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Volume: ");
    display.display();
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Calculating");
    display.display();
    /* thuật toán tính nồng độ CHCl.VHCl = CNaOH.VNaOH*/

    display.drawRect(15, 48, 100, 15, SH110X_WHITE); // hiệu ứng vẽ đường load
    display.fillRect(15, 48, 90, 15, SH110X_WHITE);
    display.display();

    // Đẩy dữ liệu sau khi tính toán
    // Blynk.virtualWrite(V2, ); // thể tích !V1!V2!V3!
    // Blynk.virtualWrite(V3, ); // nồng độ !C1!C2!C3!
    // Blynk.virtualWrite(V4, ); // trung bình thể tích và nồng độ !Vavg!Cavg!

    // Sau khi chuẩn độ xong bật motor nước
    /*digitalWrite(R_acid, LOW);
    digitalWrite(R_base, LOW);
    digitalWrite(R_phenol, LOW);
    digitalWrite(R_sti, LOW);
    digitalWrite(R_water, HIGH); 
    String data = "! ! !Washing!"; // Phenol
    Blynk.virtualWrite(V1, data);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("Chemistry:");
    display.display();

    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Volume: ");
    display.display();

    display.setTextSize(1);
    display.setCursor(0, 30);
    display.println("Process: Washing");
    display.display();*/

    break;
  }
  case 2: {
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.println("Correction is none");
    display.display();
    delay(2000);
    flagStart = 0;
    flagCorrect = 0;
    break;
  }
}

}