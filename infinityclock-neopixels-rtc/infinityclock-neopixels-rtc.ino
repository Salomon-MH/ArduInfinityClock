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
RTC_Millis rtc; //defines the DS1307 as "rtc"

#define hours 48 //24 or 12?

char buffer[MAX_BUF];
int sofar;

long start_seconds;
long start_time;

//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------


void setup() {
  // open communications
  Serial.begin(BAUD);
  
  rtc.adjust(DateTime(__DATE__, __TIME__)); //Sets or gets the RTC clock time

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
    
    if( a==0 && b==0 && c==0 ) { //Sets white dots
      if( (i%5)==0 ) a=b=c=(strip.numPixels()/24)*(i/5);
      if( i==0 ) a=b=c=(strip.numPixels()/24);
    }
    
    // this way we combine the colors when they overlap.
    strip.setPixelColor(i, strip.Color(a, b, c));
  }
  strip.show();
  delay(10);
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
