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

// Definitions for how each movement mode is represented with an int
#define fast_mode		1
#define slow_mode		2
#define crawl_stop_mode 3
#define pass_thru_mode  4
#define spiral_mode		5

#define fast_Kp			0.2
#define fast_Ki			0.001
#define fast_Kd			0
#define slow_Kp			0.15
#define slow_Ki			0.0009
#define slow_Kd			0
#define fast_speed		200 // mm/s
#define slow_speed		50  // mm/s

//main rover movement routine
void RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor);

#endif
