#include "Haptic.h"

Haptic::Haptic(int HapticPin)
{
  _HapticPin = HapticPin;
	pinMode(_HapticPin, OUTPUT);
	digitalWrite(_HapticPin, LOW);
}

void Haptic::Vibrate(int ms)
{
	  VibrateTime = ms;
    Haptic_Timing = 0;
    StartVibrate();
}

void Haptic::VibrateShort()
{
	Vibrate(250);
}

void Haptic::VibrateLong()
{
	Vibrate(1000);
}

void Haptic::StartVibrate()
{
    digitalWrite(_HapticPin, HIGH);
    State = ACTIVE;
}

void Haptic::StopVibrate()
{
    digitalWrite(_HapticPin, LOW);
    State = INACTIVE;
}


void Haptic::HandleTime(unsigned int  ElapsedTime)
{
    Haptic_Timing += ElapsedTime;
}

void Haptic::RefreshValues()
{
    if(State == ACTIVE && Haptic_Timing > VibrateTime)
    {
        StopVibrate();
        Haptic_Timing = 0;
    }
}

