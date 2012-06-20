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

#define SENSOR_OFFSET_BITS      8
#define MAX_SENSOR_OFFSET       (2^SENSOR_OFFSET_BITS)-1
#define SETTLING_CYCLES  100  //capacitors removed
#define SETTLING_TIME  2      //2ms to settle...sigh

#define DETECTION_LOW           60 //was 250
#define DETECTION_HIGH          500
#define AMBIENT_LIGHT_LIMIT_POS 35  //False alarms all the way down to 20
#define AMBIENT_LIGHT_LIMIT_NEG 35  //False alarms all the way down to 20
#define AMBIENT_DETECTION_LIMIT_NEG 20  //False alarms all the way down to 20
#define HIT_TIME                10  //was 5
  
boolean DetectionProcessing(void);
int adjust_to_light_change(int photodetectorVal);
void Adjust_Current_Sync(int value);
void Toggle_Res_On(int pin);
void Toggle_Res_Off(int pin);

#endif
