#include "TokenSoloEvent_Handler.h"

TokenSoloEvent_Handler::TokenSoloEvent_Handler(BLE_Handler *Handler)   //default constructor
{
    BLE = Handler;
    EventTriggered = false;
    EventString = String();

    //Accelerometer
    _Accelerometer = NULL;
    Accelerometer_Timing = 0;

    //InertialCentral
    _InertialCentral = NULL;
    InertialCentral_Timing = 0;
}


void TokenSoloEvent_Handler::HandleTime(unsigned int ElapsedTime)
{
    Accelerometer_Timing += ElapsedTime;  
    InertialCentral_Timing += ElapsedTime;
    _InertialCentral->HandleTime(ElapsedTime);
}

String TokenSoloEvent_Handler::pollEvent()    // If an event has occured returns the event code
{
    String AccEvent;
    String InertialCentralEvent;

    if(_AccelerometerAvailable == true && Accelerometer_Timing >= ACCELEROMETER_UPDATE)
    {
        Accelerometer_Timing = 0;
        
        _Accelerometer->RefreshValues();
        
        if (_Accelerometer->isTapped())
            AccEvent = String("tap");
        
        else if (_Accelerometer->isDoubleTapped())
            AccEvent = String("doubletap");
        
       // else if (_Accelerometer->isShaked())
        //    AccEvent = String("shake");
        
        //else if (_Accelerometer->isTilted())
        //    AccEvent = String("tilt");
    }

    if(_InertialCentralAvailable == true && InertialCentral_Timing >= ACCELEROMETER_UPDATE)
    {
        InertialCentral_Timing = 0;
        short isRotated = _InertialCentral->isRotated();
        
        _InertialCentral->RefreshValues();
        
        if (_InertialCentral->isTapped())
            InertialCentralEvent = String("tap");
        
        else if (_InertialCentral->isDoubleTapped())
            InertialCentralEvent = String("doubletap");
        
        else if (_InertialCentral->isShaked())
            InertialCentralEvent = String("shake");
        
        else if (_InertialCentral->isTilted())
            InertialCentralEvent = String("tilt");
        
        else if(isRotated != 0)
        {
            if(isRotated == 1)
                InertialCentralEvent = String("clockrot");
            else if(isRotated == -1)
                InertialCentralEvent = String("cclockrot");
        }
    }
    
    //Mix between all sensor events (Exemple : Gyro rotated & Acc tilted = Particular event)
    if(_InertialCentralAvailable == true)
      EventString = InertialCentralEvent; 

    else if(_AccelerometerAvailable == true)
      EventString = AccEvent; 

    else
      EventString = String("");

    if(EventString != String(""))
    {  
        TokenEvent Event;
        Event.set(EventString);
        BLE->SendEvent(&Event);        
    }
    
    return EventString;
}



void TokenSoloEvent_Handler::setAccelerometer(Accelerometer *Acc)  // Set the private member _Accelerometer with an existing instance of an Accelerometer object
{
    _Accelerometer = Acc;
    _AccelerometerAvailable = Acc->SensorAvailable;
}

void TokenSoloEvent_Handler::setInertialCentral(InertialCentral_LSM9DS0 *InC)  // Set the private member _Accelerometer with an existing instance of an Accelerometer object
{
    _InertialCentral = InC;
    _InertialCentralAvailable = InC->SensorAvailable;
}


