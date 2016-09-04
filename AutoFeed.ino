#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[4] = {8, 7, 6, 5};
byte colPins[4] = {4, 3, 2, 13};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

#define pwml 180
#define SolenoidAopen digitalWrite(9,1);
#define SolenoidAclose digitalWrite(9,0);
#define SolenoidBopen digitalWrite(10,1);
#define SolenoidBclose digitalWrite(10,0);

unsigned int ManyShrimp = 0, Volume = 0,coutweek = 0, coutday = 0, dday = 0, mmonth = 0, yyear = 0, ho = 0, mi = 0, se = 0;
unsigned int dataA,VolumeA;
unsigned int ilcd = 0;
String dataB;
int timearray[] = {1,8,5,0,3,0};
int manyarray[] = {1,0,0,0};
byte counttimearray = 0, countmanyarray = 0, statemenu = 2, slectmenu = 1, i=0, stac1 = 2, stac2 = 2;

float calibration_factor =99755.00; 
#define zero_factor 8563755
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  2
float offset=0;                 float get_units_kg();
HX711 scale(DOUT, CLK);
LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTC_DS1307 rtc;
float get_units_kg() {
  return(scale.get_units()*0.453592);}
void StartOn() {
  ho = EEPROM.read(0);  mi = EEPROM.read(1);  se = EEPROM.read(2);
  dday = EEPROM.read(3);  mmonth = EEPROM.read(4);    yyear |= EEPROM.read(5)<<8;  yyear |= EEPROM.read(6)&0xFF;
  ManyShrimp |= EEPROM.read(7)<<8;  ManyShrimp |= EEPROM.read(8)&0xFF;
  Volume |= EEPROM.read(9)<<8; Volume |= EEPROM.read(10)&0xFF;
  coutday |= EEPROM.read(11)<<8; coutday |= EEPROM.read(12)&0xFF;
  coutweek |= EEPROM.read(13)<<8; coutweek |= EEPROM.read(14)&0xFF;
}
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
  delay(500);}
void ewdate() {
  EEPROM.write(3,dday);  EEPROM.write(4,mmonth);  EEPROM.write(5,yyear>>8);  EEPROM.write(6,yyear&0xFF);
  coutday=0;EEPROM.write(11,coutday>>8);  EEPROM.write(12,coutday&0xFF);
  coutweek=0;EEPROM.write(13,coutday>>8);  EEPROM.write(14,coutday&0xFF);}
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
    char keydata = keypad.getKey();
    byte  ckeydata = keydata-'0';
    if (keydata != NO_KEY){
      lcd.setCursor(i, 2);    lcd.print(ckeydata);
      timearray[counttimearray] = ckeydata;
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
    char keydata1 = keypad.getKey();
    byte  ckeydata1 = keydata1-'0';
    if (keydata1 != NO_KEY){
      lcd.setCursor(i, 2);    lcd.print(keydata1);
      manyarray[countmanyarray] = ckeydata1;
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
    coutday++;
    if(coutday==7) {
      coutweek++;coutday=0;
      EEPROM.write(13,coutweek>>8);  EEPROM.write(14,coutweek&0xFF);}
    EEPROM.write(11,coutday>>8);  EEPROM.write(12,coutday&0xFF);
    Volume=(1+coutweek)*0.1*ManyShrimp;
    ewvol();}
  if((now.hour() == ho) && (now.minute() == mi) && (now.second() == se)){
    stac1 = 0;    lcd.backlight();    openmenu();      VolumeA = Volume; 
    lcd.setCursor(0, 0);  lcd.print("Week      = ");lcd.print(coutweek);
    lcd.setCursor(0, 1);  lcd.print("Many      = ");lcd.print(ManyShrimp);
    lcd.setCursor(0, 2);  lcd.print("Volume    =      g. ");
    lcd.setCursor(0, 3);  lcd.print("WEIGHTING =      g. ");

    while(VolumeA>=501){
      stac1=0;
      while(stac1 < 1 ){
      Serial.println("loop1");
      SolenoidAopen
      dataB = String(get_units_kg()+offset, DEC_POINT);
      dataA = 1000*(dataB.toFloat());
      lcd.setCursor(12, 2);  lcd.print(VolumeA);
      lcd.setCursor(12, 3);  lcd.print(dataA);lcd.print("  ");
      if(dataA >= 500){stac2 = 0;}
        while(stac2 < 1 ){
          lcd.setCursor(12, 2);  lcd.print(VolumeA);
          lcd.setCursor(12, 3);  lcd.print(dataA, DEC);lcd.print("  ");
          SolenoidAclose
          analogWrite(11,pwml);delay(250);
          SolenoidBopen
          analogWrite(11,255);delay(250);
          SolenoidBclose
          delay(250);
          dataB = String(get_units_kg()+offset, DEC_POINT);
          dataA = 1000*(dataB.toFloat());
          if(dataA <= 10){
            delay(2000);
            SolenoidAclose
            SolenoidBclose
            analogWrite(11,0);
            stac1 = 2;ilcd=0;stac2 = 2;
            VolumeA=(VolumeA-500);}}}
    }
    stac1 = 0;
    if(VolumeA<=500){
    while(stac1 < 1 ){
      Serial.println("loop2");
      SolenoidAopen
      dataB = String(get_units_kg()+offset, DEC_POINT);
      dataA = 1000*(dataB.toFloat());
      lcd.setCursor(12, 2);  lcd.print(VolumeA);
      lcd.setCursor(12, 3);  lcd.print(dataA);
      if(dataA >= VolumeA){stac2 = 0;}
        while(stac2 < 1 ){
          lcd.setCursor(12, 2);  lcd.print(VolumeA);
          lcd.setCursor(12, 3);  lcd.print(dataA, DEC);lcd.print("  ");
          SolenoidAclose
          analogWrite(11,pwml);delay(250);
          SolenoidBopen
          analogWrite(11,255);delay(250);
          SolenoidBclose
          delay(250);
          dataB = String(get_units_kg()+offset, DEC_POINT);
          dataA = 1000*(dataB.toFloat());
          if(dataA <= 5){
            delay(2000);
            SolenoidAclose
            SolenoidBclose
            analogWrite(11,0);
            closemenu();
            stac2 = 2;stac1 = 2;ilcd=0;}}}}
  }}
void setup() {
  Serial.begin(9600);
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
  if(ilcd<=30){ilcd++;}
  if(ilcd==31){lcd.noBacklight();}
  into();
  char keymenu = keypad.getKey();
  ActiveC();
    if (keymenu == '*') {//Loop Checking *
    lcd.backlight();    statemenu = 0;    openmenu();
    while (statemenu < 1) {
      lcd.setCursor(0,0);       disdate();
      lcd.setCursor(0,1);       distime();
      lcd.setCursor(0,2);       dismany(); lcd.setCursor(12,2);lcd.print("Week= "); lcd.print(coutweek);
      lcd.setCursor(0,3);       disvolume();
      char fnmenu = keypad.getKey();
      if (fnmenu == 'D') {
        lcd.clear();ilcd=0;  //Exit Menu
        statemenu = 2;}
      }}
  if (keymenu == '#') {//Loop Setup #
    slectmenu = 1;
ReEn:
    lcd.backlight();
    statemenu = 0;
    openmenu();
    while (statemenu < 1) {
      menu();      delay(150);
      char fnmenu = keypad.getKey();
      if ((fnmenu == 'A')&&(slectmenu!=1)){   //UP
        slectmenu--;}
      if ((fnmenu == 'B')&&(slectmenu!=3)){   //Down
        slectmenu++;}
      if (fnmenu == 'C') {//Enter Menu
        EnterMenu();
        goto ReEn;}        
      if (fnmenu == 'D') {
        lcd.clear();ilcd=0;  //Exit Menu
        statemenu = 2;}
      }}  
}
