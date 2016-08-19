#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
float calibration_factor =99231.00; 
#define zero_factor 8568028
#define DOUT  A3
#define CLK   A2
#define DEC_POINT  2


float offset=0;
float get_units_kg();
float data=0;

HX711 scale(DOUT, CLK);
int s1=8;
void setup() 
{
  Serial.begin(115200);
  Serial.println("Load Cell");
  lcd.begin(); 
  lcd.setCursor(0,0);
  lcd.print("...WEIGHTING....");
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);
  pinMode(s1,OUTPUT);  
}
void loop() 
{ 
  Serial.print("Reading: ");
  float CheckDataAll = (get_units_kg()+offset);
  String data = String(get_units_kg()+offset, DEC_POINT);

  Serial.print(data);
  Serial.println(" kg.");
  lcd.setCursor(4,1);
  lcd.print(data);
  lcd.setCursor(8,1);
  lcd.println(" kg.    ");
  
  if(CheckDataAll > 1.00)
  {
    digitalWrite(s1,HIGH);
  }
  else digitalWrite(s1,LOW);
}

float get_units_kg()
{
  return(scale.get_units()*0.453592);
}
