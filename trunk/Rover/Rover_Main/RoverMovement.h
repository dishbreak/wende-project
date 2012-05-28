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
#define fast_mode		INPUT_FAST_MODE
#define slow_mode		INPUT_SLOW_MODE
#define crawl_mode 		INPUT_CRAWL_AND_STOP_MODE
#define pass_thru_mode          INPUT_PASS_THROUGH_MODE
#define spiral_mode		INPUT_SPIRAL_MODE

#define fast_Kp			0.2
#define fast_Ki			0.001
#define fast_Kd			0
#define slow_Kp			0.15
#define slow_Ki			0.0009
#define slow_Kd			0
#define fast_speed		200 // mm/s
#define slow_speed		50  // mm/s

#define fast_stoptime	12000 // ms, 2m/(.2m/s) + 20% buffer
#define slow_stoptime	48000 // ms, 2m/(.05m/s) + 20% buffer
#define crawl_stoptime	32400 //ms, (.7+.65)m/(0.5m/s) + 20% buffer
#define pass_stoptime	72000 //ms, 3m/(.05m/s) + 20% buffer
#define spiral_stoptime	80000 //ms, made up. will need to be updated based on testing.

//main rover movement routine
boolean RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor);
// determines if current mode has ran too long
int mode_over(unsigned long move_time, int mode);


#endif
