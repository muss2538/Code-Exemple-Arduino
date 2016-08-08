#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>


RTC_DS1307 rtc;    // Create a RealTimeClock object

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {
  Serial.begin(9600);
  Serial.println("YourDuino.com DS1307 Real Time Clock - Set / Run Utility");
  Wire.begin();
  rtc.begin();
  lcd.begin();

}

void loop(){
  DateTime now = rtc.now();  // Read data from the RTC Chip

//  Serial.print(now.year(), DEC); //
//  Serial.print('/');
//  Serial.print(now.month(), DEC);
//  Serial.print('/');
//  Serial.print(now.day(), DEC);
//  Serial.print(' ');
//  Serial.print(now.hour(), DEC);
//  Serial.print(':');
//  Serial.print(now.minute(), DEC);
//  Serial.print(':');
//  Serial.print(now.second(), DEC);
//  Serial.println();

  String timeread = "Time = "+String(now.hour())+":"+String(now.minute())+":"+String(now.second());
  String dateread = "Date = "+String(now.day())+"/"+String(now.month())+"/"+String(now.year());
  lcd.setCursor(0,1);
  lcd.print(timeread);
  lcd.setCursor(0,2);
  lcd.print(dateread);


  delay(1000);
  lcd.clear();
}

