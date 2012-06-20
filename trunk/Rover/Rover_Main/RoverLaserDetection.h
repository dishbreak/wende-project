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
#define LASER_DETECTION         1

#define MAX_SAMPLES  1000

#define SENSOR_OFFSET_BITS      8
#define MAX_SENSOR_OFFSET       (2^SENSOR_OFFSET_BITS)-1
#define SETTLING_TIME  100  //used for serial shifter

#define DETECTION_LOW           180 //was 60
#define DETECTION_HIGH          500 //was 500
#define AMBIENT_LIGHT_LIMIT_POS 75  //False alarms all the way down to 40
#define AMBIENT_LIGHT_LIMIT_NEG 75  //False alarms all the way down to 40
#define AMBIENT_DETECTION_LIMIT_NEG 20  //False alarms all the way down to 20
#define HIT_TIME                15  //was 10
  
boolean DetectionProcessing(void);
int adjust_to_light_change(int photodetectorVal);
void Adjust_Current_Sync(int value);
void Toggle_Res_On(int pin);
void Toggle_Res_Off(int pin);

#endif

