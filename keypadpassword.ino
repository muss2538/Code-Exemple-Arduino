// PIN switch with 16-digit numeric keypad
// http://tronixstuff.com/tutorials > chapter 42
 
#include "Keypad.h"
#include <Wire.h>
#include <TWILiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] =
{
  {
    '1','2','3','A'  }
  ,
  {
    '4','5','6','B'  }
  ,
  {
    '7','8','9','C'  }
  ,
  {
    '*','0','#','D'  }
};
byte rowPins[ROWS] = {
  5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  9, 8, 7, 6}; //connect to the column pinouts of the keypad
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
char PIN[6]={
  '1','2','A','D','5','6'}; // our secret (!) number
char attempt[6]={ 
  '0','0','0','0','0','0'}; // used for comparison
int z=0;
 
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("PIN Lock ");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}
 
void correctPIN() // do this if correct PIN entered
{
  lcd.print("* Correct PIN *");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}
 
void incorrectPIN() // do this if incorrect PIN entered
{
  lcd.print(" * Try again *");
  delay(1000);
  lcd.clear();
  lcd.print("  Enter PIN...");
}
 
void checkPIN()
{
  int correct=0;
  int i;
  for ( i = 0;   i < 6 ;  i++ )
  {
 
    if (attempt[i]==PIN[i])
    {
      correct++;
    }
  }
  if (correct==6)
  {
    correctPIN();
  } 
  else
  {
    incorrectPIN();
  }
 
  for (int zz=0; zz<6; zz++) 
  {
    attempt[zz]='0';
  }
}
 
void readKeypad()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    attempt[z]=key;
    z++;
    switch(key)
    {
    case '*':
      z=0;
      break;
    case '#':
      z=0;
      delay(100); // for extra debounce
      lcd.clear();
      checkPIN();
      break;
    }
  }
}
 
void loop()
{
  readKeypad();
}
