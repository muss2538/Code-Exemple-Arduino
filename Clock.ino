#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);


RTC_DS1307 rtc;


void setup () {
  lcd.begin();
  Wire.begin();
  rtc.begin();}


void loop () {
  DateTime now = rtc.now();
  lcd.setCursor(0,1);
  lcd.print("  Time = ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print(" ");
  lcd.setCursor(0,2);
  lcd.print("  Date = ");lcd.print(now.day(), DEC);lcd.print('/');lcd.print(now.month(), DEC);lcd.print('/');lcd.print(now.year(), DEC);
  delay(900);

  int hr=now.hour();
  int mi=now.minute();
  int se=now.second();
  
  
  if((hr==8 && mi==0 && se==0) || (hr==18 && mi==0 && se==0)){ 
  
  }
}
