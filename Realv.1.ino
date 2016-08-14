#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>

char KEYS[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
byte statemenu = 0;
byte slectmenu = 1;

LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTC_DS1307 rtc;
OnewireKeypad <LiquidCrystal_I2C, 16> KP( lcd, KEYS, 4, 4, A1, 4700, 1000 );

void into(){
  DateTime now = rtc.now();
  lcd.setCursor(0,1);
  lcd.print("  Time = ");lcd.print(now.hour(), DEC);lcd.print(':');lcd.print(now.minute(), DEC);lcd.print(':');lcd.print(now.second(), DEC);lcd.print(" ");
  lcd.setCursor(0,2);
  lcd.print("  Date = ");lcd.print(now.day(), DEC);lcd.print('/');lcd.print(now.month(), DEC);lcd.print('/');lcd.print(now.year(), DEC);
  delay(900);
}

void menu(){
  char fnmenu = KP.Getkey();
  if(slectmenu==1){
    lcd.setCursor(0,0);    lcd.print("Menu");
    lcd.setCursor(0,1);    lcd.print(">> Set Time");
    lcd.setCursor(0,2);    lcd.print("   Set Many Shrimp");
    lcd.setCursor(0,3);    lcd.print("   Set Volume");
  }
  if(slectmenu==2){
    lcd.setCursor(0,0);    lcd.print("Menu");
    lcd.setCursor(0,1);    lcd.print("   Set Time");
    lcd.setCursor(0,2);    lcd.print(">> Set Many Shrimp");
    lcd.setCursor(0,3);    lcd.print("   Set Volume");
  }
  if(slectmenu==3){
    lcd.setCursor(0,0);    lcd.print("Menu");
    lcd.setCursor(0,1);    lcd.print("   Set Time");
    lcd.setCursor(0,2);    lcd.print("   Set Many Shrimp");
    lcd.setCursor(0,3);    lcd.print(">> Set Volume");
  }
}

void fnmenu(){
  if(fnmenu=='A')slectmenu++;  //UP
  if(fnmenu=='B')slectmenu--;  //Down
  if(fnmenu=='C');{            //Enter Menu
  
  }
  if(fnmenu=='D');statemenu=0; //Exit Menu
  
}

void MenuSettingTime(){
  
}

void SetManyShrimp(){
  
}

void SetVolume(){
  
}


/************************************************************************************/

void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();
  rtc.begin();
}

void loop() {
  into();
  char keymenu = KP.Getkey();
  if(keymenu == 'A')statemenu=1;
  Serial.println(statemenu);
  
  while(statemenu == 1){
    
  
  }
}
