#include <Wire.h>
#include "RTClib.h"
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>

char KEYS[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
int ManyShrimp = 0; 
int Volume = 0;
int dday;mmonth;yyear;ho;mi;se;
char timearray[] = "000000";
byte counttimearray = 0;
byte statemenu = 0;
byte slectmenu = 1;
byte i=9;

float calibration_factor =99757.00; 
#define zero_factor 8573573
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  2
float offset=0;                 float get_units_kg();
HX711 scale(DOUT, CLK);

LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTC_DS1307 rtc;
OnewireKeypad <LiquidCrystal_I2C, 16> KP( lcd, KEYS, 4, 4, A1, 4700, 1000 );

float get_units_kg() {
  return(scale.get_units()*0.453592);
}
void openmenu() {
  lcd.clear(); delay(1000);
}
void closemenu() {
  delay(5000);lcd.clear();
}
void into() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 1);
  lcd.print("  Time = "); lcd.print(now.hour(), DEC); lcd.print(':'); lcd.print(now.minute(), DEC); lcd.print(':'); lcd.print(now.second(), DEC); lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print("  Date = "); lcd.print(now.day(), DEC); lcd.print('/'); lcd.print(now.month(), DEC); lcd.print('/'); lcd.print(now.year(), DEC);
  delay(900);
}
void disdate() {
  lcd.print("  Date = "); lcd.print(dday, DEC); lcd.print('/'); lcd.print(mmonth, DEC); lcd.print('/'); lcd.print(yyear, DEC);
}
void distime() {
  ho =(timearray[0]*10)+timearray[1];
  mi =(timearray[2]*10)+timearray[3];
  se =(timearray[4]*10)+timearray[5];
  lcd.print("Time = ");lcd.print(ho, DEC);lcd.print(':');lcd.print(mi, DEC);lcd.print('');lcd.print(se, DEC);
}
void menu() {
  if (slectmenu == 1) {
    lcd.setCursor(0, 0);    lcd.print(">> Set Date");
    lcd.setCursor(0, 1);    lcd.print("   Set Time");
    lcd.setCursor(0, 2);    lcd.print("   Set Many Shrimp");
    lcd.setCursor(0, 3);    lcd.print("   Set Volume");
    }
  if (slectmenu == 2) {
    lcd.setCursor(0, 0);    lcd.print("   Set Date");
    lcd.setCursor(0, 1);    lcd.print(">> Set Time");
    lcd.setCursor(0, 2);    lcd.print("   Set Many Shrimp");
    lcd.setCursor(0, 3);    lcd.print("   Set Volume");
    }
  if (slectmenu == 3) {
    lcd.setCursor(0, 0);    lcd.print("   Set Date");
    lcd.setCursor(0, 1);    lcd.print("   Set Time");
    lcd.setCursor(0, 2);    lcd.print(">> Set Many Shrimp");
    lcd.setCursor(0, 3);    lcd.print("   Set Volume");
    }
  if (slectmenu == 4) {
    lcd.setCursor(0, 0);    lcd.print("   Set Date");
    lcd.setCursor(0, 1);    lcd.print("   Set Time");
    lcd.setCursor(0, 2);    lcd.print("   Set Many Shrimp");
    lcd.setCursor(0, 3);    lcd.print(">> Set Volume");
    }
}
void MenuSetDate() {
  openmenu();
  DateTime now = rtc.now();
  dday=now.day();  mmonth=now.month();  yyear=now.year();
  lcd.setCursor(0, 0);
  lcd.print("Saving Date");
  lcd.setCursor(0, 2);
  disdate();
  closemenu();
}
void MenuSetTime() {
  openmenu();
  lcd.setCursor(0, 0);      lcd.print("***Set Time***");
  lcd.setCursor(0, 1);      lcd.print("Form HH:MM:SS");
  if (KP.Getkey() != NO_KEY){
    lcd.setCursor(i, 2);    
    lcd.print(KP.Getkey());
    timearray[counttimearray] = KP.Getkey();
    counttimearray++;i++;
    delay(150);
    if (counttimearray == 6){
      openmenu();
      lcd.setCursor(0, 0); 
      lcd.print("Time Saving");
      lcd.setCursor(6, 1); 
      distime();
      byte i=9; counttimearray=0;
      closemenu();
    }
  }
}
void MenuSetManyShrimp() {
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("***Many Shrimp***");
  if (KP.Getkey() != NO_KEY){
    lcd.setCursor(i, 2);    
    lcd.print(KP.Getkey());
    timearray[counttimearray] = KP.Getkey();
    counttimearray++;i++;
    delay(150);
    if (counttimearray == 4){
      lcd.clear();
      delay(1000);
      lcd.setCursor(6, 1); 
      lcd.print("Time Saving");
      delay(2000);
      lcd.clear();
      byte i=9; counttimearray=0;
    }
  }
}
void MenuSetVolume() {
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("***SetVolume***");
  if (KP.Getkey() != NO_KEY){
    lcd.setCursor(i, 2);    
    lcd.print(KP.Getkey());
    timearray[counttimearray] = KP.Getkey();
    counttimearray++;i++;
    delay(150);
    if (counttimearray == 3){
      lcd.clear();
      delay(1000);
      lcd.setCursor(6, 1); 
      lcd.print("Time Saving");
      delay(2000);
      lcd.clear();
      byte i=9; counttimearray=0;
    }
  }
}
/*****************************************************************************************************************************/
void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();
  rtc.begin();
  lcd.clear();
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);
}
void loop() {
  
  String data = String(get_units_kg()+offset, DEC_POINT);
  
start:
  statemenu = 0;
  into();
  char keymenu = KP.Getkey();
  
  
  if (keymenu == '*') {//Loop Checking *
    statemenu = 1;
    lcd.clear();delay(1000);
    while (statemenu == 1) {
      //lcd.print(datearray);
      lcd.print(timearray);
      lcd.print(ManyShrimp);
      lcd.print(Volume);
      
      char fnmenu = KP.Getkey();
      if (fnmenu == 'D') {
        lcd.clear();  //Exit Menu
        goto start;
      }
      statemenu = 1;
    }
  //End Loop Checking
  }

   if (keymenu == '#') {//Loop Setup #
    slectmenu = 1;
    statemenu = 1;
    lcd.clear();delay(1000);
    while (statemenu == 1) {
      delay(250);
      char fnmenu = KP.Getkey();
      if ((fnmenu == 'A')&&(slectmenu!=1)){   //UP
        slectmenu--;
        menu();
      }
      if ((fnmenu == 'B')&&(slectmenu!=4)){   //Down
        slectmenu++; 
        menu();
      }
      if (fnmenu == 'C') {//Enter Menu
        
      }        
      if (fnmenu == 'D') {
        lcd.clear();  //Exit Menu
        goto start;
      }
      statemenu = 1;
    }
  //End Loop Setup
  }

//END
}
