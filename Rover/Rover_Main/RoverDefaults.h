/**
*	RoverDefaults:
*		-Contains all definitions used throughout the rover architecture
*		-Contains various flags such as ROVER_DEMO
*	Author: Rover Team
*	Modified: 2/22/2012
*/

#ifndef RoverDefaults_h
#define RoverDefaults_h

#include "Arduino.h"

//Rover DEMO flag - set to 1 if running a demo
//Will bypass normal rover code functionality
#define ROVER_DEMO_MODE	1

//misc defines, for debugging
//#define SERIAL_COMM_ENABLED	Serial.begin(9600)

#define PHOTO_DETECTOR_PIN	A0
#define AMBIENT_LIGHTING_PIN	A1
#define LED_READY_PIN	        8
#define LED_LOW_LIGHT_PIN       10
#define LED_MID_LIGHT_PIN       11
#define LED_HIGH_LIGHT_PIN      12
#define LED_ACTUAL_LIGHT_PIN    9

//LASER DETECTION DEFINES
#define DETECTION_THRESHOLD	23
#define DETECTION_ERROR_LOW	10
#define DETECTION_ERROR_HIGH    30
#define LIGHTING_CHANGE_THRESHOLD_LOW  15
#define LIGHTING_CHANGE_THRESHOLD_MID  20
#define LIGHTING_CHANGE_THRESHOLD_HIGH  30
#define LASER_CHANGE_THRESHOLD_LOW  5
#define LASER_CHANGE_THRESHOLD_HIGH  10
#define LIGHTING_CHANGE_ERROR  0

//Rover Processing State Machine
#define ROVER_DEMO_STATE	0
#define USER_INPUT_STATE	1
#define	ROVER_INIT_STATE	2
#define	ROVER_START_STATE	3
#define	CHECK_DETECTION_STATE	4
#define	ROVER_MOVEMENT_STATE	5
#define	ROVER_DETECTED_STATE	6
#define	ROVER_STOP_STATE	7


#endif