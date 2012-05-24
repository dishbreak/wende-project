/**
*	RoverMovement:
*        Rover movement provides the brains for moving the rover in the correct state.  Functionality Includes:
*	  -Moving in proper movement states, including tracking where we are in a given state (ie. With crawl and stop we must know when to stop, slow down, etc.)
*	  -Communicate with the Rover Motor Control to ensure proper rover movement
*
*	Author: Rover Team
*	Modified: 5/16/2012
*/

#ifndef RoverMovement_h
#define RoverMovement_h

#include "Arduino.h"
#include "RoverMotorController.h"

//main rover movement routine
void RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor);

#endif
