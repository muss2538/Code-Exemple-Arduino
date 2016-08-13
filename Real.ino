#include "HX711.h"
#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>


byte countarray = 0;
char datearray[] = "00000000";
char KEYS[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
float calibration_factor =99757.00; 
#define zero_factor 8573573
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  2

float offset=0;
float get_units_kg();

HX711 scale(DOUT, CLK);
RTC_DS1307 rtc;
LiquidCrystal_I2C lcd(0x3F, 20, 4);
OnewireKeypad <LiquidCrystal_I2C, 16> KP( lcd, KEYS, 4, 4, A1, 4700, 1000 );

float get_units_kg()
{
  return(scale.get_units()*0.453592);
}
/************************************************************************************************************************************/

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  lcd.begin();
  
  String test=datearray;
  Serial.print(test);
  
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);
}

void loop() {
  byte statemenu = 0;
  String data = String(get_units_kg()+offset, DEC_POINT);
  DateTime now = rtc.now();
  String timeread = "Time = "+String(now.hour())+":"+String(now.minute())+":"+String(now.second());
  String dateread = "Date = "+String(now.day())+"/"+String(now.month())+"/"+String(now.year());
  lcd.setCursor(0,1);
  lcd.print(timeread);
  lcd.setCursor(0,2);
  lcd.print(dateread);

  
  while(statemenu == 0){
  if(Serial.available()){
    lcd.clear();
    char key = Serial.read();
      switch(key){
        case 'A' :
          statemenu = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Menu");
          lcd.setCursor(0,1);
          lcd.print(">> Set time");
          lcd.setCursor(0,2);
          lcd.print("   Set many shrimp");
          break;
        case 'B' :
          statemenu = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Menu");
          lcd.setCursor(0,1);
          lcd.print("   Set time");
          lcd.setCursor(0,2);
          lcd.print(">> Set many shrimp");
          break;
        case 'C' :
          statemenu = 1;
          Serial.println("OK");
          break;      
        case 'D' :
          statemenu = 1;
          Serial.println("Exit");
          break;  
        default:
          if(statemenu == 1){
          }
          break;
      
    }
  }
  }
}
