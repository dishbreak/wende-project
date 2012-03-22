/**
*	RoverLaserDetection:
*		-Detect the laser, sample the ADC, etc
*	Author: Rover Team
*	Modified: 2/29/2012
*/

#ifndef RoverLaserDetection_h
#define RoverLaserDetection_h

#include "Arduino.h"

//states for state machine
#define INIT_SENSORS		0
#define SAMPLE_SENSORS	1
#define DETECT_LASER		2
#define	ADJUST_TO_LIGHTING	3
#define DETECTED_LASER	4
#define SETTLE_FROM_ADJUST  5
#define RESAMPLE_AFTER_ADJUST  6

#define MAX_SAMPLES  100

#define ADC_DETECTOR_SAMPLE_RATE  30
#define ADC_LIGHTING_SAMPLE_RATE  5
#define DETECTOR_AVG_RATE  10
#define LIGHTING_AVG_RATE  1
#define SETTLING_CYCLES  100  //capacitors removed
#define SETTLING_TIME  2      //2ms to settle...sigh

//Laser Detection data struct
typedef struct __sensor_data {
	int inst_value;		//instantaneous value
	int historic_value;	        //moving average value
        int current_value;      //current value, based on averaging a set number of samples
	unsigned long total;			//running total
        unsigned int sample_index;
	int samples[MAX_SAMPLES];		//samples
	unsigned int address;				//location of sensor
        boolean sampled;         //enough samples?
} sensor_data;


boolean compair_detector(sensor_data* data, int upper_threshold, int lower_threshold);
void DetectionProcessing(void);
void shift_average_history(sensor_data* data);
boolean initialize_laser_detection(void);
boolean DetectLaser(void);
int adjust_to_light_change(int photodetectorVal);
void Adjust_Current_Sync(int value);
void Toggle_Res_On(int pin);
void Toggle_Res_Off(int pin);
boolean sample_adc(sensor_data* data, int sample_rate, int avg_rate);
boolean sample_adc(sensor_data* data);



#endif
