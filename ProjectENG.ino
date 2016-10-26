#include <DFPlayer_Mini_Mp3.h>
#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(6, 5, 4 ,7); //Enable, RW, RS, RESET


int ex=1;

int ReadDataChoice(){
  int reada2 = analogRead(A2);
  if((reada2 >= 565)&&(reada2 <= 568))return 1;
  if((reada2 >= 742)&&(reada2 <= 745))return 2;
  if((reada2 >= 835)&&(reada2 <= 838))return 3;
  if((reada2 >= 907)&&(reada2 <= 912))return 4;
}
char ReadDataControl(){
  int reada3 = analogRead(A3);
  if((reada3 >= 565)&&(reada3 <= 568))return 'N';
  if((reada3 >= 908)&&(reada3 <= 911))return 'B';
  if((reada3 >= 835)&&(reada3 <= 838))return 'E';
  if((reada3 >= 743)&&(reada3 <= 746))return 'X';
}

void Exsample() {

}
void Ex1() {

}
void Ex2() {

}
void Ex3() {

}
void Ex4() {

}
void Summit(int ANS) {

}
void setup() {
  Serial.begin(9600);
  mp3_set_serial(Serial); //set softwareSerial for DFPlayer-mini mp3 module
  mp3_set_volume(15);

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  for(int pind=8;pind<=15;pind++){
    pinMode(pind,OUTPUT);}

  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
   u8g.setColorIndex(255);   // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
   u8g.setColorIndex(1);     // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
   u8g.setColorIndex(1);     // pixel on

}

void loop(){
  switch(ReadDataControl()){
    case 'N' :
      if(ex<=4)ex++;
      break;
    case 'B' :
      if(ex>=1)ex--;
      break;
    case 'E' :      
      if(ex==1) Ex1();
      if(ex==2) Ex2();
      if(ex==3) Ex3();
      if(ex==4) Ex4();
      break;
    case 'X' :      ex=1;      break;
  }


    mp3_play (1);
    mp3_stop ();


}
