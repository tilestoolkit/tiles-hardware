#ifndef NEO_STRIP_h
#define NEO_STRIP_h

#include <Arduino.h>
#include "FastLED.h"

#define NEO_PIN 2
#define NUM_LEDS 7      //Number of leds in the neopixel strip


class NEO_STRIP
{
    public:
        NEO_STRIP();
        void setColor(String color);
    private:
        bool state;
        CRGB leds;
};

#endif