#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>

#define SolenoidAopen digitalWrite(10,1);
#define SolenoidAclose digitalWrite(10,0);
#define SolenoidBopen digitalWrite(9,1);
#define SolenoidBclose digitalWrite(9,0);
#define motorstart digitalWrite(8,1);
#define motorstop digitalWrite(8,0);

char KEYS[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
unsigned int ManyShrimp = 0; 
unsigned int Volume = 0;
unsigned int dday = 0;
unsigned int mmonth = 0;
unsigned int yyear = 0;
unsigned int ho = 0;
unsigned int mi = 0;
unsigned int se = 0;
int dataA;
String dataB;
int stac = 2;
int timearray[] = {1,8,5,0,3,0};
int manyarray[] = {1,0,0,0};
int volumearray[] = {5,0,0};
byte counttimearray = 0;
byte countmanyarray = 0;
byte countvolumearray = 0;
byte statemenu = 0;
byte slectmenu = 1;
byte i=0;

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
  delay(1500);lcd.clear();
}
void loadmenu() {
  delay(500);lcd.print(".");delay(500);lcd.print(".");delay(500);lcd.print(".");delay(500);
}
void into() {
  DateTime now = rtc.now();
  
  lcd.setCursor(0, 1);
  lcd.print("  Time = "); lcd.print(now.hour(), DEC); lcd.print(':'); lcd.print(now.minute(), DEC); lcd.print(':'); lcd.print(now.second(), DEC); lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print("  Date = "); lcd.print(now.day(), DEC); lcd.print('/'); lcd.print(now.month(), DEC); lcd.print('/'); lcd.print(now.year(), DEC);
  delay(500);
}
void disdate() {
  lcd.print("Date = "); lcd.print(dday, DEC); lcd.print('/'); lcd.print(mmonth, DEC); lcd.print('/'); lcd.print(yyear, DEC);
}
void distime() {
  ho =(timearray[0]*10)+timearray[1];
  mi =(timearray[2]*10)+timearray[3];
  se =(timearray[4]*10)+timearray[5];
  EEPROM.write(0,ho);EEPROM.write(1,mi);EEPROM.write(2,se);
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
  
  lcd.setCursor(0, 2);  disdate();
  lcd.setCursor(0, 0);  lcd.print("Saving Date");loadmenu();
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
  lcd.setCursor(0, 2);   distime();
  lcd.setCursor(0, 0);   lcd.print("Time Saving");loadmenu();
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
  lcd.setCursor(0, 2);   dismany();
  lcd.setCursor(0, 0);   lcd.print("Many Saving");loadmenu();
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
  lcd.setCursor(0, 2);   disvolume();
  lcd.setCursor(0, 1);  lcd.print("Volume Saving");loadmenu();
  i=0; countvolumearray=0;
  closemenu();
}
void EnterMenu() {
  if (slectmenu == 1) {MenuSetDate();}
  if (slectmenu == 2) {MenuSetTime();}
  if (slectmenu == 3) {MenuSetManyShrimp();}
  if (slectmenu == 4) {MenuSetVolume();}
}
void ActiveC() {
  DateTime now = rtc.now();
  if((now.hour() == ho) && (now.minute() == mi) && (now.second() == se)){stac = 0;}
    
    while(stac < 1 ){
      
      openmenu();
      SolenoidAopen
    
      dataB = String(get_units_kg()+offset, DEC_POINT);
      dataA = 1000*(dataB.toFloat());
      lcd.setCursor(0, 2);  lcd.print("Volume Set = "); lcd.print(Volume);
      lcd.setCursor(0, 1);  lcd.print(">>  Volume = "); lcd.print(dataA);
    
      if(dataA >= Volume){
        
        while(stac < 1 ){
        
          SolenoidAclose
          motorstart
          delay(1500);
          SolenoidBopen
      
          dataB = String(get_units_kg()+offset, DEC_POINT);
          dataA = 1000*(dataB.toFloat());
          
          if(dataA <= 20){
            
            delay(2000);
            SolenoidAclose
            SolenoidBclose
            motorstop
            delay(1000);
            
            stac = 2;
          }
        }
      }
    }
  //End Fn ActiveC
}
void StartOn() {
  ho = EEPROM.read(0);
  mi = EEPROM.read(1);
  se = EEPROM.read(2);
  
  dday = EEPROM.read(3);
  mmonth = EEPROM.read(4);
  yyear = EEPROM.read(0);  
  
  ManyShrimp = EEPROM.read(3); 
  Volume = EEPROM.read(0);
}
/*****************************************************************************************************************************/
void setup() {
  lcd.begin();Wire.begin();rtc.begin();
  lcd.setCursor(0, 0);  lcd.print("Deivce Power ON");
  lcd.setCursor(3, 2);  lcd.print("Load Setup ");loadmeu();
  closemenu();
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);
  
  pinMode(8,OUTPUT);SolenoidAclose
  pinMode(9,OUTPUT);SolenoidBclose
  pinMode(10,OUTPUT);motorstop
  
  StartOn();
}
void loop() {
start:
  statemenu = 0;
  into();
  char keymenu = KP.Getkey();
  ActiveC();
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
        goto start;}
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
      menu();
      delay(200);
      char fnmenu = KP.Getkey();
      if ((fnmenu == 'A')&&(slectmenu!=1)){   //UP
        slectmenu--;}
      if ((fnmenu == 'B')&&(slectmenu!=4)){   //Down
        slectmenu++;}
      if (fnmenu == 'C') {//Enter Menu
        EnterMenu();
        goto ReEn;}        
      if (fnmenu == 'D') {
        lcd.clear();  //Exit Menu
        goto start;}
      statemenu = 1;
    }
  //End Loop Setup
  }
//END
}
