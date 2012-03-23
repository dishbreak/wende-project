#include "Arduino.h"
#include "RoverLaserDetection.h"
#include "RoverDefaults.h"


static sensor_data laserData;
static sensor_data lightingData;
static int Sensor_Offset = 0;
static int ReInitCount = 0;
static int offsetDirection = 0;

void DetectionProcessing()
{
  static int curr_state = INIT_SENSORS;
  static int settle = 0;
  int difference;
  int diff;
  //state machine...
  //path is init -> sample -> check lighting -> detect laser -> sample, or detected laser

  switch (curr_state)
  {
    case INIT_SENSORS:
      //init the sensors
      if(initialize_laser_detection())
      {
        digitalWrite(LED_LOW_LIGHT_PIN,LOW);
        curr_state = ADJUST_TO_LIGHTING;
//        curr_state = SAMPLE_SENSORS;   
      }
      else
      {
        digitalWrite(LED_LOW_LIGHT_PIN,HIGH);
      }
      break;
  		
    case SAMPLE_SENSORS:

      //sample adc on both channels
      laserData.sampled = sample_adc(&laserData, ADC_DETECTOR_SAMPLE_RATE);
//      Serial.print(laserData.inst_value);
//      Serial.print(" - "); 
//      Serial.print(laserData.current_value); 
//      Serial.print(" - "); 
//      Serial.println(Sensor_Offset);
      lightingData.sampled = sample_adc(&lightingData, ADC_LIGHTING_SAMPLE_RATE);

      if(laserData.sampled && lightingData.sampled)
      {
//        Serial.print(laserData.current_value); 
//        Serial.print(" - "); 
//        Serial.print(lightingData.current_value); 
//        Serial.print(" - "); 
//        Serial.println(Sensor_Offset);
        //check that we aren't saturated
        //added: if we are saturated we want to *attempt* to use one sensor for both detection/recalibration...
        if((laserData.current_value < (200+offsetDirection) && Sensor_Offset != 31) || (laserData.current_value > (900+offsetDirection) && Sensor_Offset !=0 )) 
        {
          curr_state = ADJUST_TO_LIGHTING;
        }
        else
        {
          /*
          int difference_lighting = abs(lightingData.historic_value - lightingData.current_value);
          if(difference_lighting > LIGHTING_CHANGE_THRESHOLD)
          {
            //reset info
            lightingData.sample_index = 0;
            lightingData.current_value = 0;
            lightingData.historic_value = 0;
            lightingData.total = 0;
            lightingData.sampled = false;
            
            laserData.current_value = 0;
            laserData.historic_value = 0;
            laserData.sample_index = 0;
            laserData.total = 0;
            laserData.sampled = false;
            
            curr_state = SAMPLE_SENSORS;
          }
          else
          {
            curr_state = DETECT_LASER;
          }
          */
          
          curr_state = DETECT_LASER;
        }
        //shift the average history in prep for a new sample set
        //shift_average_history(&laserData);
        //shift_average_history(&lightingData);
        laserData.sampled = false;
        lightingData.sampled = false;
      }

      break;
  
    case ADJUST_TO_LIGHTING:
      //check for lighting adjustments
      //Serial.println("Adjust to Lighting"); 
      if(adjust_to_light_change(laserData.current_value) == 1)
      {
        lightingData.sample_index = 0;
        lightingData.current_value = 0;
        lightingData.historic_value = 0;
        lightingData.total = 0;
        lightingData.sampled = false;
        
        laserData.current_value = 0;
        laserData.historic_value = 0;
        laserData.sample_index = 0;
        laserData.total = 0;
        laserData.sampled = false;
        
        for(int i = 0; i < ADC_DETECTOR_SAMPLE_RATE; i++)
        {
           laserData.samples[i] = 0;
        }
        for(int i = 0; i < ADC_LIGHTING_SAMPLE_RATE; i++)
        {
           lightingData.samples[i] = 0;
        }

        curr_state = SETTLE_FROM_ADJUST;
        settle = 0;
      }
      //adding fix for ambient lighting recalibration...
      //after an adjustment we need to resample before the next adjustment
      //this will ensure the sensor is stable before it actually starts resampling
      else
      {
        laserData.sampled = false;
        lightingData.sampled = false;
        //resample
        //curr_state = RESAMPLE_AFTER_ADJUST;
        curr_state = SAMPLE_SENSORS;
        
      }
      break;			
  
    case RESAMPLE_AFTER_ADJUST:
      //resample sensors
      laserData.sampled = sample_adc(&laserData, ADC_DETECTOR_SAMPLE_RATE);
//      delay(SETTLING_TIME);
      lightingData.sampled = sample_adc(&lightingData, ADC_LIGHTING_SAMPLE_RATE);
//      delay(SETTLING_TIME);
      //if we have a new sample, go back to the lighting adjust
      //this allows us to step down properly versus stepping once
      if(laserData.sampled && lightingData.sampled)
      {
        laserData.sampled = false;
        lightingData.sampled = false;
        curr_state = ADJUST_TO_LIGHTING;
      }
      
      break;
  
    case SETTLE_FROM_ADJUST:
      if(settle == SETTLING_CYCLES)
      {
        digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
        curr_state = RESAMPLE_AFTER_ADJUST;
//        curr_state = SAMPLE_SENSORS;
      }
      else if(settle > SETTLING_CYCLES)
      {
        settle=0;
        digitalWrite(LED_HIGH_LIGHT_PIN,HIGH);
      }
      else
      {
        digitalWrite(LED_HIGH_LIGHT_PIN,HIGH);
      }
      settle++;
      
      break;
  
    case DETECT_LASER:
    
      //check for a laser detection
      //Serial.println("DETECT LASER"); 
      if(DetectLaser())
      {
        digitalWrite(LED_MID_LIGHT_PIN,HIGH);
        curr_state = DETECTED_LASER;
        //Display detection history information for debug
        Serial.println("DETECTED LASER");
        Serial.print("Lighting current / Historic = ");
        Serial.print(lightingData.current_value); 
        Serial.print("-");
        Serial.println(lightingData.historic_value);
        Serial.print("Laser current / Historic = ");
        Serial.print(laserData.current_value); 
        Serial.print("-");
        Serial.println(laserData.historic_value);
        Serial.print("Sensor Offset = ");
        Serial.println(Sensor_Offset);
        Serial.println("Laser Samples: (Ambient Laser)");
        for(int history_num=0; history_num<MAX_SAMPLES; history_num++)
        {
          Serial.print(lightingData.samples[history_num]);
          Serial.print(" ");
          Serial.println(laserData.samples[history_num]);
        }
      }
      else
      {
        //turn LED off
        digitalWrite(LED_MID_LIGHT_PIN,LOW);
        //restart
        laserData.sampled = false;
        lightingData.sampled = false;
        curr_state = SAMPLE_SENSORS;
      }
      break;
  		
    case DETECTED_LASER:
      digitalWrite(LED_MID_LIGHT_PIN,HIGH);
      //if demo mode continue to loop
//      if(ROVER_DEMO_MODE == 1)
      //just stop to prove no false alarms
      if (0)
      {
        laserData.sampled = false;
        lightingData.sampled = false;
        curr_state = SAMPLE_SENSORS;
      }
      break;
  }
}

boolean initialize_laser_detection()
{
  static unsigned int init = 0;

  if(!init)
  {
            
    lightingData.current_value = 0;
    lightingData.historic_value = 0;
    lightingData.total = 0;
    lightingData.sample_index = 0;
    lightingData.address = AMBIENT_LIGHTING_PIN;
    lightingData.sampled = false;
//    analogRead(lightingData.address);

    laserData.current_value = 0;
    laserData.historic_value = 0;
    laserData.total = 0;
    laserData.sample_index = 0;
    laserData.address = PHOTO_DETECTOR_PIN;
    laserData.sampled  = false;
//    analogRead(laserData.address);

    init = true;
    return false;
  }
  //sample if still necessary...
  laserData.sampled = sample_adc(&laserData, ADC_DETECTOR_SAMPLE_RATE);
  //delay(SETTLING_TIME);
  lightingData.sampled = sample_adc(&lightingData, ADC_LIGHTING_SAMPLE_RATE);
  //delay(SETTLING_TIME);
                
  if(laserData.sampled && lightingData.sampled)
  {
    //ready for resampling
    laserData.sampled = false;
    lightingData.sampled = false;
    return true;	
  }
  return false;
}

boolean DetectLaser()
{
  //compare laser detector to resistive detector?
  //check for a significant change...
  //Laser will only impart a decrease in detection voltage, ambient light can be either way
//  int difference_detector = laserData.old_value[laserData.history_index-1] - laserData.current_value;
  int difference_lighting = abs(lightingData.historic_value - lightingData.current_value);

  if(difference_lighting > LIGHTING_CHANGE_THRESHOLD)
  {
    lightingData.historic_value = lightingData.current_value;
    laserData.historic_value = laserData.current_value;
    difference_lighting = 0;
  }

  int difference_detector = laserData.historic_value - laserData.current_value;
//  int difference_lighting = abs(lightingData.old_value[lightingData.history_index-1] - lightingData.current_value);
//Serial.print(laserData.inst_value);
//Serial.print(" - ");
//Serial.print(laserData.current_value);
//Serial.print(" - ");
//Serial.print(laserData.old_value[0]);
//Serial.print(" - ");
//Serial.println(difference_detector);

  if ((difference_detector > DETECTION_ERROR_LOW && difference_detector < DETECTION_ERROR_HIGH) && 
      (difference_lighting < LIGHTING_CHANGE_THRESHOLD))
  {
    //detection!
    return true;
  }	
  return false;
}

int adjust_to_light_change(int photodetectorVal)
{
//  static int offsetDirection=0;

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
//  pinMode( pin, OUTPUT );
  digitalWrite( pin, HIGH );
}

void Toggle_Res_Off(int pin)
{
  digitalWrite( pin, LOW );
  //pinMode( pin, INPUT );
}

/* Moving average */
boolean sample_adc(sensor_data* data, int sample_rate)
{
  static int old_sample_count = 0;
  //do we need to sample?
  //used due to different sampling rates
  //allows for concurrent ADC execution
  //deprecated on 3/21 - Robert
  //if(data->sampled == true)
  //{
  //  return true;
  //}
  
  int lastVal = 0;
  boolean sampled = false;
  data->inst_value = analogRead(data->address);
  data->total+=data->inst_value;
  
  if(data->sample_index == sample_rate-1)
  {
    if( data->samples[sample_rate-1] == 0)
    {
      data->samples[sample_rate-1] = data->inst_value;
    }
    
    //moving average		
    //shift the array down one int
    //data->samples = data->samples;
    for(int i = 1; i < sample_rate; i++)
    {
      data->samples[i-1] = data->samples[i];
    }

    //add the top value
    data->samples[sample_rate-1] = data->inst_value;

    data->total=0;

    for(int i = 0; i < sample_rate; i++)
    { 
      data->total+=data->samples[i];
    }

    //"moving" average
    data->current_value = data->total/(sample_rate);
    
    //sampled enough...
    sampled = true;
  }
  else
  { 
    //else just set data->sample[sample_index] to newest value (not enough samples)
    data->samples[data->sample_index] = data->inst_value;
    data->sample_index++;
    data->current_value = data->total/(data->sample_index);
    //new sample, no history
    data->historic_value = data->current_value;
  }
  
  //new avg
//  data->current_value = data->total/(data->sample_index+1);
  
  return sampled;
}

