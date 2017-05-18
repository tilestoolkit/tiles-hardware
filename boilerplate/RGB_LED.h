/**************************************************************************
# 	NAME: TokenFeedback.h
# 	AUTHOR: Matthias Monnier (matthias.monnier@gmail.com)
# 	DATE: 16/12/2015
# 	LICENSE: Apache V2.0(cf. file license.txt in the main repository)
#
# 	Header of the library which manages the Token feedbacks
#
**************************************************************************/

#ifndef RGB_LED_h
#define RGB_LED_h

#include <Arduino.h>

#define ACTIVE true
#define INACTIVE false

#define COMMON_ANODE  1
#define COMMON_CATHODE  2

#define LED_TYPE COMMON_CATHODE

#if LED_TYPE == COMMON_ANODE
  #define LED_OFF HIGH
  #define LED_ON LOW
#else if LED_TYPE == COMMON_CATHODE
  #define LED_OFF LOW
  #define LED_ON HIGH
#endif

class RGB_LED
{
    public:
    		RGB_LED(int R, int G, int B);

        void RefreshValues();
        void HandleTime(unsigned int ElapsedTime);
        
        void setRGBaColor(uint8_t R, uint8_t G, uint8_t B, float a = 1.0);
		
	  private:
        unsigned int  RGB_LED_Timing;
        
        bool State;

        int _Common;
        int _R_LED;
        int _G_LED;
        int _B_LED;
		
};

#endif
