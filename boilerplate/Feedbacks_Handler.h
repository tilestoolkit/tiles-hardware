#ifndef Feedbacks_Handler_h
#define Feedbacks_Handler_h

#include "Haptic.h"
#include "RGB_LED.h"
#include "NEO_STRIP.h"

class Feedbacks_Handler
{
    public:
      Feedbacks_Handler();   //default constructor
      
      String UpdateFeedback();
      void HandleTime(unsigned int ElapsedTime);

      // Haptic
      void setHapticMotor(Haptic *pHapticMotor);
      void Vibrate(unsigned int Time);
      void Vibrate(String Type);

      // RGB Led
      void setRGB_LED(RGB_LED *pLED);
      void setNEO_STRIP(NEO_STRIP *pSTRIP);
      void setColor(String color);
      
    private:
      Haptic *HapticMotor;
      bool HapticAvailable;

      RGB_LED *LED;
      bool RGB_LEDAvailable;

      NEO_STRIP *STRIP;
      bool NEO_STRIPAvailable;
 
      
};

#endif
