#include "Accelerometer.h"
#include "InertialCentral_LSM9DS0.h"
#include "BLE_Handler.h"

// class TokenSoloEvent_Handler - Handles all the TokenSoloEvent sources (Accelerometer, Gyro, Compass...)
class TokenSoloEvent_Handler
{
public:
TokenSoloEvent_Handler(BLE_Handler *Handler);         //default constructor
String pollEvent();          // If an event has occured returns the event code
void HandleTime(unsigned int ElapsedTime);

void setAccelerometer(Accelerometer *Acc);        // Set the private member _Accelerometer with an existing instance of an Accelerometer object
void setInertialCentral(InertialCentral_LSM9DS0 *InC);        // Set the private member _Accelerometer with an existing instance of an Accelerometer object

private:
bool EventTriggered;       // True if an event has occured, else false. Reset on read with pollEvent();
String EventString;             // Event code according to protocol.h

bool _InertialCentralAvailable;
bool _AccelerometerAvailable;

BLE_Handler *BLE;

//Accelrometer
Accelerometer *_Accelerometer;       // Handle an accelerometer object
      #define ACCELEROMETER_UPDATE  100   //Accelerometer refresh period
unsigned int Accelerometer_Timing;

//Sensors source of TokenSoloEvent
InertialCentral_LSM9DS0 *_InertialCentral;
      #define INERTIAL_CENTRAL_UPDATE  00   //Accelerometer refresh period
unsigned int InertialCentral_Timing;
};
