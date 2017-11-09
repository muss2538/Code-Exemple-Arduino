#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>

#define SoilPIN A0
#define Pump 12
#define Springer 13
#define Button 8
#define LOWPIN 2
#define HIGHPIN 3
#define PersentMIN 20
#define PersentMAX 60
#define ON HIGH
#define OFF LOW

bool Springerstatus = 0, statusBT = 0, Pumpstatus = 0;
int SoilValue = 0, PersentSoil = 0, timeHo = 0;
int PINx2 =0,PINx3=0;

LiquidCrystal_I2C lcd(0x3F, 20, 4); //A4 = SDA    A5 = SCL
DS1302 rtc(9, 10, 11);              //RST DAT CLK
Time t;
void setup(){
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  pinMode(Pump,OUTPUT);
  pinMode(Springer,OUTPUT);
  pinMode(Button,INPUT_PULLUP);
  pinMode(LOWPIN,INPUT);
  pinMode(HIGHPIN,INPUT);

  
  rtc.halt(false);
  rtc.writeProtect(false);
  //หากต้องการตั้งเวลาให้ลบคอมเมนต์ออก
  //rtc.setDOW(5);        // Set Day-of-Week to FRIDAY
  //rtc.setTime(12, 58, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(28, 10, 2017);   // Set the date to August 6th, 2010
  IMNTECH();
}
void loop(){
  t = rtc.getTime();
  timeHo = t.hour;
  ReadSensor();
  PumpAuto();
  Present();
  Manual();  Automatic();
  
  delay(100);
}
void IMNTECH(){
  lcd.setCursor(0, 0);  lcd.print("  Welcome Moisture  ");
  lcd.setCursor(0, 1);  lcd.print("       N.Tech       ");
  lcd.setCursor(0, 2);  lcd.print("     ELECTRONIC     ");delay(5000);  lcd.clear();
  lcd.setCursor(0, 0);  lcd.print("##watering machine##");
}
void Present(){
  lcd.setCursor(0, 1);  lcd.print("DATE :  ");lcd.print(rtc.getDateStr());
  lcd.setCursor(0, 2);  lcd.print("TIME :  ");lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 3);  lcd.print("MOISTURE :  ");lcd.print(PersentSoil);lcd.print(" %  ");  
}
void ReadSensor(){
  PINx2 = digitalRead(LOWPIN);
  PINx3 = digitalRead(HIGHPIN);
  SoilValue = analogRead(SoilPIN);
  PersentSoil = map(SoilValue,0,1023,100,0);
}
void Manual(){
  if (digitalRead(Button) == 0) { 
    statusBT = !statusBT; 
    digitalWrite(Springer, statusBT); 
  }while(digitalRead(Button) == 0); 
  delay(50);
}
void Automatic(){
  if(statusBT==0){
    if((timeHo<=18)&&(timeHo>=7)){
      if(PersentSoil<=PersentMIN){
        while(PersentSoil<PersentMAX){      //รดน้ำจนกว่าความชื้นจะถึงค่า MAX
          //เหลือวัดระดับน้ำ
          digitalWrite(Springer, ON);
          ReadSensor();
          Present();
          Manual();
          PumpAuto();
        }        
      }else{digitalWrite(Springer, OFF);}
    }    
  }
}
void PumpAuto(){
  if((PINx2==0)&&(PINx3==0)){
    digitalWrite(Pump, ON);
    Pumpstatus = 1;
  }
  if((PINx2==1)&&(PINx3==1)){
    digitalWrite(Pump, OFF);
    Pumpstatus = 0;
  }
  if((PINx2==1)&&(PINx3==0)&&(Pumpstatus==1)){digitalWrite(Pump, ON);}
  if((PINx2==1)&&(PINx3==0)&&(Pumpstatus==0)){digitalWrite(Pump, OFF);}
}

