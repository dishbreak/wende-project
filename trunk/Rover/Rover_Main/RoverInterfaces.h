/**
*	RoverInterfaces:
*		-Initialize rover inputs/outputs/interrupts
*		-Provide user friendly interface to access rover functionality
*	Author: Rover Team
*	Modified: 5/16/2012
*/

#ifndef RoverInterfaces_h
#define RoverInterfaces_h

#include "Arduino.h"

//Rover DEMO flag - set to 1 if running a demo
//Will bypass normal rover code functionality
#define ROVER_DEMO_MODE	1

//Rover debug flag...
#define DEBUG  0

//input pins
#define PHOTO_DETECTOR_PIN    A0
#define AMBIENT_LIGHTING_PIN  A1
#define ROVER_LED_PIN         1
#define ENCODER_R_PIN         2
#define ENCODER_L_PIN         3
#define INPUT_MODE_BIT0       1
#define INPUT_MODE_BIT1       1
#define INPUT_MODE_BIT2       1

//output pins
#define MOTOR_R_DIR           7
#define MOTOR_L_DIR           4
#define MOTOR_R_EN            6
#define MOTOR_L_EN            5
#define FORWARD   LOW
#define REVERSE   HIGH

//rover binary modes...
#define INPUT_IDLE_MODE            0
#define INPUT_SLOW_MODE            1
#define INPUT_FAST_MODE            2
#define INPUT_SPIRAL_MODE          3
#define INPUT_PASS_THROUGH_MODE    4
#define INPUT_CRAWL_AND_STOP_MODE  5

//rover delay...
#define INIT_DELAY_CYCLES    200

//setup inputs/outputs for the rover
//link interrupt routines for encoders
void setupInterfaces();

//read the current value of the laser detector
int readLaserDetector();

//read the current value of the ambient light sensor
int readLightSensor();

//read the current rover mode from the binary switch
int readRoverMode();

//contorl left/right motors
void setRightMotor(int dir, int val);
void setLeftMotor(int dir, int val);

#endif
