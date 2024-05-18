#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;
DateTime now;

#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);


#define R1 13
#define R2 12
#define R3 14
#define R4 27
#define R5 26
#define R6 25
#define R7 33 
#define R8 32
#define BUTTON 4

bool state = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() 
{
  Serial.begin(9600);
  Wire.begin(23, 22);
 
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {Serial.println("Couldn't find RTC");while (1);}
  if (!rtc.isrunning()) {Serial.println("RTC is NOT running!"); rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));}

  if (!tcs.begin()){Serial.println("Color Sensor Fail"); while (1);}

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
}

void loop() 
{
  if(digitalRead(BUTTON)==0)
  {
    if (state == 0) {
      digitalWrite(R1, HIGH); 
      digitalWrite(R2, HIGH); 
      digitalWrite(R3, HIGH); 
      digitalWrite(R4, HIGH); 
      digitalWrite(R5, HIGH); 
      digitalWrite(R6, HIGH); 
      digitalWrite(R7, HIGH); 
      digitalWrite(R8, HIGH); 
      state = 1; 
    } else {
      digitalWrite(R1, LOW); 
      digitalWrite(R2, LOW); 
      digitalWrite(R3, LOW); 
      digitalWrite(R4, LOW); 
      digitalWrite(R5, LOW); 
      digitalWrite(R6, LOW); 
      digitalWrite(R7, LOW); 
      digitalWrite(R8, LOW); 
      state = 0; 
    }
    delay(100);
  }

  now = rtc.now();
  printTime(); 
  color();
}

void printTime() 
{
  lcd.setCursor(0,0);
  lcd.print(now.day(),DEC); lcd.print("/"); lcd.print(now.month(),DEC); lcd.print("/"); lcd.print(now.year(),DEC); lcd.print("|");
  
  lcd.setCursor(10,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

  lcd.setCursor(0,1);
  lcd.print(now.hour(),DEC); lcd.print(":"); lcd.print(now.minute(),DEC); lcd.print(":"); lcd.print(now.second(),DEC); lcd.print("   ");

}

void color()
{
  uint16_t r, g, b, c, colorTemp, lux;
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b); //Nhiệt độ màu theo thang đo Kelvin
    lux = tcs.calculateLux(r, g, b); //Độ rọi soi, cường độ sáng
     
    Serial.print("Color Temp: "); Serial.print(colorTemp); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux); Serial.print(" - ");
    Serial.print("Red: "); Serial.print(r); Serial.print(" ");
    Serial.print("Green: "); Serial.print(g); Serial.print(" ");
    Serial.print("Blue: "); Serial.print(b); Serial.print(" ");
    Serial.print("Clear: "); Serial.print(c); Serial.print(" ");
    Serial.println(" ");

    if(c>r && c>g && c>b && lux>500) //Không có màu
    {
      Serial.println(" Màu sắc hiện tại là: KHÔNG CÓ MÀU");
      Serial.println(" ");
    }
    else if(r>g && r>b && r<c && lux>10 && lux<100) //Màu đỏ
    {
      Serial.println(" Màu sắc hiện tại là: MÀU ĐỎ");
      Serial.println(" ");  
    }
    else if(g>r && g>b && g<c && lux>200 && lux<300) //Màu lục
    {
      Serial.println(" Màu sắc hiện tại là: MÀU LỤC");
      Serial.println(" ");         
    }
    else if(b>r && b>g && b<c && lux>100 && lux<200) //Màu xanh biển
    {
      Serial.println(" Màu sắc hiện tại là: MÀU XANH BIỂN");
      Serial.println(" ");        
    }
}