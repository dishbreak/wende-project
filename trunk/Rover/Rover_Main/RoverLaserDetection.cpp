#include "Arduino.h"
#include "RoverLaserDetection.h"
#include "RoverInterfaces.h"


static int detector_samples[10];
static int lighting_samples[10];
static unsigned int curr_detector_value = 0;
static unsigned int curr_light_value = 0;
static unsigned int historic_detector_value = 0;
static unsigned int historic_light_value = 0;
static int Sensor_Offset = 0;
static int ReInitCount = 0;
static int offsetDirection = 0;

boolean DetectionProcessing()
{
    //0 - INIT
  //1 - SAMPLING/SATURATION ADJUSTMENTS, then detection
  static unsigned int curr_state = 0;
  //Serial.println("testing...");
  //delay(100);
  //Serial.println("testing...start");
  static int HIT_TIME = 5;
  static unsigned int detector_history[100];
  static unsigned int lighting_history[100];
  static int history_index = 0;
  
  static int hit_cycles = 0;
  
  boolean bDetected = false;
  
  switch (curr_state)
  {
    case 0:
      //fill the moving buffer...
      for(int i = 0; i < 10; i++)
      {
        //delayMicroseconds(1000);
        detector_samples[i] = readLaserDetector();
        lighting_samples[i] = readLightSensor();
        curr_detector_value += detector_samples[i];
        curr_light_value += lighting_samples[i];
      }
      historic_detector_value = curr_detector_value;
      historic_light_value = curr_light_value;
      hit_cycles = 0;
      curr_state = 1;
      break;
      
    case 1:
    
      //sample
      //subtract off last value
      curr_detector_value -= detector_samples[0];
      curr_light_value -= lighting_samples[0];
      
      //shift array downward
      for(int i=1; i < 10; i++)
      {
        detector_samples[i-1] = detector_samples[i];
        lighting_samples[i-1] = lighting_samples[i];
      }
      
      //set top value
      detector_samples[9] = readLaserDetector();
      lighting_samples[9] = readLightSensor();
      
      //add top value
      curr_detector_value += detector_samples[9];
      curr_light_value += lighting_samples[9];
      //curr_detector_value = analogRead(A0);
      //curr_light_value = analogRead(A1);
      
      //add to the history
      detector_history[history_index] = curr_detector_value;
      lighting_history[history_index] = curr_light_value;
      history_index++;
      
      if(history_index == 100)
      {
        history_index = 0;
      }
    
      //check for saturation
      if((curr_detector_value/10 < (200+offsetDirection) && Sensor_Offset != 31) || (curr_detector_value/10 > (900+offsetDirection) && Sensor_Offset !=0 )) 
      {
        if(adjust_to_light_change(curr_detector_value/10) == 1)
        {
          //go back to init since we had to adjust to saturation
          curr_state = 0;
        }
      }
      else
      {
        //detection.
        //check which lighting change is bigger
        int lighting_difference = 0;
        if(curr_light_value > historic_light_value)
        {
          lighting_difference = curr_light_value - historic_light_value;
        }
        else
        {
          lighting_difference = historic_light_value - curr_light_value;
        }
        
        if(lighting_difference > 100)
        {
          //Serial.println("--READJUST--");
          //reset historical values
          historic_light_value = curr_light_value;
          historic_detector_value = curr_detector_value;
        }
        
        //check for detection
        if(historic_detector_value > curr_detector_value)
        {
          int detector_difference = historic_detector_value - curr_detector_value;
          //Serial.println(detector_difference);
          if(detector_difference > 250 && detector_difference < 500)
          {
            hit_cycles++;
            
            if(hit_cycles > HIT_TIME)
            {
              Serial.println("---DETECTION---");
              digitalWrite(12,HIGH);
              
              bDetected = true;
            }
            else
            {
              int diff = HIT_TIME-hit_cycles;
              //Serial.print("Detection in ");
              //Serial.print(diff);
              //Serial.println("cycles");
              digitalWrite(12,LOW);
            }
          }
          else
          {
            hit_cycles = 0;
            digitalWrite(12,LOW);
          }
        }
        else
        {
          hit_cycles = 0;
          digitalWrite(12,LOW);
        }  
      }
      break;
  }
  //delay(100);
  return bDetected;
}

int adjust_to_light_change(int photodetectorVal)
{
  //if we just started adjusting...
  if (offsetDirection==0)
  {
    //reset to initial circuit configuration
    Adjust_Current_Sync(0);
    //set the offset to 100
    offsetDirection=100;
    return 1;
  }

  //if we are almost saturated with this voltage step
  if (photodetectorVal > (900+offsetDirection) && Sensor_Offset != 0)
  {
    //move the step down one
    Sensor_Offset=Sensor_Offset-1;
    //turn on/off resistors
    Adjust_Current_Sync(Sensor_Offset);
    offsetDirection=-100;
    //still adjusting...
    return 1;
  }
  //if we are near the bottom of the scale with this voltage step
  else if (photodetectorVal < (200+offsetDirection) && Sensor_Offset != 31)
  {
    //move the step up one
    Sensor_Offset=Sensor_Offset+1;
    //tunr on/off resistors
    Adjust_Current_Sync(Sensor_Offset);
    offsetDirection=100;
    //still adjusting...
    return 1;
  }
	
  return 0;
}

void Adjust_Current_Sync(int value)
{
  if ( value & 1)
    Toggle_Res_On(3);
  else
    Toggle_Res_Off(3);

  if ( value & 2)
    Toggle_Res_On(4);
  else
    Toggle_Res_Off(4);

  if ( value & 4)
    Toggle_Res_On(5);
  else
    Toggle_Res_Off(5);

  if ( value & 8)
    Toggle_Res_On(6);
  else
    Toggle_Res_Off(6);

  if ( value & 16)
    Toggle_Res_On(7);
  else
    Toggle_Res_Off(7);

}

void Toggle_Res_On(int pin)
{
  pinMode( pin, OUTPUT );
  digitalWrite( pin, HIGH );
}

void Toggle_Res_Off(int pin)
{
  digitalWrite( pin, LOW );
  pinMode( pin, INPUT );
}

