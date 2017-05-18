#include "Feedbacks_Handler.h"

Feedbacks_Handler::Feedbacks_Handler()
{
    HapticAvailable = false;
    RGB_LEDAvailable = false;
}

String Feedbacks_Handler::UpdateFeedback()
{
  if(HapticAvailable)
    HapticMotor->RefreshValues();
}

void Feedbacks_Handler::HandleTime(unsigned int ElapsedTime)
{
  if(HapticAvailable)
    HapticMotor->HandleTime(ElapsedTime);
} 

void Feedbacks_Handler::setHapticMotor(Haptic *pHapticMotor)
{
    HapticMotor = pHapticMotor;
    HapticAvailable = true;
}

void Feedbacks_Handler::Vibrate(unsigned int Time)
{  
    if(HapticAvailable == false)
      return;     
    HapticMotor->Vibrate(Time);
}

void Feedbacks_Handler::Vibrate(String Type)
{
    if(HapticAvailable == false)
      return;    
    if(Type == String("short"))
      HapticMotor->VibrateShort();
    else if(Type == String("long"))
      HapticMotor->VibrateLong();
}


void Feedbacks_Handler::setRGB_LED(RGB_LED *pLED)
{
    LED = pLED;
    RGB_LEDAvailable = true;
}

void Feedbacks_Handler::setColor(String color)
{
  LED->setColor(color);
}


void Feedbacks_Handler::parseColorString(String color, int& red, int& green, int& blue)
{
  unsigned int colorHex = strtol(&color[0], NULL, 16);
  red = (int)((colorHex >> 16) & 0xFF);  // Extract the RR byte
  green = (int)((colorHex >> 8) & 0xFF); // Extract the GG byte
  blue = (int)((colorHex) & 0xFF);       // Extract the BB byte
}

void Feedbacks_Handler::setColor(String color)
{
  if (color == "off")
    LED->setRGBaColor(0, 0, 0);
  else if (color == "red")
    LED->setRGBaColor(255, 0, 0);
  else if (color == "green")
    LED->setRGBaColor(0, 255, 0);
  else if (color == "blue")
    LED->setRGBaColor(0, 0, 255);
  else if (color == "white")
    LED->setRGBaColor(255, 255, 255);
  else if (color.length() == 6)
  {
    int red, green, blue;
    parseColorString(color, red, green, blue);
    LED->setRGBaColor(red, green, blue);
  }
}
