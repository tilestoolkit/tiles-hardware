#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

#define ACC_INT_TAP           0x40
#define ACC_INT_DOUBLE_TAP    0x20
#define ACC_INT_ACT           0x10
#define ACC_INT_INACT         0x08
#define ACC_FIR_SIZE          (uint8_t)10
#define ACC_SHAKE_THRESH      20000
#define ACC_TILT_THRESH       10000
#define MAG_EVENT_THRESH      1500
//#define   LOG_INC

class InertialCentral_LSM9DS0
{

public:
InertialCentral_LSM9DS0();

//Accelerometer
int a_x;        /// X acceleration instantaneous value
int a_y;        /// Y acceleration instantaneous value
int a_z;        /// Z acceleration instantaneous value

int r_x;        /// X acceleration instantaneous value
int r_y;        /// Y acceleration instantaneous value
int r_z;        /// Z acceleration instantaneous value

bool isShaked ();             // True if the sensor has been tapped once
bool isTapped ();             // True if the sensor has been tapped once
bool isDoubleTapped ();       // Returns true if the sensor has been double tapped
short isRotated();            // Returns -1 if anti-trig rotation triggered, 1 if trig rotation
int isTilted ();              // Return the tilt axis according to enum Axis, else returns false
bool isActive();              // Return the state of the sensor (true = active, false = inactive)
void RefreshValues ();        // Reads the accelerations components
void HandleTime(unsigned int ElapsedTime);        // Handle timing for click

enum Axis {X_AXIS = 1, Y_AXIS = 2, Z_AXIS = 3};

bool SensorAvailable;

private:
Adafruit_LSM9DS0 Sensor;        /// Object representing the used sensor

// Sensor events ***********************
bool Shaked;              // True if sensor is shaked, Reset on read with isShaked()
bool Tapped;              // True if tapped once, Reset on read with isTapped()
bool DoubleTapped;        // True if tapped twice, Reset on read with isDoubleTapped()
bool Tilted;              // True if the accelerometer is tilted
int TiltAxis;             // Axis on wich accelerometer is tilted
bool State;               // True if the sensor is active, else false
// *************************************

// Accelerometer computing variables ***
int a_x_FIR[ACC_FIR_SIZE];          // Samples being filtered
int a_y_FIR[ACC_FIR_SIZE];          //
int a_z_FIR[ACC_FIR_SIZE];          //
uint8_t a_FIR_Sample;               // Current sample
int32_t a_x_Filtered;         // FIR Output for x
int32_t a_y_Filtered;         // FIR Output for y
int32_t a_z_Filtered;         // FIR Output for z
int d_a_x;                // d_x = x[n] - x[n-1]
int d_a_y;                // d_y = x[n] - y[n-1]
int d_a_z;                // d_z = x[n] - z[n-1]
// ************************************

// Shake and Tilt Variables ***********
#define SHAKE_TILT_TIME  500
unsigned int ShakeTilt_Timing;
//*************************************

// Tap processing variables ***********
bool FirstTap;                        // Goes true if a first tap has been detected and remains true until the TapWindow_Timing is over
bool SecondTap;
bool TapDebounce;
#define TAP_WINDOW_TIME   500         //Opening time of the Tap window
#define TAP_LATENCY_TIME  150         // Latency after the first tap
#define TAP_DEBOUNCE_TIME 400         // Debounce time after a succeeded click
unsigned int TapWindow_Timing;        // Count the time since the start of the TapWindow opened.
// ************************************


// Rotation processing variables ******
int MagCurrent_x, MagCurrent_y, MagCurrent_z;
#define ROT_SAMPLING_TIME   500        // Time between proccessing rotation event
unsigned int RotSampling_Timing;       // Timer
short Rotation;
bool TrigoRotated;                     // true if Trigo rotated, clear on call with isRotated()
bool HorraireRotated;                  // true if Anti trig rotated clear on call with isRotated()
// ************************************

};
