#include "RoverMovement.h"
#include "RoverMotorController.h"

/**
* RoverMovementRoutines: 
* inputs: int mode - The current movement mode
*         motor_data* leftMotor - current paramaters for the left motor (struct)
*         motor_data* rightMotor - current paramaters for the right motor (struct)
* outputs: leftMotor and rightMotor structs updated
* Description: Main execution of rover movement modes
*/
void RoverMovementRoutines(int mode, motor_data* leftMotor, motor_data* rightMotor) {

	//todo: make spin time random
	//todo: determine the max distance/time for each mode then add code to shutdown rover if exceeded
	
	static int init = 1; // if 1 we intitalize function
	static int spin_time = 5000; // time to spin in place (in ms)
	
	if (init == 1) {
		move_start = milli(); // get the current time and save it to track movement time (in ms)
		init = 0; // only init once
		
		// initialize motor struct parameters (rob - is this necessary here??)
		leftMotor->cumError = 0;
		leftMotor->lastError = 0;
		rightMotor->cumError = 0;
		rightMotor->lastError = 0;
	}
	
	move time = milli() - move_start; // compute the current time spent moving
	
	
	
	
	//if we are in a mode that needs to spin and we have not spun long enough
	if ((mode == fast_mode || mode == slow_mode || mode == crawl_stop_mode) && (move_time < spin_time)) {
		// Left Motor
		leftMotor->Kp = fast_Kp; // proportional gain value
		leftMotor->Ki = fast_Ki; // integrative gain value
		leftMotor->Kd = fast_Kd; // derivative gain value
		leftMotor->targetSpeed = fast_speed;
		//Right Motor
		rightMotor->Kp = fast_Kp; // proportional gain value
		rightMotor->Ki = fast_Ki; // integrative gain value
		rightMotor->Kd = fast_Kd; // derivative gain value
		rightMotor->targetSpeed = -fast_speed;
	}
	
	else if (mode == spiral_mode) {
		// set motor parameters to spiral 
		// need to know current time
	}
	//move straight at a fast speed
	else if (mode == fast_mode) {
		// Left Motor
		leftMotor->Kp = fast_Kp; // proportional gain value
		leftMotor->Ki = fast_Ki; // integrative gain value
		leftMotor->Kd = fast_Kd; // derivative gain value
		leftMotor->targetSpeed = fast_speed;
		//Right Motor
		rightMotor->Kp = fast_Kp; // proportional gain value
		rightMotor->Ki = fast_Ki; // integrative gain value
		rightMotor->Kd = fast_Kd; // derivative gain value
		rightMotor->targetSpeed = fast_speed;
	}
	
	// else, we are in a mode that requires straight, slow movement (slow mode, crawl&stop, pass through)
	else {
		// Left Motor
		leftMotor->Kp = slow_Kp; // proportional gain value
		leftMotor->Ki = slow_Ki; // integrative gain value
		leftMotor->Kd = slow_Kd; // derivative gain value
		leftMotor->targetSpeed = slow_speed;
		//Right Motor
		rightMotor->Kp = slow_Kp; // proportional gain value
		rightMotor->Ki = slow_Ki; // integrative gain value
		rightMotor->Kd = slow_Kd; // derivative gain value
		rightMotor->targetSpeed = slow_speed;
	}
	
}
