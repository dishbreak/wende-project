#include "Arduino.h"
#include "RoverLaserDetection.h"
#include "RoverDefaults.h"


static sensor_data laserData;
static sensor_data lightingData;

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
                //        Serial.println("Init Sensor");  
			//init the sensors
			if(initialize_laser_detection())
			{
                          digitalWrite(LED_LOW_LIGHT_PIN,LOW);
			  curr_state = SAMPLE_SENSORS;   
			}
			else
                        {
                          digitalWrite(LED_LOW_LIGHT_PIN,HIGH);
                        }
                        break;
		
		case SAMPLE_SENSORS:
                        //Serial.println("Sample Sensor"); 
			//sample adc on both channels
                        //sample if still necessary...
                        laserData.sampled = sample_adc(&laserData, ADC_DETECTOR_SAMPLE_RATE);
                        lightingData.sampled = sample_adc(&lightingData, ADC_LIGHTING_SAMPLE_RATE);

			if(laserData.sampled && lightingData.sampled)
			{
                                //check for a lighting difference
                                //small lighting changes on both sensors...
                                difference = abs(lightingData.stable_value-lightingData.current_value);
                                diff = abs(laserData.stable_value-laserData.current_value);
                                //Serial.print(difference);
                                //Serial.print(" > ");
                                //Serial.print(LIGHTING_CHANGE_THRESHOLD);
                                //Serial.print(" - ");
                                //Serial.println(diff);
                                if(difference > LIGHTING_CHANGE_THRESHOLD)
                                {
                                //    Serial.print("Adj:");
                                //    Serial.print(lightingData.current_value);
                                //    Serial.print(" - ");
                                //    Serial.println(lightingData.stable_value);
//                                    lightingData.total = 0;
//                                    laserData.total = 0;
//                                    lightingData.num_samples = 1;
//                                    laserData.num_samples = 1;
                          	    curr_state = ADJUST_TO_LIGHTING;
                                }
                                /*
                                else if ((difference) < (-2))// || (difference) > (DETECTION_THRESHOLD + DETECTION_ERROR))
                                {
//                                    Serial.print("diff: ");
//                                    Serial.println(difference);
                                    lightingData.previous_value = lightingData.current_value;
                                    laserData.previous_value = laserData.current_value;
                                    //lightingData.num_samples = 1;
                                    //laserData.num_samples = 1;
                          	    //curr_state = ADJUST_TO_LIGHTING;
                                }
                                */
                                else
                                {
                        //            Serial.print("value:");
                        //            Serial.print(laserData.current_value);
                        //            Serial.print(" - ");
                        //            Serial.println(laserData.stable_value);
                                    curr_state = DETECT_LASER;
                                }
			}
//                        else
//                        {
//                                lightingData.previous_value = lightingData.current_value;
//                                laserData.previous_value = laserData.current_value;
//                        }
			break;

		case ADJUST_TO_LIGHTING:
			//check for lighting adjustments
                        //Serial.println("Adjust to Lighting"); 
			if(adjust_to_light_change(laserData.current_value) == 0)
			{
                          laserData.sampled = false;
                          lightingData.sampled = false;
                          lightingData.stable = false;
                          laserData.stable = false;
		          curr_state = SETTLE_FROM_ADJUST;
                          settle = 0;
			}

			break;			

                case SETTLE_FROM_ADJUST:
                        if(settle == SETTLING_CYCLES)
                        {
                          digitalWrite(LED_HIGH_LIGHT_PIN,LOW);
                          curr_state = SAMPLE_SENSORS;
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
                                curr_state = DETECTED_LASER;
                        //        Serial.println("DETECTED LASER");
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
                        if(ROVER_DEMO_MODE == 1)
                        {
                          laserData.sampled = false;
                          lightingData.sampled = false;
                          curr_state = SAMPLE_SENSORS;
                        }
                        
                        //Serial.println("DETECTED LASER"); 
			break;
	}
}

boolean initialize_laser_detection()
{
	static unsigned int init = 0;

	if(!init)
	{
            
		lightingData.current_value = 0;
		lightingData.total = 0;
		lightingData.num_samples = 1;
		lightingData.address = AMBIENT_LIGHTING_PIN;
		lightingData.stable = false;
                lightingData.sampled = false;
            
                laserData.current_value = 0;
      	        laserData.total = 0;
      		laserData.num_samples = 1;
      		laserData.address = PHOTO_DETECTOR_PIN;
                laserData.stable = false;
                laserData.sampled  = false;
                
		init = true;
	}
	else
	{
                //sample if still necessary...
                laserData.sampled = sample_adc(&laserData, ADC_DETECTOR_SAMPLE_RATE);
                lightingData.sampled = sample_adc(&lightingData, ADC_LIGHTING_SAMPLE_RATE);
                
		if(laserData.sampled && lightingData.sampled)
		{
                  //ready for resampling
                  laserData.sampled = false;
                  lightingData.sampled = false;

                  return true;	
		}
	}
	

	return false;	
}

boolean DetectLaser()
{
	//compare laser detector to resistive detector?
	int difference = 0;
	//check for a significant change...
	difference = abs(laserData.stable_value-laserData.current_value); //needs to be tweaked
        
        /*
        Serial.print(DETECTION_THRESHOLD-DETECTION_ERROR);
        Serial.print(" < ");
        Serial.print(difference);
        Serial.print(" < ");
        Serial.println(DETECTION_THRESHOLD+DETECTION_ERROR);
        */
        
	if(difference > (DETECTION_THRESHOLD-DETECTION_ERROR_LOW) && difference < (DETECTION_THRESHOLD+DETECTION_ERROR_HIGH))
	{
		//detection!
		return true;
        }	
	return false;
}

int adjust_to_light_change(int photodetectorVal)
{

	static int Sensor_Offset = 0;
	static int offsetDirection=0;

	if (offsetDirection==0)
	{
		Adjust_Current_Sync(0);
		offsetDirection=100;
		return 1;
	}

	if (photodetectorVal > (900+offsetDirection) && Sensor_Offset != 0)
	{
		Sensor_Offset=Sensor_Offset-1;
		Adjust_Current_Sync(Sensor_Offset);
		offsetDirection=-100;
		return 1;
	}
	else if (photodetectorVal < (200+offsetDirection) && Sensor_Offset != 31)
	{
		Sensor_Offset=Sensor_Offset+1;
		Adjust_Current_Sync(Sensor_Offset);
		offsetDirection=100;
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

boolean sample_adc(sensor_data* data, int sample_rate)
{
  //do we need to sample?
  //used due to different sampling rates
  //allows for concurrent ADC execution
  if(data->sampled == true)
  {
    return true;
  }
  
  boolean sampled = false;
  data->inst_value = analogRead(data->address);
  data->total+=data->inst_value;
  
  if(data->num_samples == sample_rate)
  { 
    //take the avg
    data->current_value = data->total/data->num_samples;
    
    //if we aren't stable make this the new stable value
    if(data->stable == false)
    {
      data->stable_value = data->current_value;
      data->stable = true;
    }
    
    //reset samples
    data->num_samples = 1;
    
    //reset avg
    data->total = data->inst_value;
    
    //set sampled to true
    sampled = true;
    
  }
  data->num_samples++;
  return sampled;
}
/*
boolean sample_adc(sensor_data* data)
{
	boolean sampled = false;
	int lastVal = 0;
	data->inst_value = analogRead(data->address);
	data->total+=data->inst_value;
	
	if(data->num_samples == ADC_SAMPLE_RATE)
	{
		//moving average
		//find the last value
		lastVal = data->samples[0];
		
		//subtract off the last value
		data->total-=lastVal;
		
		//shift the array down one int
		//data->samples = data->samples;
		for(int i = 1; i < ADC_SAMPLE_RATE; i++)
                {
                  data->samples[i-1] = data->samples[i];
                }

		//add the top value
		data->samples[ADC_SAMPLE_RATE-1] = data->inst_value;
		//sampled enough...
		sampled = true;
	}
	else
	{
		//else just set data->sample[num_samples-1] to newest value (not enough samples)
		data->samples[data->num_samples-1] = data->inst_value;
		data->num_samples++;
                if (data->address == AMBIENT_LIGHTING_PIN)
                {
                  data->previous_value=(data->samples[ADC_SAMPLE_RATE-2]+data->samples[ADC_SAMPLE_RATE-3]+data->samples[ADC_SAMPLE_RATE-4]+data->samples[ADC_SAMPLE_RATE-5]+data->samples[ADC_SAMPLE_RATE-6])/5;
                }
                else
                {
                  data->previous_value = data->total/(data->num_samples-1);
                }
	}	

	//new avg
	data->current_value = data->total/(data->num_samples-1);
	
	return sampled;
}
*/
