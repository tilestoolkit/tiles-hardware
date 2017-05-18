#include "ADXL345.h"
#include "LSM9DS0.h"
#include "BLE_Handler.h"

// class Sensors_Handler - Handles all the Sensor sources (Accelerometer, Gyro, Compass...)
class Sensors_Handler
{
    public:
      Sensors_Handler(BLE_Handler *Handler);      //default constructor
      String pollEvent();                         // If an event has occured returns the event code
      void HandleTime(unsigned int ElapsedTime);   
      void setAccelerometer(ADXL345 *Acc);  // Set the private member _Accelerometer with an existing instance of an Accelerometer object
      void setInertialCentral(LSM9DS0 *InC);  // Set the private member _Accelerometer with an existing instance of an Accelerometer object
      
    private:
      bool EventTriggered;      // True if an event has occured, else false. Reset on read with pollEvent();
      String EventString;       // Event code according to protocol.h
      bool _InertialCentralAvailable;
      bool _AccelerometerAvailable;
      
      BLE_Handler *BLE;

      //Accelrometer
      ADXL345 *_Accelerometer;            // Handle an accelerometer object
      #define ACCELEROMETER_UPDATE  100   //Accelerometer refresh period
      unsigned int Accelerometer_Timing;
      
      //Sensors source of TokenSoloEvent
      LSM9DS0 *_InertialCentral;
      #define INERTIAL_CENTRAL_UPDATE  00   //Accelerometer refresh period
      unsigned int InertialCentral_Timing;
};

