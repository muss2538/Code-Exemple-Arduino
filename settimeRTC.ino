
#include <Wire.h>
#include "RTClib.h"


RTC_DS1307 rtc;    // Create a RealTimeClock object


void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  Serial.println("YourDuino.com DS1307 Real Time Clock - Set / Run Utility");

  /*----( These lines allow code to work also on Arduino DUE )----- */
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif

  rtc.begin(); // Start the RTC library code

  /*----( SET the date and time.  Comment OUT these lines after setting )----*/

  rtc.adjust(DateTime(2016, 8, 4, 19, 2, 0));
     
  // This line sets the RTC with an explicit date & time, for example to set
  // May 21, 2015 at 6pm you would call: (use 24 hour time)
  //    rtc.adjust(DateTime(2015, 5, 21, 18, 0, 0));

}


void loop(){
  DateTime now = rtc.now();  // Read data from the RTC Chip

  Serial.print(now.year(), DEC); //
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(5000);

}

