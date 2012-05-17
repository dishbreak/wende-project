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
#define CALIBRATE_SENSORS       1
#define LASER_DETECTION         2
#define SAMPLE_SENSORS	        3
#define DETECT_LASER		4
#define	ADJUST_TO_LIGHTING	5
#define DETECTED_LASER	        6
#define SETTLE_FROM_ADJUST      7
#define RESAMPLE_AFTER_ADJUST   8

#define MAX_SAMPLES  1000


#define SETTLING_CYCLES  100  //capacitors removed
#define SETTLING_TIME  2      //2ms to settle...sigh

//Laser Detection data struct
typedef struct __sensor_data {
	int inst_value;		//instantaneous value
	int historic_value;	        //moving average value
        int current_value;      //current value, based on averaging a set number of samples
        unsigned int sample_index;
        int sample_rate;
	int samples[MAX_SAMPLES];		//samples
        int history[MAX_SAMPLES];
        unsigned int history_index;
	unsigned int address;				//location of sensor
        boolean sampled;         //enough samples?
        boolean recalibrate;      //calibrated?
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
boolean sample_adc(sensor_data* data, int sample_rate);
boolean sample_adc(sensor_data* data);



#endif
