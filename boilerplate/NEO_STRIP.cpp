#include "NEO_STRIP.h"

NEO_STRIP::NEO_STRIP()
{
    CRGB leds[7];
    FastLED.addLeds<NEOPIXEL, NEO_PIN>(leds, NUM_LEDS);
    LEDS.setBrightness(100); //0 to 128
   
      for (int i = 0; i < NUM_LEDS; i++)
     {
          leds[i] = CRGB::Red;
     }
     FastLED.show();
}

 void NEO_STRIP::setColor(String color)
 {
    if (color == "off"){
    for (int i = 0; i < NUM_LEDS; i++)
    {
         leds[i] = CRGB::Black;
    }
    FastLED.show();   } 
    else if (color == "red") {
    for (int i = 0; i < NUM_LEDS; i++)
    {
         leds[i] = CRGB::Red;
    }
    FastLED.show();    }
    else if (color == "green"){
    for (int i = 0; i < NUM_LEDS; i++)
    {
         leds[i] = CRGB::Green;
    }
    FastLED.show();   } 
    else if (color == "blue"){
    for (int i = 0; i < NUM_LEDS; i++)
    {
         leds[i] = CRGB::Blue;
    }
    FastLED.show();    }
    else if (color == "white") {
    for (int i = 0; i < NUM_LEDS; i++)
    {
         leds[i] = CRGB::White;
    }
    FastLED.show(); }
 }