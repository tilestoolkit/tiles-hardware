#include "InertialCentral_LSM9DS0.h"

InertialCentral_LSM9DS0::InertialCentral_LSM9DS0()
{
    Tapped = false;
    DoubleTapped = false;
    State = false;
    Tilted = false;
    TiltAxis = 0;
    TapWindow_Timing = 0;
    FirstTap = SecondTap = false;
    
    a_x = a_y = a_z = 0;
    d_a_x = d_a_y = d_a_z = 0;

    a_FIR_Sample = 0;
    
    for(int i = 0; i < ACC_FIR_SIZE; i++)
    {
        a_x_FIR[i] = 0;
        a_y_FIR[i] = 0;
        a_z_FIR[i] = 0;
    }
    
    Serial.println("Checking for LSM9DS0");
	
	Wire.begin();
	Wire.beginTransmission(LSM9DS0_ADDRESS_ACCELMAG);
	#if ARDUINO >= 100
	Wire.write((uint8_t)0x00);
	#else
	Wire.send(0x00);
	#endif
	
    if(Wire.endTransmission() != 0)
    {
      Serial.println("No LSM9DS0");
	  SensorAvailable = false;	  
	  return;
    }
	
	else
	{
	  Sensor.begin();
      Serial.println("LSM9DS0 available");
	  SensorAvailable = true;	  
	}

    // 1.) Set the accelerometer range
    Sensor.setupAccel(Sensor.LSM9DS0_ACCELRANGE_2G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
    //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
    
    // 2.) Set the magnetometer sensitivity
    Sensor.setupMag(Sensor.LSM9DS0_MAGGAIN_2GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
    //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);
    
    // 3.) Setup the gyroscope
    Sensor.setupGyro(Sensor.LSM9DS0_GYROSCALE_245DPS);
    //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
    //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);

    //Setting up click detections (see en.DM00087365.pdf @ p.67)
    // Also see STM AN2768 for further explanations on the settings
    Sensor.write8(XMTYPE, 0x1F, 0x04);  // 0x1F : CTRL_REG0_XM / 0x04 --> High pass filter enabled for Click
    Sensor.write8(XMTYPE, 0x38, 0x10);  // 0x10 : CLICK_CFG / 0x10 --> ZS = 1 If ZD = 1 not able to detect ZS !
    Sensor.write8(XMTYPE, 0x3A, 0x10);  // 0x3A : CLICK_THS --> Threshold (0x10 to 0x24)
    Sensor.write8(XMTYPE, 0x3D, 0x10);  // 0x3D : TIME_WINDOW --> ms
    Sensor.write8(XMTYPE, 0x3C, 0x10);  // 0x3C : TIME_LATENCY --> ms
    Sensor.write8(XMTYPE, 0x3B, 0x10);  // 0x3A : TIME_LIMIT --> ms

}

void InertialCentral_LSM9DS0::HandleTime(unsigned int ElapsedTime)
{
    if(FirstTap || TapDebounce)
        TapWindow_Timing += ElapsedTime;

    RotSampling_Timing += ElapsedTime;
    ShakeTilt_Timing += ElapsedTime;
}

short InertialCentral_LSM9DS0::isRotated()
{
    if(TrigoRotated)
    {
        TrigoRotated = false;
        return 1;
    }

    else if(HorraireRotated)
    {
        HorraireRotated = false;
        return -1;
    }

    else 
        return 0;
}

bool InertialCentral_LSM9DS0::isShaked ()       // True if the sensor is shaked
{
    if(Shaked)
    {
      Shaked = false;
      return true;
    }

    else
      return false;
}

bool InertialCentral_LSM9DS0::isTapped ()       // True if the sensor has been tapped once
{
    if(Tapped)
    {
      Tapped = false;
      return true;
    }

    else
      return false;
}
bool InertialCentral_LSM9DS0::isDoubleTapped () // Returns true if the sensor has been double tapped
{
    if(DoubleTapped)
    {
      DoubleTapped = false;
      return true;
    }

    else
      return false;
}

int InertialCentral_LSM9DS0::isTilted ()        // Return the tilt axis according to enum Axis, else returns false
{
    if(Tilted)
    {
        Tilted = false;
        return TiltAxis;
    }

    else
        return 0;
}

bool InertialCentral_LSM9DS0::isActive()        // Return the state of the sensor (true = active, false = inactive)
{
    return State;
}



void InertialCentral_LSM9DS0::RefreshValues() // This function has to be adapted to the current used sensor
{
 
/**********************************************
 * Read new values, compute the new low pass
 * FIR outputs and store the delta with previous 
 * values
 *********************************************/
 	if(SensorAvailable == false)
		return;
	
    Sensor.read();
    
    int new_a_x = Sensor.accelData.x;
    int new_a_y = Sensor.accelData.y;
    int new_a_z = Sensor.accelData.z;

    d_a_x = new_a_x -a_x;
    d_a_y = new_a_y -a_y;
    d_a_z = new_a_z -a_z;
    
    a_x = new_a_x;
    a_y = new_a_y;
    a_z = new_a_z;
    
    if(a_FIR_Sample +1 == ACC_FIR_SIZE)
        a_FIR_Sample = 0;
    
    else
        a_FIR_Sample++;

    a_x_FIR[a_FIR_Sample] = new_a_x;
    a_y_FIR[a_FIR_Sample] = new_a_y;
    a_z_FIR[a_FIR_Sample] = new_a_z; 
    
    a_x_Filtered = 0;
    a_y_Filtered = 0;
    a_z_Filtered = 0;
  
    for(uint8_t i = 0; i < ACC_FIR_SIZE; i++)
    { 
        a_x_Filtered += a_x_FIR[i];
        a_y_Filtered += a_y_FIR[i];
        a_z_Filtered += a_z_FIR[i];
    }

    a_x_Filtered /= ACC_FIR_SIZE;  
    a_y_Filtered /= ACC_FIR_SIZE;
    a_z_Filtered /= ACC_FIR_SIZE;

    // Gyro Rotation
        
    int g_x = Sensor.gyroData.x;
    int g_y = Sensor.gyroData.y;
    int g_z = Sensor.gyroData.z;

    #ifdef LOG_INC
    Serial.print("X");Serial.println(Sensor.magData.x, DEC);
    Serial.print("Y");Serial.println(Sensor.magData.y, DEC);
    Serial.print("Z");Serial.println(Sensor.gyroData.z, DEC);
    #endif
    
/**********************************************
 * Check if the pawn is tilted using the output
 * of a low pass FIR 
 *********************************************/

    int IntSource = 0;
    IntSource = Sensor.read8(XMTYPE, 0x39);

    if(abs(d_a_x) < 6000 && abs(d_a_y) < 6000)    // If the sensor isn't being shaked, we check for tap actions
    {
        if(IntSource & 0x04 && IntSource & 0x40 && IntSource & 0x08)
        {
            Serial.println("Tap !");
            
            if(FirstTap && TapWindow_Timing > TAP_LATENCY_TIME)
              SecondTap = true;
            else if(!TapDebounce)
            {
              FirstTap = true;
            }
        }
    }


    // We process tap events and check the timings        
    if(FirstTap)
    {
        if(TapWindow_Timing > TAP_LATENCY_TIME)
        {
            if(TapWindow_Timing < TAP_WINDOW_TIME && SecondTap)
            {
              
                DoubleTapped = true;
                Serial.println("Double click !");
                TapWindow_Timing = 0;
                TapDebounce = true;
                FirstTap = false;
            }
            
            else if(TapWindow_Timing > TAP_WINDOW_TIME)
            {
                FirstTap = false;
                TapDebounce = true;
                TapWindow_Timing = 0;
                Tapped = true;
                Serial.println("Single click !"); 
            }

            SecondTap = false;
        }

        Serial.println(TapWindow_Timing, DEC);
    }

    // Tap debouncer
    if(TapDebounce)
    {
        if(TapWindow_Timing > TAP_DEBOUNCE_TIME)
        {
            TapDebounce = false;
            TapWindow_Timing = 0;
        }
    }

    if(ShakeTilt_Timing >= SHAKE_TILT_TIME)
    {
        ShakeTilt_Timing = 0;
        // Shake event
        if(abs(d_a_x) > ACC_SHAKE_THRESH || abs(d_a_y) > ACC_SHAKE_THRESH)
        {
          if(abs(g_z) < 1000);
            Shaked = true;
        }
        
        //Tilt Events    
        if(abs(a_x_Filtered) > ACC_TILT_THRESH && abs(a_y_Filtered) > ACC_TILT_THRESH)
        {
            Tilted = true;
            TiltAxis = InertialCentral_LSM9DS0::Y_AXIS | InertialCentral_LSM9DS0::X_AXIS;
        }
        
        else if(abs(a_x_Filtered) > ACC_TILT_THRESH)
        {
            Tilted = true;
            TiltAxis = InertialCentral_LSM9DS0::X_AXIS;
        }
        
        else if(abs(a_y_Filtered) > ACC_TILT_THRESH)
        {
            Tilted = true;
            TiltAxis = InertialCentral_LSM9DS0::Y_AXIS;
        }
    
        else
            Tilted = false;

    }
    
    if(g_z > 5000)
        Rotation = 1;
    else if(g_z < -5000)
        Rotation = -1;
        
    // Rotation Events
    if(RotSampling_Timing >= ROT_SAMPLING_TIME)
    {
        RotSampling_Timing = 0;
        
        int new_m_x = Sensor.magData.x;
        int new_m_y = Sensor.magData.y;
        int new_m_z = Sensor.magData.z;
        
        if(abs(g_x) < 1500 || abs(g_y) < 1500 ||abs(g_z) < 1500)    // The sensor isn't being rotated
        {
            if(abs(new_m_x - MagCurrent_x) > MAG_EVENT_THRESH || abs(new_m_y - MagCurrent_y) > MAG_EVENT_THRESH)
            {
                if(Rotation > 0)
                {
                  TrigoRotated = true;
                  HorraireRotated = false;
                }
                else if(Rotation < 0)
                {
                  TrigoRotated = false;
                  HorraireRotated = true;
                }

                MagCurrent_x = new_m_x;
                MagCurrent_y = new_m_y;
            }
        }
    }
    
    return;  
}
