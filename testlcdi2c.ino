#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);


void setup() {
  Serial.begin(9600);
  lcd.begin();

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("test");
  delay(1000);


}
