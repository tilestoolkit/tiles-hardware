#include "Accelerometer.h"

Accelerometer::Accelerometer(int Pin)
{
    InterruptPin = Pin;
    Tapped = false;
    DoubleTapped = false;
    State = false;
    Tilted = false;
    TiltAxis = 0;

    x = y = z = 0;
    d_x = d_y = d_z = 0;

    FIR_Sample = 0;
    
    for(int i = 0; i < ACC_FIR_SIZE; i++)
    {
        x_FIR[i] = 0;
        y_FIR[i] = 0;
        z_FIR[i] = 0;
    }

    pinMode(InterruptPin, INPUT);
    
    Serial.println("Checking for ADXL345");
    accel = Adafruit_ADXL345_Unified(12345);
	
	Wire.begin();
	Wire.beginTransmission(ADXL345_ADDRESS);
	#if ARDUINO >= 100
	Wire.write((uint8_t)0x00);
	#else
	Wire.send(0x00);
	#endif
	
    if(Wire.endTransmission() != 0)
    {
      Serial.println("No ADXL345");
	  SensorAvailable = false;	  
	  return;
    }
	
	else
	{
	  accel.begin();
      Serial.println("ADXL345 Available");
	  SensorAvailable = true;	  
	}

    /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_2_G);

    // enable single and double tap interrupt + activity/inactivity interrupts
    accel.writeRegister(ADXL345_REG_INT_ENABLE, 0b01111000);

    // map single tap, double tap, activity and inactivity interrupts in the INT1 pin
    accel.writeRegister(ADXL345_REG_INT_MAP, 0x87);

    /************************** Activity and Inactivity configuration **************************/
    accel.writeRegister(ADXL345_REG_THRESH_ACT, 8);
    accel.writeRegister(ADXL345_REG_THRESH_INACT, 3);
    accel.writeRegister(ADXL345_REG_TIME_INACT, 0b00000001);
    accel.writeRegister(ADXL345_REG_ACT_INACT_CTL, 0b11111111);

    /*********************** Tap and double tap configuration ************************/
    // single tap configuration
    accel.writeRegister(ADXL345_REG_DUR, 0x30);
    accel.writeRegister(ADXL345_REG_THRESH_TAP, 0x40);
    accel.writeRegister(ADXL345_REG_TAP_AXES, 0b001); // enable tap detection on the z axe

    // double tap configuration
    accel.writeRegister(ADXL345_REG_LATENT, 100);
    accel.writeRegister(ADXL345_REG_WINDOW, 255);
    /*************************************************************************************/

    //read and clear interrupts
    accel.readRegister(ADXL345_REG_INT_SOURCE);
}

bool Accelerometer::isShaked ()       // True if the sensor is shaked
{
    if(Shaked)
    {
      Shaked = false;
      return true;
    }

    else
      return false;
}

bool Accelerometer::isTapped ()       // True if the sensor has been tapped once
{
    if(Tapped)
    {
      Tapped = false;
      return true;
    }

    else
      return false;
}
bool Accelerometer::isDoubleTapped () // Returns true if the sensor has been double tapped
{
    if(DoubleTapped)
    {
      DoubleTapped = false;
      return true;
    }

    else
      return false;
}

int Accelerometer::isTilted ()        // Return the tilt axis according to enum Axis, else returns false
{
    if(Tilted)
    {
        Tilted = false;
        return TiltAxis;
    }

    else
        return 0;
}

bool Accelerometer::isActive()        // Return the state of the sensor (true = active, false = inactive)
{
    return State;
}



void Accelerometer::RefreshValues() // This function has to be adapted to the current used sensor
{
 
/**********************************************
 * Read new values, compute the new low pass
 * FIR outputs and store the delta with previous 
 * values
 *********************************************/
    
	if(SensorAvailable == false)
		return;
	
    int new_x = accel.getX();
    int new_y = accel.getY();
    int new_z = accel.getZ();

    d_x = new_x -x;
    d_y = new_y -y;
    d_z = new_z -z;
    
    x = new_x;
    y = new_y;
    z = new_z;
    
    if(FIR_Sample +1 == ACC_FIR_SIZE)
        FIR_Sample = 0;
    
    else
        FIR_Sample++;

    x_FIR[FIR_Sample] = new_x;
    y_FIR[FIR_Sample] = new_y;
    z_FIR[FIR_Sample] = new_z; 
    
    x_Filtered = 0;
    y_Filtered = 0;
    z_Filtered = 0;
  
    for(uint8_t i = 0; i < ACC_FIR_SIZE; i++)
    { 
        x_Filtered += x_FIR[i];
        y_Filtered += y_FIR[i];
        z_Filtered += z_FIR[i];
    }

    x_Filtered /= ACC_FIR_SIZE;  
    y_Filtered /= ACC_FIR_SIZE;
    z_Filtered /= ACC_FIR_SIZE;

    #ifdef LOG_ACC
    Serial.print("X");Serial.println(x, DEC);
    Serial.print("Y");Serial.println(y, DEC);
    Serial.print("Z");Serial.println(z, DEC);
    #endif
    
/**********************************************
 * Check if the pawn is tilted using the output
 * of a low pass FIR 
 *********************************************/
 
    if(abs(x_Filtered) > ACC_SHAKE_THRESH && abs(y_Filtered) > ACC_SHAKE_THRESH)
    {
        Tilted = true;
        TiltAxis = Accelerometer::Y_AXIS | Accelerometer::X_AXIS;
    }
    
    else if(abs(x_Filtered) > ACC_SHAKE_THRESH)
    {
        Tilted = true;
        TiltAxis = Accelerometer::X_AXIS;
    }
    
    else if(abs(y_Filtered) > ACC_SHAKE_THRESH)
    {
        Tilted = true;
        TiltAxis = Accelerometer::Y_AXIS;
    }

    else
        Tilted = false;


    
    //Populate sensor events if interrupt occured
    if (digitalRead(InterruptPin))  //Interrupt occured
    {
        int Source = accel.readRegister(ADXL345_REG_INT_SOURCE);
        
        if(abs(d_x) < 200 && abs(d_y) < 200)
        {
            if(Source & ACC_INT_TAP)
            {
                if(Source & ACC_INT_DOUBLE_TAP)
                {
                    DoubleTapped = 1;
                    Tapped = 0;
                }
            
                else
                {
                  DoubleTapped = 0;
                  Tapped = 1;
                }
            }
        }
  
        else if(abs(d_x) > 200 || abs(d_y) > 200)
        {
          Shaked = true;
        }
      
        if(Source & ACC_INT_ACT)
        {
            #ifdef LOG_ACC
            if(State == false)
              Serial.println("ACC Active !");
            #endif
            
            State = true;
        }
        
        if(Source & ACC_INT_INACT)
        {
            #ifdef LOG_ACC
            if(State == true)
              Serial.println("ACC inactive...");
            #endif
            
            State = false;
        }
    }

    return;  
}
