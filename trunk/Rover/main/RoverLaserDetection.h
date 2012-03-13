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

#define MIDDLING_SAMPLES  20
#define ADC_DETECTOR_SAMPLE_RATE  20
#define ADC_LIGHTING_SAMPLE_RATE  20
#define SETTLING_CYCLES  0  //capacitors removed

//Laser Detection data struct
typedef struct __sensor_data {
	unsigned int inst_value;		//instantaneous value
	int current_value;	//current avg'ed value
	int stable_value;	//current stable value
	unsigned long total;			//running total
	unsigned int num_samples;		//number of samples
	unsigned int samples[MIDDLING_SAMPLES];		//samples
	unsigned int address;				//location of sensor
        boolean sampled;         //enough samples?
        boolean stable;          //stable?
} sensor_data;



void DetectionProcessing(void);
boolean initialize_laser_detection(void);
boolean DetectLaser(void);
int adjust_to_light_change(int photodetectorVal);
void Adjust_Current_Sync(int value);
void Toggle_Res_On(int pin);
void Toggle_Res_Off(int pin);
boolean sample_adc(sensor_data* data, int sample_rate);
boolean sample_adc(sensor_data* data);



#endif
