#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OnewireKeypad.h>

char KEYS[]= {
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D'
};

LiquidCrystal_I2C Lcd(0x3F,20,4); 
OnewireKeypad <LiquidCrystal_I2C, 16> KP( Lcd, KEYS, 4, 4, A1, 4700, 1000 );
//OnewireKeypad <Print, 12 > KP2(Serial, KEYS, 4, 3, A0, 4700, 1000 );

int countarray = 0;
char datearray[] = "00000000";

void setup (){
  Serial.begin(9600);
  Lcd.begin();
  Lcd.backlight(); //tune off led
}

void loop(){

  if (KP.Getkey() != NO_KEY){
    Lcd.print(KP.Getkey());
    datearray[countarray] = KP.Getkey();
    countarray++;
    delay(250);
    
    if (countarray == 8){
      Lcd.clear();
      countarray=0;}
}
}
