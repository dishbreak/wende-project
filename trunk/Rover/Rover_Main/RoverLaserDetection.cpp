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
  static int adjusting = 0;
  static unsigned int curr_state = 0;
  //Serial.println("testing...");
  //delay(100);
  //Serial.println("testing...start");
  static unsigned int detector_history[100];
  static unsigned int lighting_history[100];
  static int history_index = 0;
  
  static int hit_cycles = 0;
  
  boolean bDetected = false;
  
  switch (curr_state)
  {
    case 0:
      Serial.println("--init--");
      Adjust_Current_Sync(Sensor_Offset);
      //fill the moving buffer...
      curr_detector_value=0;
      curr_light_value=0;
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
      adjusting = HIT_TIME^2;
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
      if((curr_detector_value/10 < (200+offsetDirection) && Sensor_Offset != MAX_SENSOR_OFFSET) || (curr_detector_value/10 > (900+offsetDirection) && Sensor_Offset !=0 )) 
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
        lighting_difference = curr_light_value - historic_light_value;
        /* We want to know if it is less, we should respond differently if it is lower shouldn't we
        if(curr_light_value > historic_light_value)
        {
          lighting_difference = curr_light_value - historic_light_value;
        }
        else
        {
          lighting_difference = historic_light_value - curr_light_value;
        }
        */
        if(lighting_difference > AMBIENT_LIGHT_LIMIT_POS || (lighting_difference < 0 && abs(lighting_difference) > AMBIENT_LIGHT_LIMIT_NEG))
        {
          
          /*Serial.println("--READJUST--");
          Serial.print("Values (Lighting Diff - Detection Diff): ");
          Serial.print(lighting_difference);
          Serial.print(" - ");
          Serial.print(historic_detector_value - curr_detector_value);
          Serial.print(" - ");
          Serial.print(curr_light_value);
          Serial.print(" - ");
          Serial.print(curr_detector_value);
          Serial.print(" - ");
          Serial.println(Sensor_Offset);*/
          //reset historical values
          historic_light_value = curr_light_value;
          historic_detector_value = curr_detector_value;
          //Not just reset, but re-initialize (lightign conditions have changed adjust)
          //curr_state = 0;
          adjusting = HIT_TIME*100;
        }
        else if (adjusting > 1)
        {
          adjusting -= 1;
        }
        else if (adjusting == 1)
        {
          hit_cycles = 0;
          adjusting = 0;
          historic_light_value = curr_light_value;
          historic_detector_value = curr_detector_value;
        }
        else if ( historic_detector_value < curr_detector_value && (curr_detector_value - historic_detector_value) > AMBIENT_DETECTION_LIMIT_NEG )
        {
          
          //Serial.println("--READJUST HIGH--");
          /*Serial.print("Values (Lighting Diff - Detection Diff): ");
          Serial.print(lighting_difference);
          Serial.print(" - ");
          Serial.print(curr_detector_value - historic_detector_value);
          Serial.print(" - ");
          Serial.print(curr_light_value);
          Serial.print(" - ");
          Serial.println(curr_detector_value);
          */
          historic_light_value = curr_light_value;
          historic_detector_value = curr_detector_value;
        }
        //check for detection
        else if(historic_detector_value >= curr_detector_value)
        {
          int detector_difference = historic_detector_value - curr_detector_value;
          //Serial.println(detector_difference);
          if(detector_difference > DETECTION_LOW && detector_difference < DETECTION_HIGH)
          {
            hit_cycles++;
            
            if(hit_cycles > HIT_TIME)
            {
              Serial.println("---DETECTION---");
              Serial.print("Values (Lighting Diff - Detection Diff): ");
              Serial.print(lighting_difference);
              Serial.print(" - ");
              Serial.print(detector_difference);
              Serial.print(" - ");
              Serial.print(curr_light_value);
              Serial.print(" - ");
              Serial.print(curr_detector_value);
              Serial.print(" - ");
              Serial.println(Sensor_Offset);


              digitalWrite(ROVER_LED_PIN,HIGH);
              
              bDetected = true;
            }
            else
            {
              int diff = HIT_TIME-hit_cycles;
              //Serial.print("Detection in ");
              //Serial.print(diff);
              //Serial.println("cycles");
              digitalWrite(ROVER_LED_PIN,LOW);
            }
          }
          else
          {
            hit_cycles = 0;
            digitalWrite(ROVER_LED_PIN,LOW);
          }
        }
        else
        {
          hit_cycles = 0;
          digitalWrite(ROVER_LED_PIN,LOW);
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
  else if (photodetectorVal < (200+offsetDirection) && Sensor_Offset != MAX_SENSOR_OFFSET)
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
  digitalWrite( SENSOR_OFFSET_CLOCK, LOW );
  for(int i = 0; i < SENSOR_OFFSET_BITS; i++)
  {
    //Note this is reversed since the buffers inver the signal
    if ( value & 128)
    {
      digitalWrite( SENSOR_OFFSET_DATA, LOW );
    }
    else
    {
      digitalWrite( SENSOR_OFFSET_DATA, HIGH );
    }
    //clock pulse, this is mighty fast
    digitalWrite( SENSOR_OFFSET_CLOCK, HIGH );
    value = value << 1;
    digitalWrite( SENSOR_OFFSET_CLOCK, LOW );
  }
/* Legacy Shifter
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
*/
}
/*Needed for legacy Shifter
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
*/
