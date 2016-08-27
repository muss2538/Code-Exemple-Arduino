#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>
#define pwml 180
#define SolenoidAopen digitalWrite(9,1);
#define SolenoidAclose digitalWrite(9,0);
#define SolenoidBopen digitalWrite(10,1);
#define SolenoidBclose digitalWrite(10,0);
char KEYS[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
unsigned int ManyShrimp = 0; 
unsigned int Volume = 0;
unsigned int coutday = 0;
unsigned int dday = 0;
unsigned int mmonth = 0;
unsigned int yyear = 0;
unsigned int ho = 0;
unsigned int mi = 0;
unsigned int se = 0;
unsigned int dataA;
unsigned int ilcd = 0;
String dataB;
int timearray[] = {1,8,5,0,3,0};
int manyarray[] = {1,0,0,0};
byte counttimearray = 0;  byte countmanyarray = 0;  byte statemenu = 0; 
byte slectmenu = 1; byte i=0; byte stac1 = 2;  byte stac2 = 2;
float calibration_factor =100456.00; 
#define zero_factor 8567295
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  2
float offset=0;                 float get_units_kg();
HX711 scale(DOUT, CLK);
LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTC_DS1307 rtc;
OnewireKeypad <LiquidCrystal_I2C, 16> KP( lcd, KEYS, 4, 4, A1, 4700, 1000 );
float get_units_kg() {
  return(scale.get_units()*0.453592);}
void StartOn() {
  ho = EEPROM.read(0);  mi = EEPROM.read(1);  se = EEPROM.read(2);
  dday = EEPROM.read(3);  mmonth = EEPROM.read(4);    yyear |= EEPROM.read(5)<<8;  yyear |= EEPROM.read(6)&0xFF;
  ManyShrimp |= EEPROM.read(7)<<8;  ManyShrimp |= EEPROM.read(8)&0xFF;
  Volume |= EEPROM.read(9)<<8; Volume |= EEPROM.read(10)&0xFF;
  coutday |= EEPROM.read(11)<<8; coutday |= EEPROM.read(12)&0xFF;}
void openmenu() {
  lcd.clear(); delay(1000);}
void closemenu() {
  delay(1500);lcd.clear();}
void loadmenu() {
  delay(500);lcd.print(".");delay(500);lcd.print(".");delay(500);lcd.print(".");delay(500);}
void into() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 1);
  lcd.print("  Time = "); lcd.print(now.hour(), DEC); lcd.print(':'); lcd.print(now.minute(), DEC); lcd.print(':'); lcd.print(now.second(), DEC); lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print("  Date = "); lcd.print(now.day(), DEC); lcd.print('/'); lcd.print(now.month(), DEC); lcd.print('/'); lcd.print(now.year(), DEC);
  delay(1000);}
void ewdate() {
  EEPROM.write(3,dday);  EEPROM.write(4,mmonth);  EEPROM.write(5,yyear>>8);  EEPROM.write(6,yyear&0xFF);
  coutday=0;EEPROM.write(11,coutday>>8);  EEPROM.write(12,coutday&0xFF);}
void ewtime() {
  EEPROM.write(0,ho);  EEPROM.write(1,mi);  EEPROM.write(2,se);}
void ewmany() {
  EEPROM.write(7,ManyShrimp>>8);  EEPROM.write(8,ManyShrimp&0xFF);}
void ewvol() {
  EEPROM.write(9,Volume>>8);  EEPROM.write(10,Volume&0xFF);}
void datatime() {
  ho =(timearray[0]*10)+timearray[1];
  mi =(timearray[2]*10)+timearray[3];
  se =(timearray[4]*10)+timearray[5];}
void datamany() {
  ManyShrimp =((manyarray[0]*1000)+(manyarray[1]*100)+(manyarray[2]*10)+manyarray[3]);}
void disdate() {
  lcd.print("Date = "); lcd.print(dday, DEC); lcd.print('/'); lcd.print(mmonth, DEC); lcd.print('/'); lcd.print(yyear, DEC);}
void distime() {
  lcd.print("Time = ");lcd.print(ho);lcd.print(":");lcd.print(mi);lcd.print(":");lcd.print(se);}
void dismany() {
  lcd.print("Many = ");lcd.print(ManyShrimp);}
void disvolume() {
  lcd.print("Vol. = ");lcd.print(Volume);lcd.print(" Gram");}
void menu() {
  if (slectmenu == 1) {
    lcd.setCursor(0, 0);    lcd.print(">> Set Date");
    lcd.setCursor(0, 1);    lcd.print("   Set Time");
    lcd.setCursor(0, 2);    lcd.print("   Set Many Shrimp");}
  if (slectmenu == 2) {
    lcd.setCursor(0, 0);    lcd.print("   Set Date");
    lcd.setCursor(0, 1);    lcd.print(">> Set Time");
    lcd.setCursor(0, 2);    lcd.print("   Set Many Shrimp");}
  if (slectmenu == 3) {
    lcd.setCursor(0, 0);    lcd.print("   Set Date");
    lcd.setCursor(0, 1);    lcd.print("   Set Time");
    lcd.setCursor(0, 2);    lcd.print(">> Set Many Shrimp");}
}
void MenuSetDate() {
  openmenu();
  DateTime now = rtc.now();
  dday=now.day();  mmonth=now.month();  yyear=now.year();
  lcd.setCursor(0, 2);  disdate();
  lcd.setCursor(0, 0);  lcd.print("Saving Date");loadmenu();  ewdate();  closemenu();}
void MenuSetTime() {
  openmenu();
  while(counttimearray < 6) {
    lcd.setCursor(0, 0);      lcd.print("***Set Time***");
    lcd.setCursor(0, 1);      lcd.print("Form HH:MM:SS");
    if (KP.Getkey() != NO_KEY){
      lcd.setCursor(i, 2);    lcd.print(KP.Getkey());
      timearray[counttimearray] = KP.Getkey()-'0';
      counttimearray++;i++;
      delay(250);}
  }
  openmenu();  datatime();  ewtime();
  lcd.setCursor(0, 2);   distime();
  lcd.setCursor(0, 0);   lcd.print("Time Saving");loadmenu();
  i=0; counttimearray=0;  closemenu();}
void MenuSetManyShrimp() {
  openmenu();
  lcd.setCursor(0, 0);    lcd.print("***Many Shrimp***");
  while(countmanyarray < 4) {
    if (KP.Getkey() != NO_KEY){
      lcd.setCursor(i, 2);    lcd.print(KP.Getkey());
      manyarray[countmanyarray] = KP.Getkey()-'0';
      countmanyarray++;i++;
      delay(250);}  
  }
  openmenu(); datamany(); ewmany();
  Volume=((1000*coutday)+(7*ManyShrimp))/70;
  ewvol();
  lcd.setCursor(0, 2);   dismany();
  lcd.setCursor(0, 0);   lcd.print("Many Saving");loadmenu();
  i=0; countmanyarray=0;  closemenu();}
void EnterMenu() {
  if (slectmenu == 1) {MenuSetDate();}
  if (slectmenu == 2) {MenuSetTime();}
  if (slectmenu == 3) {MenuSetManyShrimp();}}
void ActiveC() {
  DateTime now = rtc.now();
  if((now.hour() == 00) && (now.minute() == 00) && (now.second() == 00)){
    coutday++;EEPROM.write(11,coutday>>8);  EEPROM.write(12,coutday&0xFF);
    Volume=((1000*coutday)+(7*ManyShrimp))/70;
    ewvol();
  }
  if((now.hour() == ho) && (now.minute() == mi) && (now.second() == se)){
    stac1 = 0;    lcd.backlight();    openmenu();
    lcd.setCursor(0, 0);  lcd.print("Day       =");lcd.print(coutday);
    lcd.setCursor(0, 1);  lcd.print("Many      = ");lcd.print(ManyShrimp);
    lcd.setCursor(0, 2);  lcd.print("Volume    =      g. ");
    lcd.setCursor(0, 3);  lcd.print("WEIGHTING =      g. ");}
    while(stac1 < 1 ){
      SolenoidAopen
      dataB = String(get_units_kg()+offset, DEC_POINT);
      dataA = 1000*(dataB.toFloat());
      lcd.setCursor(12, 2);  lcd.print(Volume);
      lcd.setCursor(12, 3);  lcd.print(dataA);
      if(dataA >= Volume){stac2 = 0;}
        while(stac2 < 1 ){
          lcd.setCursor(12, 2);  lcd.print(Volume);
          lcd.setCursor(12, 3);  lcd.print(dataA, DEC);
          SolenoidAclose
          SolenoidBopen
          analogWrite(11,pwml);delay(500);analogWrite(11,255);delay(500);
          SolenoidBclose
          dataB = String(get_units_kg()+offset, DEC_POINT);
          dataA = 1000*(dataB.toFloat());
          if(dataA <= 5){
            delay(2000);
            SolenoidAclose
            SolenoidBclose
            analogWrite(11,0);
            closemenu();
            stac2 = 2;stac1 = 2;ilcd=0;}}}}
void setup() {
  lcd.begin();Wire.begin();rtc.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);  lcd.print("Deivce Power ON");
  lcd.setCursor(3, 2);  lcd.print("Load Setup ");loadmenu();
  closemenu();
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);
  pinMode(9,OUTPUT);  pinMode(10,OUTPUT);
  SolenoidAclose
  SolenoidBclose
  StartOn();}
void loop() {
start:
  if(ilcd<=30){ilcd++;}
  if(ilcd==31){lcd.noBacklight();}
  statemenu = 0;  into();
  char keymenu = KP.Getkey();
  ActiveC();
  if (keymenu == '*') {//Loop Checking *
    lcd.backlight();
    statemenu = 1;    openmenu();
    while (statemenu == 1) {
      Serial.println("Loop Checking");
      lcd.setCursor(0,0);       disdate();
      lcd.setCursor(0,1);       distime();
      lcd.setCursor(0,2);       dismany(); lcd.setCursor(11,2);lcd.print("Day = "); lcd.print(coutday);
      lcd.setCursor(0,3);       disvolume();
      char fnmenu = KP.Getkey();
      if (fnmenu == 'D') {
        lcd.clear();ilcd=0;  //Exit Menu
        goto start;}
      statemenu = 1;}}
  if (keymenu == '#') {//Loop Setup #
    slectmenu = 1;
ReEn:
    lcd.backlight();
    statemenu = 1;
    openmenu();
    while (statemenu == 1) {
      menu();
      delay(200);
      char fnmenu = KP.Getkey();
      if ((fnmenu == 'A')&&(slectmenu!=1)){   //UP
        slectmenu--;}
      if ((fnmenu == 'B')&&(slectmenu!=3)){   //Down
        slectmenu++;}
      if (fnmenu == 'C') {//Enter Menu
        EnterMenu();
        goto ReEn;}        
      if (fnmenu == 'D') {
        lcd.clear();ilcd=0;  //Exit Menu
        goto start;}
      statemenu = 1;}}
}
