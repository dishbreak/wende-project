/**
*	RoverMotorController:
*		-Control the rover motors
*		-Interrupt Service Routines (ISR) for encoders
*	Author: Rover Team
*	Modified: 5/16/2012
*/

#ifndef RoverMotorController_h
#define RoverMotorController_h

#include "Arduino.h"

//main rover motor controller routines
void RoverMotorRoutines(int left, int right);

//left/right encoder ISR
void incLeftMotor();
void incRightMotor();

#endif
