/**************************************************************************
# 	NAME: TokenFeedback.h
# 	AUTHOR: Matthias Monnier (matthias.monnier@gmail.com)
# 	DATE: 16/12/2015
# 	LICENSE: Apache V2.0(cf. file license.txt in the main repository)
#
# 	Header of the library which manages the Token feedbacks
#
**************************************************************************/

#ifndef Haptic_h
#define Haptic_h

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"

#define ACTIVE true
#define INACTIVE false



class Haptic
{
    public:
    		Haptic(int pin_vibrating_motor);
    		void Vibrate(int ms);
    		void VibrateShort();
    		void VibrateLong();
        void RefreshValues();
        void HandleTime(unsigned int ElapsedTime);

        void VibrateCustom(int code);
		
	  private:
        void StartVibrate();
        void StopVibrate();
        
        unsigned int  Haptic_Timing;
        unsigned int  VibrateTime;
        bool State;
		    int _HapticPin;

        Adafruit_DRV2605 drv;
		
};

#endif
