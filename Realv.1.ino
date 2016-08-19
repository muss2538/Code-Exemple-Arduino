#include <Wire.h>
#include "RTClib.h"
//#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>

char KEYS[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
unsigned int ManyShrimp = 0; 
unsigned int Volume = 0;
unsigned int dday = 0;
unsigned int mmonth = 0;
unsigned int yyear = 0;
unsigned int ho = 0;
unsigned int mi = 0;
unsigned int se = 0;

int timearray[] = {1,8,5,0,3,0};
int manyarray[] = {1,0,0,0};
int volumearray[] = {5,0,0};
byte counttimearray = 0;
byte countmanyarray = 0;
byte countvolumearray = 0;

byte statemenu = 0;
byte slectmenu = 1;
byte i=0;

//float calibration_factor =99757.00; 
//#define zero_factor 8573573
//#define DOUT  A3
//#define CLK   A2
//#define DEC_POINT  2
float offset=0;                 float get_units_kg();
//HX711 scale(DOUT, CLK);

LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTC_DS1307 rtc;
OnewireKeypad <LiquidCrystal_I2C, 16> KP( lcd, KEYS, 4, 4, A1, 4700, 1000 );

//float get_units_kg() {
//  return(scale.get_units()*0.453592);
//}
void openmenu() {
  lcd.clear(); delay(1000);
}
void closemenu() {
  delay(3500);lcd.clear();
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
  lcd.print("Date = "); lcd.print(dday, DEC); lcd.print('/'); lcd.print(mmonth, DEC); lcd.print('/'); lcd.print(yyear, DEC);
}
void distime() {
  ho =(timearray[0]*10)+timearray[1];
  mi =(timearray[2]*10)+timearray[3];
  se =(timearray[4]*10)+timearray[5];
  lcd.print("Time = ");lcd.print(ho);lcd.print(":");lcd.print(mi);lcd.print(":");lcd.print(se);
}
void dismany() {
  ManyShrimp =((manyarray[0]*1000)+(manyarray[1]*100)+(manyarray[2]*10)+manyarray[3]);
  lcd.print("Many = ");lcd.print(ManyShrimp);
}
void disvolume() {
  Volume =((volumearray[0]*100)+(volumearray[1]*10)+volumearray[2]);
  lcd.print("Vol. = ");lcd.print(Volume);lcd.print(" Gram");
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
  lcd.setCursor(0, 0);  lcd.print("Saving Date...");
  lcd.setCursor(0, 2);  disdate();
  closemenu();
}
void MenuSetTime() {
  openmenu();
  while(counttimearray < 6) {
    lcd.setCursor(0, 0);      lcd.print("***Set Time***");
    lcd.setCursor(0, 1);      lcd.print("Form HH:MM:SS");
    if (KP.Getkey() != NO_KEY){
      lcd.setCursor(i, 2);    
      lcd.print(KP.Getkey());
      timearray[counttimearray] = KP.Getkey()-'0';
      counttimearray++;i++;
      delay(250);
    }
  }
  openmenu();
  lcd.setCursor(0, 0);   lcd.print("Time Saving...");
  lcd.setCursor(0, 2);   distime();
  i=0; counttimearray=0;
  closemenu();
}
void MenuSetManyShrimp() {
  openmenu();
  lcd.setCursor(0, 0);  lcd.print("***Many Shrimp***");
  while(countmanyarray < 4) {
    if (KP.Getkey() != NO_KEY){
      lcd.setCursor(i, 2);    
      lcd.print(KP.Getkey());
      manyarray[countmanyarray] = KP.Getkey()-'0';
      countmanyarray++;i++;
      delay(250);
    }  
  }
  openmenu();
  lcd.setCursor(0, 0);   lcd.print("Many Saving...");
  lcd.setCursor(0, 2);   dismany();
  i=0; countmanyarray=0;
  closemenu();
}
void MenuSetVolume() {
  openmenu();
  lcd.setCursor(0, 0);  lcd.print("***SetVolume***");
  while(countvolumearray < 3){
    if (KP.Getkey() != NO_KEY){
      lcd.setCursor(i, 2);    
      lcd.print(KP.Getkey());
      volumearray[countvolumearray] = KP.Getkey()-'0';
      countvolumearray++;i++;
      delay(250);
    }
  }
  openmenu();
  lcd.setCursor(0, 1);  lcd.print("Volume Saving...");
  lcd.setCursor(0, 2);   disvolume();
  i=0; countvolumearray=0;
  closemenu();
}
void EnterMenu() {
  if (slectmenu == 1) {MenuSetDate();}
  if (slectmenu == 2) {MenuSetTime();}
  if (slectmenu == 3) {MenuSetManyShrimp();}
  if (slectmenu == 4) {MenuSetVolume();}
}

/*****************************************************************************************************************************/
void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();
  rtc.begin();
  lcd.print("Deivce Power ON");
  Serial.println("Deivce Power ON");
  closemenu();
  //scale.set_scale(calibration_factor); 
  //scale.set_offset(zero_factor);
}
void loop() {
  Serial.println("Loop intro");
  //String data = String(get_units_kg()+offset, DEC_POINT);
  
start:
  statemenu = 0;
  into();
  char keymenu = KP.Getkey();
  
  if (keymenu == '*') {//Loop Checking *
    statemenu = 1;
    openmenu();
    while (statemenu == 1) {
      Serial.println("Loop Checking");
      lcd.setCursor(0,0);       disdate();
      lcd.setCursor(0,1);       distime();
      lcd.setCursor(0,2);       dismany();
      lcd.setCursor(0,3);       disvolume();
      
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
ReEn:
    slectmenu = 1;
    statemenu = 1;
    openmenu();
    while (statemenu == 1) {
      Serial.println("Loop Setup");
      menu();
      delay(200);
      char fnmenu = KP.Getkey();
      
      if ((fnmenu == 'A')&&(slectmenu!=1)){   //UP
        slectmenu--;
      }
      if ((fnmenu == 'B')&&(slectmenu!=4)){   //Down
        slectmenu++; 
      }
      if (fnmenu == 'C') {//Enter Menu
        EnterMenu();
        goto ReEn;
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
