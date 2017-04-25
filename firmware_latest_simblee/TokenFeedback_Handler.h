#ifndef TokenFeedback_h
#define TokenFeedback_h

#include "Haptic.h"
#include "RGB_LED.h"

class TokenFeedback_Handler
{
    public:
      TokenFeedback_Handler();   //default constructor

      String UpdateFeedback();
      void HandleTime(unsigned int ElapsedTime);

      // Haptic
      void setHapticMotor(Haptic *pHapticMotor);
      void Vibrate(unsigned int Time);
      void Vibrate(String Type);
      void VibrateCustom(String code);

      // RGB Led
      void setRGB_LED(RGB_LED *pLED);
      void setColor(String color);

    private:
      Haptic *HapticMotor;
      bool HapticAvailable;
      RGB_LED *LED;
      bool RGB_LEDAvailable;
      void parseColorString(String color, int& red, int& green, int& blue);

};

#endif
