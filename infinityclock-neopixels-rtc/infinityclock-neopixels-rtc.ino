//------------------------------------------------------------------------------
// Neopixel 60 light LED strip clock
// dan@marginallycelver.com 2011-06-21
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/Neopixel_clock for more information.

//------------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------------
// Serial communication bitrate
const long BAUD        = 9600;
// Maximum length of serial input message.
const int MAX_BUF      = 64;

//const long ONE_DAY     = 24 * 60 * 60 * 1000;

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <RTClib.h>

//------------------------------------------------------------------------------
// globals
//------------------------------------------------------------------------------
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 2, NEO_GRB + NEO_KHZ800);
//RTC_Millis rtc; //defines the DS1307 as "rtc"
RTC_DS1307 rtc; //defines the DS1307 as "rtc"

#define hours 48 //24 or 12?

char buffer[MAX_BUF];
int sofar;

long start_seconds;
long start_time;

double brightnessmod = 1;
uint8_t millicount;
uint8_t oldSecond;

//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------


void setup() {
  // open communications
  Serial.begin(BAUD);

  
  //RTC - Vom Beispiel einfach übernommen
  #ifdef AVR
   Wire.begin();
  #else
   Wire1.begin()
  #endif
   rtc.begin();
  
  //rtc.adjust(DateTime(__DATE__, __TIME__)); //Sets the RTC clock time. Uncomment if you want to set the time, but dont forget to comment it in again afterwards!!
  //EXAMPLE TIME: rtc.adjust(DateTime(2015, 2, 4, 10, 51, 0));

  // start the strip  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop() {
  DateTime now = rtc.now(); //Sets the date and time using the rtc
  // get the seconds, minutes, and hours.
  int s = now.second();
  int m = now.minute();
  int h = now.hour();
  
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  /*if (h > 21 || h < 8) brightnessmod = 0.15;
  else brightnessmod = 0.8;*/
  
  if (h >= 12) h -= 12;

  // make sure the elapsed_seconds doesn't overflow.
  /*if( elapsed_seconds > ONE_DAY ) {
    // more than a day?
    start_time -= ONE_DAY;
  }*/
  
  // display the clock
  int a,b,c;
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    a = ( i == s ? 255 : 0 );
    b = ( i == m ? 255 : 0 );
    //c = ( floor((float)i/2.5) == h ? 255 : 0 );
    c = (i == h*5 ? 255 : 0 );
    if (c != 255) {
      if (i == h*5+1) c = 100;
      else if (h*5-1 < 0) {if (i == 59) c = 100;}
      else if (i == h*5-1) c = 100;
      else c = 0;
    }


    if( a==0 && b==0 && c==0 ) { //Special orange dots at 12, 3, 6, 9
      if( i == 0 || i == 15 || i == 30 || i == 45 ) {a=10; b=7; c=0;}
    }
    
    if( a==0 && b==0 && c==0 ) { //Sets white dots
      if( (i%5)==0 ) a=b=c=(3);
      if( i==0 ) a=b=c=(3);
    }
    
    
    if (b == 255 && c >= 100) c = 0;

    //Milliseconds: move one every 17 ms.
    if (oldSecond != s) {oldSecond = s; millicount = 0;} //millicount
    if (i == millicount) {a = (a > 40)? a : 28; /*b = (b > 40)? b : 40;*/}
    //if (i == millicount-1) {a = (a > 30)? a : 30; c = (c > 30)? c : 30;}

    if (a < 10) a *= brightnessmod;
    if (b < 10) b *= brightnessmod;
    if (c < 10) c *= brightnessmod;
    
    
    // this way we combine the colors when they overlap.
    strip.setPixelColor(i, strip.Color(a, b, c));
  }
  strip.show();

  millicount = (millicount > 58)? 59 : millicount+1;
  delay(12);
}



/**
* This file is part of Neopixel_clock.
*
* Neopixel_clock is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Neopixel_clock is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Neopixel_clock. If not, see <http://www.gnu.org/licenses/>.
*/
