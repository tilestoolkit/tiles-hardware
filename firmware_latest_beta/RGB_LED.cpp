#include "RGB_LED.h"

RGB_LED::RGB_LED(int R, int G, int B)
{
    _R_LED = R;
    _G_LED = G;
    _B_LED = B;

    pinMode(_R_LED, OUTPUT);
    pinMode(_G_LED, OUTPUT);
    pinMode(_B_LED, OUTPUT);

    digitalWrite(_R_LED, LED_OFF);
    digitalWrite(_G_LED, LED_OFF);
    digitalWrite(_B_LED, LED_OFF);

    State = LED_OFF;
}

void RGB_LED::setRGBaColor(uint8_t R, uint8_t G, uint8_t B, float a)
{
    #if LED_TYPE == COMMON_ANODE
      R = (255 - R);
      G = (255 - G);
      B = (255 - B);
    #endif
    
    analogWrite(_R_LED, R);
    analogWrite(_G_LED, G);
    analogWrite(_B_LED, B);
}


void RGB_LED::HandleTime(unsigned int ElapsedTime)
{
    RGB_LED_Timing += ElapsedTime;
}

void RGB_LED::RefreshValues()
{
    
}

